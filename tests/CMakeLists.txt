include(ExternalProject)
include(GoogleTest)

ExternalProject_Add(
  gtest
  URL https://github.com/google/googletest/archive/master.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  INSTALL_COMMAND "")

ExternalProject_Get_Property(gtest source_dir binary_dir)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)
set_target_properties(libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)
set_target_properties(libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/lib/libgmock.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

include_directories("${source_dir}/googletest/include" "${source_dir}/googlemock/include")

# testGroup.1
add_executable(testGroup.1 test1.cpp main.cpp)
target_link_libraries(testGroup.1 libgtest libgmock rtMessage)

# testGroup.2
#add_executable(testGroup.2 test1.cpp main.cpp)
#target_link_libraries(testGroup.2 libgtest libgmock rtMessage)

# not sure why this isn't working
gtest_discover_tests(testGroup.1)
