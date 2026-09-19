// Local persistence prototype for HLA-PERSIST, HLA-CORE, and HLA-OBSERVER.
// Exercises FR-031, FR-032, FR-041, NFR-003, and NFR-006 storage concerns.

#pragma once

#include <nlohmann/json.hpp>
#include <sqlite3.h>

#include <string>
#include <vector>

namespace campistoria_prototype {

enum class Stream { Reality, Knowledge };

struct Event {
  std::string campaign_id;
  std::string kind;
  nlohmann::json payload;

  bool operator==(const Event&) const = default;
};

void to_json(nlohmann::json& output, const Event& event);
void from_json(const nlohmann::json& input, Event& event);

class Store {
 public:
  explicit Store(const std::string& path);
  ~Store();

  Store(const Store&) = delete;
  Store& operator=(const Store&) = delete;

  void append(Stream stream, const Event& event);
  std::vector<Event> events(Stream stream) const;
  std::string export_json() const;
  void import_json(const std::string& input);

 private:
  sqlite3* connection_ = nullptr;

  void execute(const char* sql) const;
  void insert(const char* table, const Event& event);
};

}  // namespace campistoria_prototype
