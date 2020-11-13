#include "Roasty.hpp"
#include "Server/RoastyServerException.hpp"
#include "Storage/DiskStorage.hpp"
#include "Storage/MemoryStorage.hpp"
#include "Utilities.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

template <typename StorageImplementation> void Roasty<StorageImplementation>::startServer() {
  std::cout << "Listening on http://" << roastyServer.getInterface() << ":"
            << roastyServer.getPort() << std::endl;

  std::cout << "ctrl+c to quit" << std::endl;

  roastyServer.startServer();
}

// ==================== Bean ========================
template <typename RoastyImplementation>
std::vector<Bean> Roasty<RoastyImplementation>::allBeans() {
  return storage->getBeans();
}

auto const check_unique = [](auto& container, auto comparator) {
  auto it = std::find_if(container.begin(), container.end(), comparator);
  return it == container.end();
};

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::addBean(const Bean& bean) {
  auto allBean = storage->getBeans();

  if(!check_unique(allBean, [&bean](const auto& b) { return b.getName() == bean.getName(); })) {
    throw RoastyServerException{"Cannot add bean, they already exist.", errorCode};
  }
  storage->addBean(bean);
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::deleteBean(const Bean& bean) {
  for(auto i = 0U; i < storage->getBeans().size(); i++) {
    if(storage->getBean(i).getName() == bean.getName()) {
      storage->removeBean(i);
      return;
    }
  }
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::renameBean(const Bean& bean, std::string const& newName) {
  for(auto i = 0U; i < storage->getBeans().size(); i++) {
    if(storage->getBean(i).getName() == bean.getName()) {
      storage->replaceBean(i, Bean{storage->getBean(i).getName()});
    }
  }
};

// ====================== Roast =========================
template <typename RoastyImplementation>
std::vector<Roast> Roasty<RoastyImplementation>::allRoasts() {
  return storage->getRoasts();
}

template <typename RoastyImplementation> Roast& Roasty<RoastyImplementation>::getRoast(long id) {
  auto& allRoasts = storage->getRoasts();

  auto it = std::find_if(allRoasts.begin(), allRoasts.end(),
                         [id](auto& elem) { return elem.getId() == id; });

  if(it == allRoasts.end()) {
    throw RoastyServerException{"Unknown roast id", errorCode};
  }

  return *it;
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::addRoast(Roast const& roast) {
  auto& roasts = storage->getRoasts();
  for(auto i = 0U; i < roasts.size(); i++) {
    if(roasts[i].getId() == roast.getId()) {
      throw RoastyServerException{"Cannot add roast, id already exists.", errorCode};
    }
  }
  roasts.push_back(roast);
  storage->setRoasts(roasts);
}

template <typename RoastyImplementation> void Roasty<RoastyImplementation>::deleteRoast(long id) {
  auto& allRoasts = storage->getRoasts();
  allRoasts.erase(std::remove_if(allRoasts.begin(), allRoasts.end(),
                                 [id](auto& elem) { return elem.getId() == id; }),
                  allRoasts.end());
  storage->setRoasts(allRoasts);
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::replaceRoast(long oldId, const Roast& newRoast) {
  auto allRoasts = storage->getRoasts();

  auto it = std::find_if(allRoasts.begin(), allRoasts.end(),
                         [oldId](auto& elem) { return elem.getId() == oldId; });

  if(it == allRoasts.end()) {
    throw RoastyServerException{"Unknown roast id", errorCode};
  }

  *it = newRoast;
  storage->setRoasts(allRoasts);
}

template <typename RoastyImplementation>
Ingredient Roasty<RoastyImplementation>::getIngredientByBeanName(long roastId,
                                                                 std::string const& beanName) {
  auto& roast = getRoast(roastId);

  auto ingredients = RangeGenerator<const Ingredient>(
      [&](auto i) -> Ingredient const& { return roast.getIngredient(i); },
      roast.getIngredientsCount());

  auto it =
      std::find_if(ingredients.begin(), ingredients.end(), [&beanName](const auto& ingredient) {
        return ingredient.getBean().getName() == beanName;
      });
  if(it == ingredients.end()) {
    std::stringstream message{};
    message << "No bean with name " << beanName << " in roast " << roastId;
    throw RoastyServerException(message.str(), errorCode);
  }

  return *it;
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::addIngredientToRoast(long roastId,
                                                        const Ingredient& ingredient) {
  auto roast = getRoast(roastId);

  auto ingredients = RangeGenerator<const Ingredient>(
      [&](auto i) -> Ingredient const& { return roast.getIngredient(i); },
      roast.getIngredientsCount());

  if(!check_unique(ingredients, [&ingredient](const auto& b) {
       return b.getBean().getName() == ingredient.getBean().getName();
     })) {
    throw RoastyServerException{"Cannot add ingredient, ingredient already exists.", errorCode};
  }
  roast.addIngredient(ingredient);
  replaceRoast(roastId, roast);
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::removeIngredientFromRoast(long roastId,
                                                             std::string const& beanName) {
  auto roast = getRoast(roastId);
  roast.removeIngredientByBeanName(beanName);
  replaceRoast(roastId, roast);
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::updateIngredient(long roastId, std::string const& beanName,
                                                    int newAmount) {
  auto roast = getRoast(roastId);
  auto* newIngredient = new Ingredient{*(new Bean{beanName}), newAmount};
  roast.removeIngredientByBeanName(beanName);
  roast.addIngredient(*newIngredient);

  replaceRoast(roastId, roast);
}

template <typename RoastyImplementation>
Event Roasty<RoastyImplementation>::getEventById(long roastId, long eventTimestamp) {
  auto& roast = getRoast(roastId);
  auto events = RangeGenerator<const Event>(
      [&](auto i) -> Event const& { return roast.getEvent(i); }, roast.getEventCount());

  auto it = std::find_if(events.begin(), events.end(), [eventTimestamp](const auto& event) {
    return event.getTimestamp() == eventTimestamp;
  });

  if(it == events.end()) {
    std::stringstream message{};
    message << "No event with timestamp " << eventTimestamp << " in roast" << roastId;
    throw RoastyServerException(message.str(), errorCode);
  }

  return *it;
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::addEventToRoast(long roastId, const Event& e) {
  auto roast = getRoast(roastId);
  for (auto i = 0u; i < roast.getEventCount(); i++) {
    if(roast.getEvent(i).getTimestamp() == e.getTimestamp())
      throw RoastyServerException{"Cannot add event, id already exists.", errorCode};
  }
  roast.addEvent(e);
  replaceRoast(roastId, roast);
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::removeEventFromRoast(long roastId, long eventTimestamp) {
  auto roast = getRoast(roastId);
  roast.removeEventByTimestamp(eventTimestamp);
  replaceRoast(roastId, roast);
}

template <typename RoastyImplementation>
void Roasty<RoastyImplementation>::replaceEventInRoast(long roastId, long oldEventTimestamp,
                                                       const Event& newEvent) {
  auto roast = getRoast(roastId);
  roast.removeEventByTimestamp(oldEventTimestamp);
  roast.addEvent(newEvent);
  replaceRoast(roastId, roast);
}

template struct Roasty<MemoryStorage>;
template struct Roasty<DiskStorage>;
