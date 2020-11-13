#include "Serialisation.hpp"
#include "Server/RoastyServerException.hpp"
#include <exception>
#include <functional>
#include <string>

using json = nlohmann::json;

template <typename T> T parseWithErrorHandling(std::function<T()> action) {
  try {
    return action();
  } catch(json::parse_error& e) {
    throw RoastyServerException(e.what(), 400);
  } catch(json::type_error& e) {
    throw RoastyServerException(e.what(), 400);
  } catch(std::exception& e) {
    throw RoastyServerException(e.what(), 500);
  }
}

json roastToJson(const Roast& r) {
  // clang-format off
  json roast {
    {"id", r.getId()},
    {"beginTimestamp", r.getTimestamp()}
  };
  // clang-format on

  for(auto i = 0U; i < r.getEventCount(); i++) {
    roast["events"].push_back(eventToJson(r.getEvent(i)));
  }

  for(auto i = 0U; i < r.getIngredientsCount(); i++) {
    roast["beans"].push_back(ingredientToJson(r.getIngredient(i)));
  }

  return roast;
}

Roast jsonToRoast(json& j) {
  return parseWithErrorHandling<Roast>([&] {
    auto roast = Roast{j["id"].get<long>(), j["beginTimestamp"].get<long>()};

    for(auto& event : j["events"]) {
      roast.addEvent(*jsonToEvent(event));
    }

    for(auto& blend : j["beans"]) {
      roast.addIngredient(*jsonToIngredient(blend));
    }

    return roast;
  });
}

json eventToJson(const Event& e) {
  // clang-format off
  json j{
    {"id", e.getTimestamp()},
    {"timestamp", e.getTimestamp()},
    {"type", e.getType()}
  };
  // clang-format on

  if(e.hasValue()) {
    j["value"] = e.getValue()->getValue();
  }

  return j;
}

Event* jsonToEvent(json& j) {
  return parseWithErrorHandling<Event*>([&] {
    EventValue* eventValue = nullptr;

    if(!j["value"].empty()) {
      auto value = j["value"].get<long>();
      eventValue = new EventValue(value);
    }
    auto type = j["type"].get<std::string>();
    auto timestamp = j["timestamp"].get<long>();
    return new Event{type, timestamp, eventValue};
  });
}

nlohmann::json ingredientToJson(const Ingredient& e) {
  // clang-format off
  return json{
    {"name", e.getBean().getName()},
    {"amount", e.getAmount()}
  };
  // clang-format on
}

Ingredient* jsonToIngredient(nlohmann::json& j) {
  return parseWithErrorHandling<Ingredient*>([&] {
    return new Ingredient{*(new Bean(j["name"].get<std::string>())), j["amount"].get<int>()};
  });
}
