// Focused parity tests for the HLA-PERSIST C++/SQLite experiment.
// Exercises FR-031, FR-032, FR-041, NFR-003, and NFR-006 storage concerns.

#include "store.hpp"

#include <nlohmann/json.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using campistoria_prototype::Event;
using campistoria_prototype::Store;
using campistoria_prototype::Stream;

namespace {

void expect(bool condition) {
  if (!condition) {
    throw std::runtime_error("prototype test assertion failed");
  }
}

Event event(const std::string& kind) {
  return {"campaign-1", kind, {{"source", "test"}}};
}

void export_import_preserves_separate_ordered_streams() {
  Store source(":memory:");
  source.append(Stream::Reality, event("created"));
  source.append(Stream::Knowledge, event("observed"));
  source.append(Stream::Reality, event("resolved"));

  Store destination(":memory:");
  destination.import_json(source.export_json());

  expect(destination.events(Stream::Reality) ==
         std::vector<Event>({event("created"), event("resolved")}));
  expect(destination.events(Stream::Knowledge) ==
         std::vector<Event>({event("observed")}));
}

void invalid_import_rolls_back_earlier_inserts() {
  Store store(":memory:");
  nlohmann::json invalid = {
      {"version", 1},
      {"reality_events", nlohmann::json::array({event("valid"), event(" ")})},
      {"knowledge_events", nlohmann::json::array()},
  };

  bool rejected = false;
  try {
    store.import_json(invalid.dump());
  } catch (const std::invalid_argument&) {
    rejected = true;
  }
  expect(rejected);
  expect(store.events(Stream::Reality).empty());
}

void rejected_append_has_no_side_effect() {
  Store store(":memory:");
  bool rejected = false;
  try {
    store.append(Stream::Reality, event(""));
  } catch (const std::invalid_argument&) {
    rejected = true;
  }
  expect(rejected);
  expect(store.events(Stream::Reality).empty());
}

void file_backed_events_survive_reopen() {
  const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
  const auto path = std::filesystem::temp_directory_path() /
                    ("campistoria-spike-" + std::to_string(stamp) + ".sqlite");
  {
    Store store(path.string());
    store.append(Stream::Reality, event("created"));
  }
  {
    Store reopened(path.string());
    expect(reopened.events(Stream::Reality) == std::vector<Event>({event("created")}));
  }
  std::filesystem::remove(path);
}

void imports_shared_snapshot_fixture() {
  std::ifstream file(CAMPISTORIA_SNAPSHOT_FIXTURE);
  expect(file.good());
  const std::string input(std::istreambuf_iterator<char>{file}, {});
  Store store(":memory:");
  store.import_json(input);
  expect(store.events(Stream::Reality) ==
         std::vector<Event>({event("created"), event("resolved")}));
  expect(store.events(Stream::Knowledge) ==
         std::vector<Event>({event("observed")}));
}

}  // namespace

int main() {
  export_import_preserves_separate_ordered_streams();
  invalid_import_rolls_back_earlier_inserts();
  rejected_append_has_no_side_effect();
  file_backed_events_survive_reopen();
  imports_shared_snapshot_fixture();
}
