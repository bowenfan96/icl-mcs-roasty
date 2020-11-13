#include "../Source/Model/RoastyModel.hpp"
#include "../Source/Serialisation.hpp"
#include <catch2/catch.hpp>

using json = nlohmann::json;

TEST_CASE("Serialisation") {
  SECTION("Can serialise bean blend") {
    Ingredient b{*(new Bean{"Java"}), 500};
    auto j = ingredientToJson(b);

    REQUIRE(j["amount"].get<int>() == 500);
    REQUIRE(j["name"].get<std::string>() == "Java");
  }

  SECTION("Can de-serialise bean blend") {
    auto const* j = R"({
      "amount": 500,
      "name": "Java"
    })";

    auto parsed = json::parse(j);
    auto* b = jsonToIngredient(parsed);

    REQUIRE(b->getAmount() == 500);
    REQUIRE(b->getBean().getName() == "Java");
    delete b;
  }

  SECTION("Can serialise event") {
    auto* value = new EventValue(500);
    Event e{"measurement", 100, value};
    auto j = eventToJson(e);

    REQUIRE(j["timestamp"].get<int>() == 100);
    REQUIRE(j["type"].get<std::string>() == "measurement");
    REQUIRE(j["value"].get<int>() == 500);
  }

  SECTION("Can de-serialise event") {
    auto const* j = R"({
      "timestamp": 1234,
      "type": "measurement",
      "value": 50
    })";

    auto parsed = json::parse(j);
    auto* b = jsonToEvent(parsed);

    REQUIRE(b->getTimestamp() == 1234);
    REQUIRE(b->getType() == "measurement");
    REQUIRE(b->getValue()->getValue() == 50);
    delete b;
  }

  SECTION("Can serialise roast") {
    Roast r{1, 100};
    r.addEvent(*(new Event{"measurement", 4}));
    auto* javaBean = new Bean{"Java"};
    r.addIngredient(*(new Ingredient{*javaBean, 600}));
    auto roast = roastToJson(r);

    REQUIRE(roast["id"].get<int>() == 1);
    REQUIRE(roast["beginTimestamp"].get<int>() == 100);
    REQUIRE(roast["events"].at(0)["timestamp"].get<int>() == 4);
    REQUIRE(roast["beans"].at(0)["name"].get<std::string>() == "Java");
  }

  SECTION("Can de-serialise roast") {
    auto const* j = R"({
      "id": 3,
      "beginTimestamp": 2352351221,
      "beans": [
        {
          "name": "java",
          "amount": 600
        }
      ],
      "events": [
        {
          "type": "measurement",
          "value": 750,
          "timestamp": 12512523
        }
      ]
    })";

    auto parsed = json::parse(j);
    auto r = jsonToRoast(parsed);

    REQUIRE(r.getId() == 3);
    REQUIRE(r.getTimestamp() == 2352351221);
    REQUIRE(r.getEvent(0).getTimestamp() == 12512523);
    REQUIRE(r.getIngredient(0).getAmount() == 600);
  }
}
