
#include <catch.hpp>
#include <iostream>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/printf.h>

using namespace fmt::literals;

SCENARIO("fmt", "[all][operators]") {
	GIVEN("a source") {


		WHEN("a predicate function is passed to the all operator") {


			THEN("create from empty") {
				fmt::format("The answer is {}.", 42);

				fmt::memory_buffer out;
				fmt::format_to(out, "For a moment, {} happened.", "nothing");
				out.data(); // returns a pointer to the formatted data

				fmt::print("Don't {}\n", "panic");

				fmt::print("I'd rather be {1} than {0}.", "right", "happy");

				fmt::print("Hello, {name}! The answer is {number}. Goodbye, {name}.",
					fmt::arg("name", "World"), fmt::arg("number", 42));

				fmt::print("Hello, {name}! The answer is {number}. Goodbye, {name}.",
					"name"_a = "World", "number"_a = 42);
			}

			THEN("format suffix") {
				std::string message = "{0}{1}{0}"_format("abra", "cad");
			}

			THEN("format inf") {
				fmt::print("{}", std::numeric_limits<double>::infinity());
			}
		}
	}
}