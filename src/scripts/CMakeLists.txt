include_directories(${CMAKE_BINARY_DIR}/src/scripts)

set(SCRIPTS
      qaircrack_mac
      qaircrack_stopmonitors
      qaircrack_terminal
      qaircrack_wlans
)

install(FILES ${SCRIPTS} DESTINATION bin)
