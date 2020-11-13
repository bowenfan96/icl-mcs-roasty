#pragma once

#include "Model/RoastyModel.hpp"
#include <nlohmann/json.hpp>

nlohmann::json roastToJson(const Roast& r);
Roast jsonToRoast(nlohmann::json& j);

nlohmann::json eventToJson(const Event& e);
Event* jsonToEvent(nlohmann::json& j);

nlohmann::json ingredientToJson(const Ingredient& e);
Ingredient* jsonToIngredient(nlohmann::json& j);
