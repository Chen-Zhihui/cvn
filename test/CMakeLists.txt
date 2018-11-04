

set(testcase 
    test_main.cpp
    test_catch.cpp
	test_expected.cpp 
	test_json.cpp
	test_fmt.cpp
	#test_sqlite_orm.cpp
	)

add_executable(test_3rdparty  ${testcase})
target_link_libraries(test_3rdparty fmt::fmt)
target_link_libraries(test_3rdparty Catch2::Catch2)
target_link_libraries(test_3rdparty sqlite_orm)
if(MSVC)
target_compile_options(test_3rdparty
PRIVATE "/bigobj"
)
endif()
include(Catch)
catch_discover_tests(test_3rdparty)