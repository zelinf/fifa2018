project(fifa2018-lib)

# Library
set(PUBLIC_HEADERS
        include/fifa2018/Team.h
        include/fifa2018/RandomGen.h
        include/fifa2018/Player.h
        include/fifa2018/Match.h
        include/fifa2018/Tournament.h
        include/fifa2018/Stage.h)
add_library(fifa2018
        ${PUBLIC_HEADERS}
        src/Team.cpp
        src/RandomGen.cpp
        src/Player.cpp
        src/Match.cpp
        src/Tournament.cpp
        src/Stage.cpp)
target_include_directories(fifa2018 PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE src)
target_link_libraries(fifa2018
        fmt
        json)

# Test Suite
add_executable(fifa2018-test
        test/TestMain.cpp
        test/RandomGenTest.cpp
        test/TeamPlayerTest.cpp)
target_link_libraries(fifa2018-test
        fifa2018
        catch)
add_test(fifa2018-test fifa2018-test)
