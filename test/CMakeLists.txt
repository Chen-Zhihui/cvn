

set(testcase 
    test_main.cpp
    test_catch.cpp
	test_expected.cpp 
	test_json.cpp
	test_fmt.cpp
	#test_sqlite_orm.cpp
	)

add_executable(catch_test  ${testcase})
target_link_libraries(catch_test fmt::fmt)
target_link_libraries(catch_test Catch2::Catch2)
target_link_libraries(catch_test sqlite_orm)
if(MSVC)
target_compile_options(catch_test 
PRIVATE "/bigobj"
)
endif()
include(Catch)
catch_discover_tests(catch_test)