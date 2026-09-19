cmake_minimum_required(VERSION 3.22)
project(campistoria_storage_prototype LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
  URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)
set(CAMPISTORIA_SQLITE_AMALGAMATION "" CACHE PATH "SQLite amalgamation directory for controlled comparison")
if(CAMPISTORIA_SQLITE_AMALGAMATION)
  add_library(campistoria_comparison_sqlite STATIC "${CAMPISTORIA_SQLITE_AMALGAMATION}/sqlite3.c")
  target_include_directories(campistoria_comparison_sqlite PUBLIC "${CAMPISTORIA_SQLITE_AMALGAMATION}")
  target_compile_definitions(campistoria_comparison_sqlite PRIVATE
    SQLITE_CORE SQLITE_DEFAULT_FOREIGN_KEYS=1 SQLITE_ENABLE_API_ARMOR
    SQLITE_ENABLE_COLUMN_METADATA SQLITE_ENABLE_DBSTAT_VTAB SQLITE_ENABLE_FTS3
    SQLITE_ENABLE_FTS3_PARENTHESIS SQLITE_ENABLE_FTS5 SQLITE_ENABLE_JSON1
    SQLITE_ENABLE_LOAD_EXTENSION=1 SQLITE_ENABLE_MEMORY_MANAGEMENT
    SQLITE_ENABLE_RTREE SQLITE_ENABLE_STAT4 SQLITE_SOUNDEX SQLITE_THREADSAFE=1
    SQLITE_USE_URI HAVE_USLEEP=1 HAVE_ISNAN _POSIX_THREAD_SAFE_FUNCTIONS)
  set(CAMPISTORIA_SQLITE_TARGET campistoria_comparison_sqlite)
else()
  find_package(SQLite3 REQUIRED)
  if(TARGET SQLite3::SQLite3)
    set(CAMPISTORIA_SQLITE_TARGET SQLite3::SQLite3)
  else()
    set(CAMPISTORIA_SQLITE_TARGET SQLite::SQLite3)
  endif()
endif()

add_library(campistoria_storage_prototype src/store.cpp)
set_target_properties(campistoria_storage_prototype PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(campistoria_storage_prototype PUBLIC cxx_std_20)
target_include_directories(campistoria_storage_prototype PUBLIC include)
target_link_libraries(campistoria_storage_prototype PUBLIC ${CAMPISTORIA_SQLITE_TARGET} nlohmann_json::nlohmann_json)

add_executable(storage_tests tests/store_tests.cpp)
target_link_libraries(storage_tests PRIVATE campistoria_storage_prototype)
target_compile_definitions(storage_tests PRIVATE CAMPISTORIA_SNAPSHOT_FIXTURE="${CMAKE_CURRENT_SOURCE_DIR}/../fixtures/snapshot-v1.json")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(campistoria_storage_prototype PRIVATE -Wall -Wextra -Werror)
  target_compile_options(storage_tests PRIVATE -Wall -Wextra -Werror)
endif()

enable_testing()
add_test(NAME storage_tests COMMAND storage_tests)

add_executable(storage_benchmark bench/benchmark.cpp)
target_link_libraries(storage_benchmark PRIVATE campistoria_storage_prototype)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(storage_benchmark PRIVATE -Wall -Wextra -Werror)
endif()

add_library(campistoria_store_ffi SHARED src/ffi.cpp)
target_link_libraries(campistoria_store_ffi PRIVATE campistoria_storage_prototype)
target_compile_features(campistoria_store_ffi PRIVATE cxx_std_20)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(campistoria_store_ffi PRIVATE -Wall -Wextra -Werror)
endif()
