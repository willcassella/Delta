# Delta - CMakeLists.txt
cmake_minimum_required(VERSION 3.6)
find_package(PkgConfig)

set(CMAKE_CXX_STANDARD_REQUIRED 11)
project(delta)

set(CORE_SOURCE
    source/dt_common.c
    source/dt_layout.c
    source/dt_template.c
)

set(EXTRA_SOURCE
    source/serializer.cpp
    source/main.cpp
)

add_executable(delta ${CORE_SOURCE} ${EXTRA_SOURCE})

pkg_check_modules(SFML REQUIRED sfml-system sfml-window sfml-graphics)
target_link_libraries(delta ${SFML_LIBRARIES} pugixml)
target_include_directories(delta PUBLIC ${SFML_INCLUDE_DIRS})
