

set(testcase 
    test_main.cpp
    #test_catch.cpp
	#test_cereal.cpp 
	#test_json.cpp
	)

add_executable(catch_test  ${testcase})
target_link_libraries(catch_test ${CMAKE_THREAD_LIBS_INIT})