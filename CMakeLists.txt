project(LibTui)

cmake_minimum_required(VERSION 2.6)
# change to 3.* in the future
# then easily create export headers for cmake

set(tui_VERSION_MAJOR 0)
set(tui_VERSION_MINOR 0)
set(tui_VERSION_PATCH 1)
set(tui_VERSION "0.0.1")

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)
find_package(Threads)

set(CONF_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/include")

configure_file(cmake/LibTuiConfig.cmake.in
  ${PROJECT_BINARY_DIR}/LibTuiConfig.cmake @ONLY)
configure_file(cmake/LibTuiConfigVersion.cmake.in
  ${PROJECT_BINARY_DIR}/LibTuiConfigVersion.cmake @ONLY)


set(SRCS
  src/tui.cc

  src/button.cc
  src/checkbox.cc
  src/label.cc
  src/progressbar.cc
  src/textbox.cc
  src/window.cc

  src/positioning.cc

  src/text.cc
)

set(INCS
  src/tui.hh

  src/widget.hh

  src/button.hh
  src/checkbox.hh
  src/label.hh
  src/progressbar.hh
  src/textbox.hh
  src/window.hh

  src/point.hh
  src/size.hh
  src/positioning.hh

  src/text.hh
)

foreach(INC IN ITEMS ${INCS})
  STRING(REGEX REPLACE "^src/" "" FILENAME ${INC})
  configure_file(${INC} ${PROJECT_BINARY_DIR}/include/tui/${FILENAME} COPYONLY)
endforeach(INC)


set(INSTALL_CMAKE_DIR lib/libtui-${LibTui_VERSION}/cmake CACHE PATH
  "Install path for CMake files")

# handle it in other way when we switch to cmake 3.*
add_definitions(-std=c++11 -g)

include_directories(${CURSES_INCLUDE_DIRS})

add_library(tui SHARED ${SRCS})
target_link_libraries(tui ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(tui PROPERTIES
  #PUBLIC_HEADER ${INCS}
  VERSION ${tui_VERSION}
  SOVERSION "0.1")

install(TARGETS tui
  EXPORT LibTuiExports
  LIBRARY       DESTINATION lib         COMPONENT bin
  PUBLIC_HEADER DESTINATION include/tui COMPONENT dev
)

install(FILES
  ${PROJECT_BINARY_DIR}/LibTuiConfig.cmake
  ${PROJECT_BINARY_DIR}/LibTuiConfigVersion.cmake
  DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT dev
)

# export targets to build and install tree
export(TARGETS tui
  FILE ${PROJECT_BINARY_DIR}/LibTuiExports.cmake)
export(PACKAGE LibTui)
install(EXPORT LibTuiExports
  DESTINATION ${INSTALL_CMAKE_DIR}
  COMPONENT dev
)
