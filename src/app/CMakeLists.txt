add_executable(fifa2018-app
        src/Program.cpp)
target_link_libraries(fifa2018-app
        fifa2018)

install(TARGETS fifa2018-app DESTINATION fifa2018)
install(FILES ${PROJECT_SOURCE_DIR}/resources/config.cfg
        DESTINATION fifa2018)