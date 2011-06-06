cmake_minimum_required(VERSION 2.6)

project(qaircrack)

find_package(Qt4 REQUIRED)
include( ${QT_USE_FILE} )

set(SOURCES main.cpp qaircrack.cpp)
set(HEADERS qaircrack.h)
set(UI_FILES qaircrack.ui)
set(RCS graphics.qrc)
qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})
qt4_add_resources(RC_SOURCES ${RCS})

add_executable(qaircrack ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS} ${RC_SOURCES})
target_link_libraries(qaircrack ${QT_LIBRARIES})

install(TARGETS qaircrack DESTINATION bin)

set(DESKTOP_ENTRY qaircrack.desktop)
set(APP_ICON icons/qaircrack.png)

install(FILES ${CMAKE_SOURCE_DIR}/${APP_ICON} DESTINATION share/icons)
install(FILES ${CMAKE_SOURCE_DIR}/${DESKTOP_ENTRY} DESTINATION share/applications)
