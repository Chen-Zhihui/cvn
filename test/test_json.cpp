
#include "test.h"

SCENARIO("json", "[all][operators]") {
	GIVEN("a source") {
		
		int i = 0;

		WHEN("a predicate function is passed to the all operator") {

			i ++

			THEN("the output only contains true") {
				i--;
			}

			THEN("there was 1 subscription/unsubscription to the source") {
				i++
			}

		}
	}
}