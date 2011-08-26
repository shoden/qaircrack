cmake_minimum_required(VERSION 2.6)

project(qaircrack)

find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

add_subdirectory(src)

set(DESKTOP_ENTRY qaircrack.desktop)
set(APP_ICON src/pixmaps/qaircrack.png)

install(FILES ${CMAKE_SOURCE_DIR}/${DESKTOP_ENTRY} DESTINATION share/applications)
install(FILES ${CMAKE_SOURCE_DIR}/${APP_ICON} DESTINATION share/icons)
