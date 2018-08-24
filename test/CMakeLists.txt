

set(testcase 
    test_main.cpp
    test_catch.cpp
	test_expected.cpp 
	test_json.cpp
	test_fmt.cpp
	)

add_executable(catch_test  ${testcase})
target_link_libraries(catch_test fmt::fmt)