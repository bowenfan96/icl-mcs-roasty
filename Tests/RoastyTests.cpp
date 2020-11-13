#include "../Source/Roasty.hpp"
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
using namespace std;

#include "../Source/Server/RoastyServerException.hpp"
#include "../Source/Storage/MemoryStorage.hpp"
#include "../Source/Utilities.hpp"

TEST_CASE("Bean can be CRUD") {
  MemoryStorage storage;
  Roasty<MemoryStorage> roasty{&storage};

  REQUIRE(storage.getBeans().empty());

  SECTION("Adding bean works") {
    roasty.addBean((Bean{"Best Bean"}));
    auto it = std::find_if(storage.beans.begin(), storage.beans.end(),
                           [&](const auto& bean) { return "Best Bean" == bean.getName(); });

    REQUIRE(it != storage.beans.end());
    REQUIRE(it->getName() == "Best Bean");
    storage.removeBean(0);
  }

  SECTION("Removing bean works") {
    Bean b{"Other bean"};
    storage.beans.push_back(b);
    roasty.deleteBean(b);

    auto it = std::find_if(storage.beans.begin(), storage.beans.end(),
                           [&](const auto& bean) { return b.getName() == bean.getName(); });

    REQUIRE(it == storage.beans.end());
  }
}

TEST_CASE("Roasts can be CRUD") {
  MemoryStorage storage;
  Roasty<MemoryStorage> roasty{&storage};

  SECTION("Adding a roast works") {
    Roast r{0, 1000};
    roasty.addRoast(r);

    auto it = std::find_if(storage.roasts.begin(), storage.roasts.end(),
                           [&](const auto& roast) { return 0 == roast.getId(); });

    REQUIRE(it != storage.roasts.end());
    REQUIRE(it->getId() == 0);
  }

  SECTION("Deleting a roast works") {
    Roast r{1, 50};
    storage.roasts.push_back(r);

    roasty.deleteRoast(r.getId());

    auto it = std::find_if(storage.roasts.begin(), storage.roasts.end(),
                           [&](const auto& roast) { return r.getId() == roast.getId(); });

    REQUIRE(it == storage.roasts.end());
  }

  SECTION("Adding an event to a roast works") {
    Roast r{12, 50};
    storage.roasts.push_back(r);

    auto& eNoValue = *(new Event{"measurement", 12345});
    auto* v = new EventValue{200};
    auto& eWithValue = *(new Event{"measurement", 123456, v});

    roasty.addEventToRoast(12, eNoValue);
    roasty.addEventToRoast(12, eWithValue);

    auto roast = std::find_if(storage.roasts.begin(), storage.roasts.end(),
                              [](const auto& roast) { return 12 == roast.getId(); });
    auto matches = 0;
    for(auto i = 0u; i < roast->getEventCount(); i++) {
      if(roast->getEvent(i).getTimestamp() == 123456) {
        REQUIRE(roast->getEvent(i).hasValue());
        REQUIRE(roast->getEvent(i).getValue()->getValue() == 200);
        matches++;
      }
      if(roast->getEvent(i).getTimestamp() == 12345)
        matches++;
    }
    REQUIRE(matches == 2);
  }

  SECTION("Getting all roasts works") {
    Roast r1{15, 50};
    Roast r2{16, 60};
    storage.roasts.push_back(r1);
    storage.roasts.push_back(r2);

    auto allRoasts = roasty.allRoasts();

    auto it = std::find_if(allRoasts.begin(), allRoasts.end(),
                           [&](const auto& roast) { return 15 == roast.getId(); });

    REQUIRE(it != allRoasts.end());

    it = std::find_if(allRoasts.begin(), allRoasts.end(),
                      [&](const auto& roast) { return 15 == roast.getId(); });

    REQUIRE(it != allRoasts.end());
  }

  SECTION("Getting a roast works") {
    Roast r{55, 65};
    storage.roasts.push_back(r);

    auto& foundRoast = roasty.getRoast(55);

    REQUIRE(foundRoast.getId() == 55);
  }

  SECTION("Deleting an event from a roast works") {
    Roast r{1237, 5678};
    auto& e = *(new Event{"measurement", 123459});
    r.addEvent(e);
    REQUIRE(r.getEventCount() > 0);
    storage.roasts.push_back(r);

    roasty.removeEventFromRoast(1237, 123459);

    auto& newRoast = roasty.getRoast(1237);

    REQUIRE(newRoast.getEventCount() == 0);
  }

  SECTION("Replacing a roast works") {
    Roast oldR{88, 1};
    Roast newR{99, 2};

    storage.roasts.push_back(oldR);

    roasty.replaceRoast(88, newR);

    auto itOld = std::find_if(storage.roasts.begin(), storage.roasts.end(),
                              [](const auto& roast) { return roast.getId() == 88; });

    auto itNew = std::find_if(storage.roasts.begin(), storage.roasts.end(),
                              [](const auto& roast) { return roast.getId() == 99; });

    REQUIRE(itOld == storage.roasts.end());
    REQUIRE(itNew != storage.roasts.end());
  }

  SECTION("Replacing an event in a roast works") {
    Roast r{1235, 5124};
    auto& oldE = *(new Event{"measurement", 123459});
    auto& newE = *(new Event{"measurement", 100000});

    r.addEvent(oldE);
    storage.roasts.push_back(r);

    roasty.replaceEventInRoast(1235, 123459, newE);

    auto newRoast = roasty.getRoast(1235);
    auto events = RangeGenerator<const Event>(
        [&newRoast](auto i) -> Event const& { return newRoast.getEvent(i); },
        newRoast.getEventCount());

    auto it = std::find_if(events.begin(), events.end(),
                           [&](const auto& event) { return event.getTimestamp() == 100000; });

    REQUIRE(it != events.end());
    REQUIRE(it->getTimestamp() == 100000);
    storage.roasts.clear();
  }

  SECTION("Adding a blend to a roast works") {
    Roast r{1111, 5124};
    auto& javaBean = *(new Bean{"Java"});
    auto& b = *(new Ingredient{javaBean, 400});
    storage.roasts.push_back(r);

    roasty.addIngredientToRoast(1111, b);

    auto& newRoast = roasty.getRoast(1111);
    auto bean = RangeGenerator<const Ingredient>(
        [&newRoast](auto i) -> Ingredient const& { return newRoast.getIngredient(i); },
        newRoast.getIngredientsCount());

    auto it = std::find_if(bean.begin(), bean.end(),
                           [](const auto& blend) { return blend.getBean().getName() == "Java"; });

    REQUIRE(newRoast.getIngredientsCount() == 1);
    REQUIRE(it != bean.end());
    REQUIRE(it->getAmount() == 400);
  }

  SECTION("Deleting a blend from a roast works") {
    Roast r{1112, 5123};
    auto& b = *(new Ingredient{*(new Bean{"Java"}), 500});
    r.addIngredient(b);

    storage.roasts.push_back(r);

    roasty.removeIngredientFromRoast(1112, "Java");

    auto& newRoast = roasty.getRoast(1112);
    auto bean = RangeGenerator<Ingredient const* const>(
        [&newRoast](auto i) -> Ingredient const* const { return &newRoast.getIngredient(i); },
        newRoast.getIngredientsCount());

    auto it = std::find_if(bean.begin(), bean.end(),
                           [](const auto& blend) { return blend->getBean().getName() == "Java"; });

    REQUIRE(it == bean.end());
  }

  SECTION("Updating a blend from a roast works") {
    Roast r{1113, 5123};
    auto* javaBean = new Bean{"Java"};
    auto* b = new Ingredient{*javaBean, 300};
    r.addIngredient(*b);

    storage.roasts.push_back(r);

    roasty.updateIngredient(1113, "Java", 500);

    auto& newRoast = roasty.getRoast(1113);

    REQUIRE(newRoast.getIngredientsCount() == 1);
    REQUIRE(newRoast.getIngredient(0).getAmount() == 500);
  }
}

