#pragma once
#include <string>

class Bean {
private:
  std::string name;

public:
  Bean(std::string name);       // normal constructor

  Bean(Bean const &bean);       // copy constructor (rule of 3)
  Bean& operator=(Bean const& bean);   // copy assignment (rule of 3)

  std::string getName() const;



};


class Ingredient {
private:
  Bean bean;

  int amount;

public:
  Ingredient* next = nullptr;

  Ingredient(Bean& bean, int amount);   // normal constructor
  Ingredient(Ingredient const &ingd);               // copy constructor
  Ingredient& operator=(Ingredient const& ingd);    // assignment
           // destructor


  Bean getBean() const;
  int getAmount() const;


};


struct EventValue {
  int value;
  int getValue() const;

  EventValue(int value);              // normal constructor
  EventValue(EventValue const &evntVal);    // copy constructor
  EventValue& operator=(EventValue const& evntVal);   // assignment

};

class Event {
private:
  std::string type;
  long timestamp;
  EventValue* eventValue = nullptr;

public:
  Event* next = nullptr;

  Event(std::string type, long timestamp, EventValue* eventValue);
  Event(std::string type, long timestamp);

  Event(Event const& evnt);          // copy constructor
  Event& operator=(Event const& evnt);    // assignment
  ~Event();


  long getTimestamp() const;
  std::string getType() const;
  bool hasValue() const;

  EventValue* getValue() const;

};


class Roast {
private:
  long id;
  long timestamp;

  Ingredient* ingredients = nullptr;
  int num_ingredients;

  Event* events = nullptr;
  int num_events;


public:

  Roast(long id, long timestamp);   // normal constructor
  Roast(Roast const& rst);          // copy constructor
  Roast& operator=(Roast const& rst);   // assignment

  ~Roast();

  // Roast Add Functions
  void addIngredient(const Ingredient& ingd);
  void addEvent(const Event& evnt);

  // Roast Get Functions
  long getId() const;
  long getTimestamp() const;

  int getIngredientsCount() const;
  int getEventCount() const;

  Ingredient& getIngredient(int index) const;
  Event& getEvent(int index) const;

  // Roast Remove Functions
  void removeEventByTimestamp(long);
  void removeIngredientByBeanName(std::string);

  int ev_count = 0;
  int ig_count = 0;

  int ev_del_count = 0;
  int ig_del_count = 0;
};