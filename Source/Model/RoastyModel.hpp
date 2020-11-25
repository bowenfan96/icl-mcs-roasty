#pragma once
#include <string>

class Bean {
private:
  std::string name;
public:
  Bean(std::string& Name);
  std::string& getName();

  Bean& operator=(const Bean& copy);

};

class Ingredient {
private:
  Bean* bean;
  int amount;

public:
  Ingredient(Bean bean, int amount);
  Ingredient(const Ingredient& copy);
  Ingredient& operator=(const Ingredient& copy);
  Ingredient(Ingredient&& source);

  ~Ingredient();

  Bean getBean(std::string);
  int getAmount();

};

class Event {
private:
  std::string type;
  int timestamp;
  int eventValue;

public:
  Event(std::string, int, int*);
  int getTimestamp();

};


class Roast {
private:
  int id;
  int timestamp;

  Ingredient* ingredients = new Ingredient[1];
  int max_ingredients;
  int num_ingredients;

  Event* events;
  int max_events;
  int num_events;


public:

  Roast(int, int);
  Roast(Roast const& copy);
  ~Roast();

  // Roast Add Functions
  void addIngredient(Ingredient);
  void addEvent(Event);

  // Roast Get Functions
  int getId();
  int getTimestamp();

  Ingredient getIngredient(int);
  Event getEvent(int);
  int getIngredientsCount();
  int getEventCount();

  // Roast Remove Functions
  void removeEventByTimestamp(int);
  void removeIngredientByBeanName(std::string);
};