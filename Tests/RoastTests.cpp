#include "../Source/Model/RoastyModel.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Ingredient Count is Correct") {
  auto id = 1;
  auto timestamp = 12345;
  Roast r{id, timestamp};
  for(auto i = 0; i < 20; i++) {
    auto amount = i * 10;
    r.addIngredient(*(new Ingredient{*(new Bean{"b"}), amount}));
  }

  REQUIRE(r.getIngredientsCount() == 20);
  REQUIRE(r.getIngredient(5).getAmount() == 50);
}

TEST_CASE("Event Count is Correct") {
  auto id = 1;
  auto timestamp = 12345;

  Roast r{id, timestamp};

  for(auto i = 0; i < 20; i++) { // NOLINT
    auto timestamp = 20 - i;     // NOLINT
    auto const* eventTypeName = "measurement";
    r.addEvent(*(new Event{eventTypeName, timestamp}));
  }

  REQUIRE(r.getEventCount() == 20);
  REQUIRE(r.getEvent(5).getTimestamp() == 15);
}
