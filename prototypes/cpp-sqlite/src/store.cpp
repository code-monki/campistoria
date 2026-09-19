// SQLite adapter experiment for HLA-PERSIST; not a product contract.
// Exercises FR-031, FR-032, FR-041, NFR-003, and NFR-006 storage concerns.

#include "store.hpp"

#include <algorithm>
#include <cctype>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace campistoria_prototype {
namespace {

constexpr const char* schema = R"sql(
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
)sql";

const char* table_for(Stream stream) {
  return stream == Stream::Reality ? "reality_events" : "knowledge_events";
}

bool blank(const std::string& value) {
  return std::all_of(value.begin(), value.end(), [](unsigned char ch) {
    return std::isspace(ch) != 0;
  });
}

void validate(const Event& event) {
  if (blank(event.campaign_id) || blank(event.kind)) {
    throw std::invalid_argument("campaign_id and kind must be nonempty");
  }
}

using Statement = std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

Statement prepare(sqlite3* connection, const std::string& sql) {
  sqlite3_stmt* raw = nullptr;
  if (sqlite3_prepare_v2(connection, sql.c_str(), -1, &raw, nullptr) != SQLITE_OK) {
    throw std::runtime_error(sqlite3_errmsg(connection));
  }
  return Statement(raw, sqlite3_finalize);
}

void bind_text(sqlite3* connection, sqlite3_stmt* statement, int index,
               const std::string& value) {
  if (sqlite3_bind_text(statement, index, value.c_str(), -1, SQLITE_TRANSIENT) !=
      SQLITE_OK) {
    throw std::runtime_error(sqlite3_errmsg(connection));
  }
}

}  // namespace

void to_json(nlohmann::json& output, const Event& event) {
  output = {{"campaign_id", event.campaign_id},
            {"kind", event.kind},
            {"payload", event.payload}};
}

void from_json(const nlohmann::json& input, Event& event) {
  event.campaign_id = input.at("campaign_id").get<std::string>();
  event.kind = input.at("kind").get<std::string>();
  event.payload = input.at("payload");
}

Store::Store(const std::string& path) {
  if (sqlite3_open(path.c_str(), &connection_) != SQLITE_OK) {
    const std::string message = sqlite3_errmsg(connection_);
    sqlite3_close(connection_);
    throw std::runtime_error(message);
  }
  try {
    execute(schema);
  } catch (...) {
    sqlite3_close(connection_);
    throw;
  }
}

Store::~Store() { sqlite3_close(connection_); }

void Store::execute(const char* sql) const {
  char* error = nullptr;
  if (sqlite3_exec(connection_, sql, nullptr, nullptr, &error) != SQLITE_OK) {
    const std::string message = error != nullptr ? error : sqlite3_errmsg(connection_);
    sqlite3_free(error);
    throw std::runtime_error(message);
  }
}

void Store::insert(const char* table, const Event& event) {
  validate(event);
  auto statement = prepare(
      connection_, std::string("INSERT INTO ") + table +
                       " (campaign_id, kind, payload) VALUES (?1, ?2, ?3)");
  bind_text(connection_, statement.get(), 1, event.campaign_id);
  bind_text(connection_, statement.get(), 2, event.kind);
  bind_text(connection_, statement.get(), 3, event.payload.dump());
  if (sqlite3_step(statement.get()) != SQLITE_DONE) {
    throw std::runtime_error(sqlite3_errmsg(connection_));
  }
}

void Store::append(Stream stream, const Event& event) {
  insert(table_for(stream), event);
}

std::vector<Event> Store::events(Stream stream) const {
  auto statement = prepare(connection_, std::string("SELECT campaign_id, kind, payload FROM ") +
                                            table_for(stream) + " ORDER BY id");
  std::vector<Event> result;
  int status = SQLITE_ROW;
  while ((status = sqlite3_step(statement.get())) == SQLITE_ROW) {
    Event event;
    event.campaign_id = reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 0));
    event.kind = reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 1));
    event.payload = nlohmann::json::parse(
        reinterpret_cast<const char*>(sqlite3_column_text(statement.get(), 2)));
    result.push_back(std::move(event));
  }
  if (status != SQLITE_DONE) {
    throw std::runtime_error(sqlite3_errmsg(connection_));
  }
  return result;
}

std::string Store::export_json() const {
  nlohmann::json snapshot = {
      {"version", 1},
      {"reality_events", events(Stream::Reality)},
      {"knowledge_events", events(Stream::Knowledge)},
  };
  return snapshot.dump(2);
}

void Store::import_json(const std::string& input) {
  const auto snapshot = nlohmann::json::parse(input);
  if (snapshot.at("version").get<int>() != 1) {
    throw std::invalid_argument("unsupported snapshot version");
  }
  const auto reality = snapshot.at("reality_events").get<std::vector<Event>>();
  const auto knowledge = snapshot.at("knowledge_events").get<std::vector<Event>>();

  execute("BEGIN IMMEDIATE");
  try {
    for (const auto& event : reality) {
      insert("reality_events", event);
    }
    for (const auto& event : knowledge) {
      insert("knowledge_events", event);
    }
    execute("COMMIT");
  } catch (...) {
    sqlite3_exec(connection_, "ROLLBACK", nullptr, nullptr, nullptr);
    throw;
  }
}

}  // namespace campistoria_prototype