TEST_CASE("Error handling") {
  MemoryStorage storage;
  Roasty<MemoryStorage> roasty{&storage};

  SECTION("Cannot add duplicate event") {
    Roast r{12, 50};
    storage.roasts.push_back(r);

    auto& e1 = *(new Event{"measurement", 12345});
    auto& e2 = *(new Event{"measurement", 12345});

    roasty.addEventToRoast(12, e1);

    REQUIRE_THROWS(roasty.addEventToRoast(12, e2));
    delete &e2;
  }

  SECTION("Cannot add event to nonexistent roast") {
    Roast r{12, 50};
    storage.roasts.push_back(r);

    Event e1{"measurement", 12345};

    REQUIRE_THROWS(roasty.addEventToRoast(13, e1));
  }

  SECTION("Cannot add duplicate ingredient") {
    Roast r{12, 50};
    storage.roasts.push_back(r);

    auto& b = *(new Ingredient{*(new Bean{"Java"}), 400});
    auto& b2 = *(new Ingredient{*(new Bean{"Java"}), 400});

    roasty.addIngredientToRoast(12, b);

    REQUIRE_THROWS(roasty.addIngredientToRoast(12, b2));
    delete &b2;
  }

  SECTION("Cannot add event to nonexistent roast") {
    Roast r{12, 50};
    storage.roasts.push_back(r);

    auto& b = *(new Ingredient{*(new Bean{"Java"}), 400});
    auto& b2 = *(new Ingredient{*(new Bean{"Java"}), 400});

    REQUIRE_THROWS(roasty.addIngredientToRoast(300, b2));
    delete &b2;
    delete &b;
  }
  storage.getRoasts().clear();
}
