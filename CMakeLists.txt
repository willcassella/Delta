# Delta - CMakeLists.txt
cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD_REQUIRED 11)
find_package(PkgConfig)

project(delta)

set(CORE_SOURCE
        source/common.c
        source/layout.c
        source/template.c
        source/model.c
)

set(EXTRA_SOURCE
    source/serializer.cpp
    source/main.cpp
)

add_executable(delta ${CORE_SOURCE} ${EXTRA_SOURCE})

pkg_check_modules(SFML REQUIRED sfml-system sfml-window sfml-graphics)
target_link_libraries(delta ${SFML_LIBRARIES} pugixml)
target_include_directories(delta PUBLIC ${SFML_INCLUDE_DIRS})
