cmake_minimum_required(VERSION 3.16)
project(bpmcalq)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-base-1.0 gstreamer-app-1.0) # Added GStreamer

include_directories(${GTKMM_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS} ${GSTREAMER_INCLUDE_DIRS}) # Added GSTREAMER_INCLUDE_DIRS
link_directories(${GTKMM_LIBRARY_DIRS} ${SNDFILE_LIBRARY_DIRS} ${GSTREAMER_LIBRARY_DIRS})   # Added GSTREAMER_LIBRARY_DIRS
add_definitions(${GTKMM_CFLAGS_OTHER} ${SNDFILE_CFLAGS_OTHER} ${GSTREAMER_CFLAGS_OTHER})    # Added GSTREAMER_CFLAGS_OTHER

add_executable(bpmcalq main.cpp)

target_link_libraries(bpmcalq ${GTKMM_LIBRARIES} ${SNDFILE_LIBRARIES} ${GSTREAMER_LIBRARIES}) # Added GSTREAMER_LIBRARIES

install(TARGETS bpmcalq DESTINATION bin)