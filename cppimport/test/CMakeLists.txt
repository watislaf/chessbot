# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

#CMAKE_CURRENT_LIST_DIR - get current cmake dir
add_executable(
        ${MY_TESTS_TO_AI}
        ${CMAKE_CURRENT_LIST_DIR}/test.cpp
)

target_link_libraries(
        ${MY_TESTS_TO_AI} gtest_main ${MY_AI_LIB}
)


include(GoogleTest)
gtest_discover_tests(${MY_TESTS_TO_AI})