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

#Add execute
add_executable(
        ${TESTS_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/tests.cpp
)

#Add library
target_link_libraries(
        ${TESTS_NAME}
        gtest_main
        ${AI_LIB_NAME}
)


include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})