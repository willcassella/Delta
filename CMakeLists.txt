# Delta - CMakeLists.txt
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

project(delta)

set(CORE_SOURCE
    source/common.c
    source/layout.c
    source/template.c
    source/model.c
)

set(EXTRA_SOURCE
    source/serializer.cpp
    source/main.c
)

add_executable(main ${CORE_SOURCE} ${EXTRA_SOURCE})

if(UNIX)
    find_package(PkgConfig)
    pkg_check_modules(SFML REQUIRED sfml-system sfml-window sfml-graphics)
elseif(WIN32)
    find_package(pugixml REQUIRED)
	target_include_directories(main PRIVATE pugixml)
	target_link_libraries(main PRIVATE pugixml)

	find_package(glfw3 REQUIRED)
	target_link_libraries(main PRIVATE glfw)
	target_link_libraries(main PRIVATE OpenGL32)
endif()

# Configure CMake to copy test file to build directory
configure_file(test.xml test.xml COPYONLY)
