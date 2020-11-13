#pragma once

#include "../Model/RoastyModel.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

class DiskStorage {
public:
  std::vector<Bean> const& getBeans();
  void addBean(Bean const& b);
  void removeBean(size_t position);
  void replaceBean(size_t position, Bean const& b);
  Bean const& getBean(int i) { return getBeans()[i]; };

  std::vector<Roast>& getRoasts();
  void setRoasts(std::vector<Roast> const& roasts);

private:
  // Internal methods used to store to disk
  std::vector<Bean> beans;
  std::vector<Roast> roasts;
  std::string const beansJsonFileName = "../beans.json";
  std::string const roastsJsonFileName = "../roasts.json";
  void setBean(std::vector<Bean> const& beans);

  static json readJson(std::string const& file);
  static void writeJson(std::string const& file, json& j);
};
