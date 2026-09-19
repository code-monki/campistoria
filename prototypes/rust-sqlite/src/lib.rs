//! Local persistence prototype for HLA-PERSIST, HLA-CORE, and HLA-OBSERVER.
//! Exercises FR-031, FR-032, FR-041, NFR-003, and NFR-006 storage concerns.

use rusqlite::{Connection, params};
use serde::{Deserialize, Serialize};
use serde_json::Value;
use std::path::Path;

mod ffi;

const SCHEMA: &str = "
    CREATE TABLE IF NOT EXISTS reality_events (
        id INTEGER PRIMARY KEY,
        campaign_id TEXT NOT NULL CHECK (length(trim(campaign_id)) > 0),
        kind TEXT NOT NULL CHECK (length(trim(kind)) > 0),
        payload TEXT NOT NULL
    );
    CREATE TABLE IF NOT EXISTS knowledge_events (
        id INTEGER PRIMARY KEY,
        campaign_id TEXT NOT NULL CHECK (length(trim(campaign_id)) > 0),
        kind TEXT NOT NULL CHECK (length(trim(kind)) > 0),
        payload TEXT NOT NULL
    );
";

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Stream {
    Reality,
    Knowledge,
}

impl Stream {
    fn table(self) -> &'static str {
        match self {
            Self::Reality => "reality_events",
            Self::Knowledge => "knowledge_events",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Event {
    pub campaign_id: String,
    pub kind: String,
    pub payload: Value,
}

#[derive(Debug, Serialize, Deserialize)]
struct Snapshot {
    version: u32,
    reality_events: Vec<Event>,
    knowledge_events: Vec<Event>,
}

#[derive(Debug)]
pub enum Error {
    Invalid(&'static str),
    Sql(rusqlite::Error),
    Json(serde_json::Error),
}

impl From<rusqlite::Error> for Error {
    fn from(value: rusqlite::Error) -> Self {
        Self::Sql(value)
    }
}

impl From<serde_json::Error> for Error {
    fn from(value: serde_json::Error) -> Self {
        Self::Json(value)
    }
}

pub struct Store {
    connection: Connection,
}

impl Store {
    pub fn open(path: impl AsRef<Path>) -> Result<Self, Error> {
        let connection = Connection::open(path)?;
        connection.execute_batch(SCHEMA)?;
        Ok(Self { connection })
    }

    pub fn append(&self, stream: Stream, event: &Event) -> Result<(), Error> {
        validate(event)?;
        let sql = format!(
            "INSERT INTO {} (campaign_id, kind, payload) VALUES (?1, ?2, ?3)",
            stream.table()
        );
        self.connection.execute(
            &sql,
            params![event.campaign_id, event.kind, event.payload.to_string()],
        )?;
        Ok(())
    }

    pub fn events(&self, stream: Stream) -> Result<Vec<Event>, Error> {
        let sql = format!(
            "SELECT campaign_id, kind, payload FROM {} ORDER BY id",
            stream.table()
        );
        let mut statement = self.connection.prepare(&sql)?;
        let rows = statement.query_map([], |row| {
            Ok((
                row.get::<_, String>(0)?,
                row.get::<_, String>(1)?,
                row.get::<_, String>(2)?,
            ))
        })?;
        let mut events = Vec::new();
        for row in rows {
            let (campaign_id, kind, payload) = row?;
            events.push(Event {
                campaign_id,
                kind,
                payload: serde_json::from_str(&payload)?,
            });
        }
        Ok(events)
    }

    pub fn export_json(&self) -> Result<String, Error> {
        let snapshot = Snapshot {
            version: 1,
            reality_events: self.events(Stream::Reality)?,
            knowledge_events: self.events(Stream::Knowledge)?,
        };
        Ok(serde_json::to_string_pretty(&snapshot)?)
    }

    pub fn import_json(&mut self, input: &str) -> Result<(), Error> {
        let snapshot: Snapshot = serde_json::from_str(input)?;
        if snapshot.version != 1 {
            return Err(Error::Invalid("unsupported snapshot version"));
        }
        let transaction = self.connection.transaction()?;
        for (table, events) in [
            (Stream::Reality.table(), snapshot.reality_events),
            (Stream::Knowledge.table(), snapshot.knowledge_events),
        ] {
            for event in events {
                validate(&event)?;
                let sql =
                    format!("INSERT INTO {table} (campaign_id, kind, payload) VALUES (?1, ?2, ?3)");
                transaction.execute(
                    &sql,
                    params![event.campaign_id, event.kind, event.payload.to_string()],
                )?;
            }
        }
        transaction.commit()?;
        Ok(())
    }
}

fn validate(event: &Event) -> Result<(), Error> {
    if event.campaign_id.trim().is_empty() || event.kind.trim().is_empty() {
        return Err(Error::Invalid("campaign_id and kind must be nonempty"));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;
    use std::time::{SystemTime, UNIX_EPOCH};

    fn event(kind: &str) -> Event {
        Event {
            campaign_id: "campaign-1".into(),
            kind: kind.into(),
            payload: json!({"source": "test"}),
        }
    }

    #[test]
    fn export_import_preserves_separate_ordered_streams() {
        let source = Store::open(":memory:").unwrap();
        source.append(Stream::Reality, &event("created")).unwrap();
        source
            .append(Stream::Knowledge, &event("observed"))
            .unwrap();
        source.append(Stream::Reality, &event("resolved")).unwrap();

        let mut destination = Store::open(":memory:").unwrap();
        destination
            .import_json(&source.export_json().unwrap())
            .unwrap();

        assert_eq!(
            destination.events(Stream::Reality).unwrap(),
            vec![event("created"), event("resolved")]
        );
        assert_eq!(
            destination.events(Stream::Knowledge).unwrap(),
            vec![event("observed")]
        );
    }

    #[test]
    fn invalid_import_rolls_back_earlier_inserts() {
        let mut store = Store::open(":memory:").unwrap();
        let invalid = json!({
            "version": 1,
            "reality_events": [event("valid"), event(" ")],
            "knowledge_events": []
        });

        assert!(store.import_json(&invalid.to_string()).is_err());
        assert!(store.events(Stream::Reality).unwrap().is_empty());
    }

    #[test]
    fn rejected_append_has_no_side_effect() {
        let store = Store::open(":memory:").unwrap();
        assert!(store.append(Stream::Reality, &event("")).is_err());
        assert!(store.events(Stream::Reality).unwrap().is_empty());
    }

    #[test]
    fn file_backed_events_survive_reopen() {
        let stamp = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_nanos();
        let path = std::env::temp_dir().join(format!(
            "campistoria-spike-{}-{stamp}.sqlite",
            std::process::id()
        ));
        {
            let store = Store::open(&path).unwrap();
            store.append(Stream::Reality, &event("created")).unwrap();
        }
        let reopened = Store::open(&path).unwrap();
        assert_eq!(
            reopened.events(Stream::Reality).unwrap(),
            vec![event("created")]
        );
        drop(reopened);
        std::fs::remove_file(path).unwrap();
    }

    #[test]
    fn imports_shared_snapshot_fixture() {
        let mut store = Store::open(":memory:").unwrap();
        store
            .import_json(include_str!("../../fixtures/snapshot-v1.json"))
            .unwrap();
        assert_eq!(
            store.events(Stream::Reality).unwrap(),
            vec![event("created"), event("resolved")]
        );
        assert_eq!(
            store.events(Stream::Knowledge).unwrap(),
            vec![event("observed")]
        );
    }
}
