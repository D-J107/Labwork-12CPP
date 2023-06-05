include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        My_cool_DB_tests
        Insert_test.cpp
        Join_test.cpp
        Update_test.cpp
        Reference_and_Delete_test.cpp
        Drop_test.cpp
        main.cpp)

target_link_libraries(
        My_cool_DB_tests
        MyCoolDB
        GTest::gtest_main
)

target_include_directories(My_cool_DB_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(My_cool_DB_tests)