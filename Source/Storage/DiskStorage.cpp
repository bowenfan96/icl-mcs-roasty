#include "DiskStorage.hpp"
#include "../Serialisation.hpp"
#include "../Server/RoastyServerException.hpp"
#include <exception>
#include <iostream>
#include <string>
static auto const IOdebug = false;
// ============== Bean =======================

void DiskStorage::addBean(Bean const& b) {
  auto allBeans = getBeans();
  allBeans.push_back(b);
  setBean(allBeans);
}

void DiskStorage::replaceBean(size_t position, Bean const& b) {
  auto allBeans = getBeans();
  allBeans[position] = b;
  setBean(allBeans);
}

void DiskStorage::removeBean(size_t position) {
  auto allBeans = getBeans();
  allBeans.erase(allBeans.begin() + position);
  setBean(allBeans);
}

// Private method to flush to disk
void DiskStorage::setBean(std::vector<Bean> const& bean) {
  std::vector<std::string> beanNames{};

  beanNames.reserve(bean.size());
  for(const auto& bean : bean) {
    beanNames.push_back(bean.getName());
  }

  json j;
  j["beans"] = beanNames;
  if(IOdebug)
    std::cout << j.dump(2) << std::endl;
  writeJson(beansJsonFileName, j);
}

std::vector<Bean> const& DiskStorage::getBeans() {
  beans = {};
  auto data = readJson("../beans.json");
  auto& beanStrings = data["beans"];

  try {
    for(auto& bean : beanStrings) {
      auto value = bean.get<std::string>();
      beans.emplace_back(Bean{value});
    }
  } catch(std::exception& e) {
    std::stringstream message{};
    message << "Corrupt database file bean.json! Error while reading: " << e.what();
    throw RoastyServerException{message.str(), 500};
  }
  return beans;
}

// ==================== Roasts =============================
std::vector<Roast>& DiskStorage::getRoasts() {
  roasts.clear();
  auto json = readJson("../roasts.json");

  try {
    for(auto& roastJ : json) {
      roasts.push_back(jsonToRoast(roastJ));
    }

  } catch(std::exception& e) {
    std::stringstream message{};
    message << "Corrupt database file roasts.json! Error while reading: " << e.what();
    throw RoastyServerException{message.str(), 500};
  }

  return roasts;
}

void DiskStorage::setRoasts(std::vector<Roast> const& roasts) {
  json j;

  for(const auto& roast : roasts) {
    j.push_back(roastToJson(roast));
  }
  if(IOdebug)
    std::cout << j.dump(2) << std::endl;
  writeJson(roastsJsonFileName, j);
}

json DiskStorage::readJson(std::string const& file) {
  std::ifstream i(file);

  if(i.fail()) {
    return {};
  }

  try {
    json j;
    i >> j;
    return j;
  } catch(std::exception& e) {
    throw RoastyServerException("Corrupt database file", 500);
  }
}

void DiskStorage::writeJson(std::string const& file, json& j) {
  std::ofstream o(file);

  if(o.fail()) {
    throw RoastyServerException{"Error reading database file", 500};
  }

  if(j.empty()) {
    j = std::vector<int>{};
  }

  o << j;
}
