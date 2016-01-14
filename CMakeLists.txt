project(LibTui)

cmake_minimum_required(VERSION 2.6)
# change to 3.* in the future
# then easily create export headers for cmake

set(tui_VERSION_MAJOR 0)
set(tui_VERSION_MINOR 0)
set(tui_VERSION_PATCH 1)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)

set(SRCS
  src/tui.cc
  src/window.cc
)

set(INCS
  src/tui.hh
  src/window.hh
)

# handle it in other way when we switch to cmake 3.*
add_definitions(-std=c++11)

include_directories(${CURSES_INCLUDE_DIRS})

add_library(tui SHARED ${SRCS})
target_link_libraries(tui ${CURSES_LIBRARIES})

install(TARGETS tui DESTINATION lib)
install(FILES ${INCS} DESTINATION include/tui)
