cmake_minimum_required(VERSION 3.16)
project(bpmcalq)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(SNDFILE REQUIRED sndfile)

include_directories(${GTKMM_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS} ${SNDFILE_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER} ${SNDFILE_CFLAGS_OTHER})

add_executable(bpmcalq main.cpp)

target_link_libraries(bpmcalq ${GTKMM_LIBRARIES} ${SNDFILE_LIBRARIES})

install(TARGETS bpmcalq DESTINATION bin)