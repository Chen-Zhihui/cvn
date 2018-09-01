
#include <catch2/catch.hpp>
#include <iostream>
#include <tl/expected.hpp>
#include <string>

using ResultT = tl::expected<int, std::string>;

int copy(int v) {
	return v;
}

int add(int v) {
	return v + v;
}

int sqare(int v) {
	return v * v;
}

ResultT chain(int v) {
	return ResultT(v).map(copy).map(add).map(sqare);
}

SCENARIO("expected", "[all][operators]") {
	GIVEN("a source") {

		WHEN("a predicate function is passed to the all operator") {


			THEN("create from empty") {
				chain(2);
			}
		}
	}
}