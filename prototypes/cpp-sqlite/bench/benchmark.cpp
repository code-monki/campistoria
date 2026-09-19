// Repeatable storage-slice workload for DEC-002; not an engine NFR test.
#include "store.hpp"

#include <sqlite3.h>

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

using campistoria_prototype::Event;
using campistoria_prototype::Store;
using campistoria_prototype::Stream;

namespace {
constexpr int event_count = 300;
constexpr int repetitions = 5;

Event event(int index) {
  return {"campaign-1", "event", {{"index", index}, {"text", "representative payload"}}};
}

template <typename Action>
void measure(const char* name, int run, int operations, Action action) {
  const auto start = std::chrono::steady_clock::now();
  action();
  const auto end = std::chrono::steady_clock::now();
  const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  std::cout << name << ',' << run << ',' << operations << ',' << ns << '\n';
}
}  // namespace

int main() {
  std::cerr << "sqlite_version=" << sqlite3_libversion() << '\n';
  std::cout << "workload,run,operations,elapsed_ns\n";
  for (int run = 0; run <= repetitions; ++run) {
    Store store(":memory:");
    measure("append", run, event_count, [&] {
      for (int index = 0; index < event_count; ++index) store.append(Stream::Reality, event(index));
    });
    measure("read_300", run, 20, [&] {
      for (int index = 0; index < 20; ++index) {
        if (store.events(Stream::Reality).size() != event_count) throw std::runtime_error("read count");
      }
    });
    measure("export_300", run, 10, [&] {
      for (int index = 0; index < 10; ++index) {
        if (store.export_json().empty()) throw std::runtime_error("empty export");
      }
    });
    const auto snapshot = store.export_json();
    measure("import_300", run, 5, [&] {
      for (int index = 0; index < 5; ++index) {
        Store destination(":memory:");
        destination.import_json(snapshot);
        if (destination.events(Stream::Reality).size() != event_count) throw std::runtime_error("import count");
      }
    });
    const auto path = (std::filesystem::temp_directory_path() /
                       ("campistoria-cpp-benchmark-" +
                        std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
                        "-" + std::to_string(run) + ".sqlite")).string();
    {
      Store file_store(path);
      measure("file_append", run, 100, [&] {
        for (int index = 0; index < 100; ++index) file_store.append(Stream::Reality, event(index));
      });
    }
    std::remove(path.c_str());
  }
}
