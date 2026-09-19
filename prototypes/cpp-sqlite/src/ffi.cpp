// Experimental C ABI adapter for the HLA-PERSIST storage slice.
#include "../../ffi/store_abi.h"

#include "store.hpp"

#include <nlohmann/json.hpp>

#include <exception>
#include <string>

using campistoria_prototype::Event;
using campistoria_prototype::Store;
using campistoria_prototype::Stream;

namespace {
Stream checked_stream(int stream) {
  if (stream == 0) return Stream::Reality;
  if (stream == 1) return Stream::Knowledge;
  throw std::invalid_argument("invalid stream");
}
}  // namespace

extern "C" void* campistoria_store_new(void) {
  try {
    return new Store(":memory:");
  } catch (...) {
    return nullptr;
  }
}

extern "C" void campistoria_store_free(void* handle) {
  delete static_cast<Store*>(handle);
}

extern "C" int campistoria_store_append(void* handle, int stream, const char* event_json) {
  if (handle == nullptr || event_json == nullptr) return -1;
  try {
    const Event event = nlohmann::json::parse(event_json).get<Event>();
    static_cast<Store*>(handle)->append(checked_stream(stream), event);
    return 0;
  } catch (...) {
    return -1;
  }
}

extern "C" int campistoria_store_count(void* handle, int stream) {
  if (handle == nullptr) return -1;
  try {
    return static_cast<int>(static_cast<Store*>(handle)->events(checked_stream(stream)).size());
  } catch (...) {
    return -1;
  }
}
