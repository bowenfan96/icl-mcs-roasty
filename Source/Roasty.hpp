#pragma once

#include "Model/RoastyModel.hpp"
#include "Server/RoastyServer.hpp"
#include <vector>

template <typename StorageImplementation> struct Roasty {
public:
  static auto const errorCode = 400;

  explicit Roasty(StorageImplementation* storage) : storage(storage) {}

  void startServer();

  // ============== Bean =================
  std::vector<Bean> allBeans();
  void addBean(const Bean& bean);
  void deleteBean(const Bean& bean);
  void renameBean(const Bean& bean, std::string const& newName);

  // ============== Roasts ================
  std::vector<Roast> allRoasts();
  Roast& getRoast(long id);
  void addRoast(Roast const& r);
  void deleteRoast(long id);
  void replaceRoast(long oldId, const Roast& newRoast);

  // ============== Ingredients ================
  Ingredient getIngredientByBeanName(long roastId, std::string const& beanName);
  void addIngredientToRoast(long roastId, Ingredient const& ingredient);
  void removeIngredientFromRoast(long roastId, std::string const& beanName);
  void updateIngredient(long roastId, std::string const& beanName, int newAmount);

  // ============== Events ================
  Event getEventById(long roastId, long eventId);
  void addEventToRoast(long roastId, const Event& e);
  void removeEventFromRoast(long roastId, long eventTimestamp);
  void replaceEventInRoast(long roastId, long oldEventTimestamp, const Event& newEvent);

private:
  int const defaultPort = 1234;
  RoastyServer<Roasty<StorageImplementation>> roastyServer{"localhost", defaultPort, this};
  StorageImplementation* storage;
};
