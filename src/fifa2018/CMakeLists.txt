project(fifa2018-lib)

# Library
set(PUBLIC_HEADERS
        include/fifa2018/Team.h)
add_library(fifa2018
        ${PUBLIC_HEADERS}
        src/Team.cpp)
target_include_directories(fifa2018 PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE src)

# Test Suite
add_executable(fifa2018-test
        test/TestMain.cpp)
target_link_libraries(fifa2018-test
        catch)
add_test(fifa2018-test fifa2018-test)
