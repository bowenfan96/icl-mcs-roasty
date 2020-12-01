#pragma once
#include <string>

// error codes
#define INVALID_INDEX -1


/* =============== BEAN =============== */
class Bean {
private:
  std::string name;     // bean has just a name
public:
  Bean(std::string const& argName);        // normal constructor
  Bean(Bean const& argBean);               // copy constructor
  Bean& operator=(Bean const& argBean);    // copy assignment

  std::string const& getName() const;
};


/* =============== INGREDIENT =============== */
class Ingredient {
private:
  Bean bean;      // ingredient contains a bean and,
  int amount;     // the number of beans as an integer

public:
  Ingredient* next = nullptr;       // linked list implementation

  Ingredient(Bean const& argBean, int argAmount);         // normal constructor
  Ingredient(Ingredient const& argIngd);                  // copy constructor
  Ingredient& operator=(Ingredient const& argIngd);       // copy assignment

  Bean const& getBean() const;
  int getAmount() const;
};


/* =============== EVENT VALUE =============== */
class EventValue {
private:
  int value;      // eventvalue has just an integer value
public:
  EventValue(int argValue);                                // normal constructor
  EventValue(EventValue const& argEvntVal);                // copy constructor
  EventValue& operator=(EventValue const& argEvntVal);     // copy assignment

  int getValue() const;
};


/* =============== EVENT =============== */
class Event {
private:
  std::string type;                     // event has a type,
  long timestamp;                       // a timestamp, and,
  EventValue* eventValue = nullptr;     // an optional eventValue object

public:
  Event* next = nullptr;      // linked list implementation

  // constructor for event with eventValue object:
  Event(std::string const& argType, long argTimestamp, EventValue* ptrEventValue);
  // constructor for event with no eventValue object:
  Event(std::string const& argType, long argTimestamp);

  Event(Event const& argEvnt);               // copy constructor
  Event& operator=(Event const& argEvnt);    // copy assignment
  ~Event();                                  // destructor to free contained eventValue object

  long getTimestamp() const;
  std::string const& getType() const;
  bool hasValue() const;
  EventValue* getValue() const;
};


/* =============== ROAST =============== */
class Roast {
private:
  long id;
  long timestamp;

  Ingredient* ingredients = nullptr;          // head node of the ingredients linked list
  int num_ingredients;

  Event* events = nullptr;
  int num_events;                             // head node of the events linked list

public:
  Roast(long argId, long argTimestamp);       // normal constructor
  Roast(Roast const& argRst);                 // deep copy constructor
  Roast& operator=(Roast const& argRst);      // deep copy assignment
  ~Roast();                                   // deep destructor

  // roast add functions
  void addIngredient(Ingredient const& argIngd);
  void addEvent(Event const& argEvnt);

  // roast get functions
  long getId() const;
  long getTimestamp() const;
  int getIngredientsCount() const;
  int getEventCount() const;
  // return const& to protect our Ingredient from being changed
  Ingredient const& getIngredient(int index) const;
  // return const& to protect our Event from being changed
  Event const& getEvent(int index) const;

  // roast remove functions
  void removeEventByTimestamp(long argTimestamp);
  void removeIngredientByBeanName(std::string const& argBeanName);
};
