
#include <catch.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SCENARIO("json", "[all][operators]") {
	GIVEN("a source") {
		

		WHEN("a predicate function is passed to the all operator") {


			THEN("create from empty") {
				// create an empty structure (null)
				json j;

				// add a number that is stored as double (note the implicit conversion of j to an object)
				j["pi"] = 3.141;

				// add a Boolean that is stored as bool
				j["happy"] = true;

				// add a string that is stored as std::string
				j["name"] = "Niels";

				// add another null object by passing nullptr
				j["nothing"] = nullptr;

				// add an object inside the object
				j["answer"]["everything"] = 42;

				// add an array that is stored as std::vector (using an initializer list)
				j["list"] = { 1, 0, 2 };

				// add another object (using an initializer list of pairs)
				j["object"] = { {"currency", "USD"}, {"value", 42.99} };

				// instead, you could also write (which looks very similar to the JSON above)
				json j2 = {
				  {"pi", 3.141},
				  {"happy", true},
				  {"name", "Niels"},
				  {"nothing", nullptr},
				  {"answer", {
					{"everything", 42}
				  }},
				  {"list", {1, 0, 2}},
				  {"object", {
					{"currency", "USD"},
					{"value", 42.99}
				  }}
				};
				//std::cout << j2.dump(4) << std::endl;

			}

			THEN("create from string") {
				// or even nicer with a raw string literal
				auto j2 = R"(
					  {
						"happy": true,
						"pi": 3.141
					  }
					)"_json;
				//std::cout << j2.dump(4) << std::endl;
			}

		}
	}
}