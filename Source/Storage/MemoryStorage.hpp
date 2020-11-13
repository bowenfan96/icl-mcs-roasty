#pragma once

class MemoryStorage {
public:
  // Use these methods to interact with the database
  std::vector<Bean> getBeans() const { return beans; }
  size_t getBeanCount() const { return beans.size(); }
  void addBean(Bean const& b) { beans.push_back(b); };
  void replaceBean(size_t position, Bean const& b) { beans[position] = b; }
  void removeBean(size_t i) { beans.erase(beans.begin() + i); };
  Bean& getBean(int i) { return beans[i]; };

  void setBean(std::vector<Bean> beans) { this->beans = beans; }

  std::vector<Roast>& getRoasts() { return roasts; }
  void setRoasts(std::vector<Roast> roasts) { this->roasts = roasts; }

  std::vector<Bean> beans;
  std::vector<Roast> roasts;
};
