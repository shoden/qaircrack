set(SOURCES main.cpp qaircrack.cpp)
set(HEADERS qaircrack.h)
set(UI_FILES qaircrack.ui)
set(RCS graphics.qrc)

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_FILES})
qt4_add_resources(RC_SOURCES ${RCS})

add_subdirectory(pixmaps)
add_subdirectory(scripts)

include_directories(${CMAKE_BINARY_DIR}/src)

add_executable(qaircrack ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS} ${RC_SOURCES})
target_link_libraries(qaircrack ${QT_LIBRARIES})

install(TARGETS qaircrack DESTINATION bin)
