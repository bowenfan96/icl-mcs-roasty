#include "RoastyModel.hpp"
#include <iostream>


/* =============== BEAN =============== */
Bean::Bean(std::string argName) : name(argName) {}          // normal constructor

Bean::Bean(Bean const& argBean) : name(argBean.name) {}     // copy constructor

Bean& Bean::operator=(Bean const& argBean) {                // copy assignment
  if(&argBean == this) {         // self-assignment guard
    return *this;
  }
  name = argBean.name;
  return *this;
}

std::string Bean::getName() const {       // returns name of the bean
  return name;
}


/* =============== INGREDIENT =============== */
Ingredient::Ingredient(Bean& argBean, int argAmount)                  // normal constructor
    : bean(argBean), amount(argAmount) {
  delete &argBean;
}

Ingredient::Ingredient(Ingredient const &argIngd)                     // copy constructor
    : bean(argIngd.bean), amount(argIngd.amount), next(argIngd.next) {}

Ingredient& Ingredient::operator=(const Ingredient& argIngd) {        // copy assignment
  if(&argIngd == this) {        // self-assignment guard
    return *this;
  }

  bean = argIngd.bean;
  amount = argIngd.amount;
  next = argIngd.next;
  return *this;
}

Bean Ingredient::getBean() const {      // returns bean object in ingredient
  return bean;
}

int Ingredient::getAmount() const {     // returns number of beans in ingredient
  return amount;
}


/* =============== EVENT VALUE =============== */
EventValue::EventValue(int argValue) : value(argValue) {}             // normal constructor

EventValue::EventValue(EventValue const& argEvntVal)                  // copy constructor
    : value(argEvntVal.value) {}

EventValue& EventValue::operator=(EventValue const& argEvntVal) {     // copy assignment
  if(&argEvntVal == this) {     // self-assignment guard
    return *this;
  }
  value = argEvntVal.value;
  return *this;
}

int EventValue::getValue() const {      // returns integral value of the EventValue object
  return value;
}


/* =============== EVENT =============== */
// constructor with eventValue object:
Event::Event(std::string argType, long argTimestamp, EventValue* ptrEventValue)
    : type(argType), timestamp(argTimestamp) {
  eventValue = new EventValue(*ptrEventValue);
  delete ptrEventValue;
}

Event::Event(std::string argType, long argTimestamp)    // constructor with no eventValue object
    : type(argType), timestamp(argTimestamp) {}

Event::Event(Event const& argEvnt)                      // copy constructor
    : type(argEvnt.type), timestamp(argEvnt.timestamp), next(argEvnt.next) {
  if(argEvnt.eventValue != nullptr) {         // check if eventValue exists - if yes, copy it
    eventValue = new EventValue(*argEvnt.eventValue);
  }
}

Event& Event::operator=(Event const& argEvnt) {
  if(&argEvnt == this) {      // self-assignment guard
    return *this;
  }
  delete eventValue;          // free eventValue object held by event

  type = argEvnt.type;
  timestamp = argEvnt.timestamp;
  if(argEvnt.eventValue != nullptr) {         // check if eventValue exists - if yes, copy it
    eventValue = new EventValue(*argEvnt.eventValue);
  }
  next = argEvnt.next;
  return *this;
}

Event::~Event() {     // destructor to free up eventValue object held within event
  delete eventValue;
}

long Event::getTimestamp() const {
  return timestamp;
}

std::string Event::getType() const {
  return type;
}

bool Event::hasValue() const {      // returns true if event has eventValue object
  return (eventValue != nullptr);
}

EventValue* Event::getValue() const {
  return eventValue;
}


/* =============== ROAST =============== */
Roast::Roast(long argId, long argTimestamp)       // roast constructor
    : id(argId), timestamp(argTimestamp) {
  num_ingredients = 0;
  num_events = 0;
}

Roast::Roast(Roast const& argRst)                 // roast deep copy constructor
    : id(argRst.id), timestamp(argRst.timestamp),
      num_ingredients(argRst.num_ingredients),
      num_events(argRst.num_events) {

  // deep copy of the events linked list until the end of list is reached
  // nothing to copy if RHS events list ptr is null, as LHS ptr already inited to null
  if(argRst.events != nullptr) {
    events = new Event(*argRst.events);             // copy first element
    Event* evntIterLHS = events;
    Event* evntIterRHS = argRst.events->next;

    while(evntIterRHS != nullptr) {                 // chase RHS list down until all copied to LHS
      evntIterLHS->next = new Event(*evntIterRHS);
      evntIterLHS = evntIterLHS->next;
      evntIterRHS = evntIterRHS->next;
    }
  }

  // deep copy of the ingredients linked list until the end of list is reached
  // nothing to copy if RHS ingredients list ptr is null, as LHS ptr already inited to null
  if(argRst.ingredients != nullptr) {
    ingredients = new Ingredient(*argRst.ingredients);
    Ingredient* ingdIterLHS = ingredients;
    Ingredient* ingdIterRHS = argRst.ingredients->next;

    while(ingdIterRHS != nullptr) {
      ingdIterLHS->next = new Ingredient(*ingdIterRHS);
      ingdIterLHS = ingdIterLHS->next;
      ingdIterRHS = ingdIterRHS->next;
    }
  }
}

Roast& Roast::operator=(const Roast& argRst) {    // roast copy assignment operator
  if(&argRst == this) {      // self-assignment guard
    return *this;
  }

  id = argRst.id;
  timestamp = argRst.timestamp;
  num_ingredients = argRst.num_ingredients;
  num_events = argRst.num_events;

  // destroy and free memory of the LHS events and ingredient linked lists, else memory will leak
  Event* delCurrEvnt = events;
  while(delCurrEvnt != nullptr) {    // chase the event list down and delete every LHS element
    Event* delNextEvnt = delCurrEvnt->next;
    delete delCurrEvnt;
    delCurrEvnt = delNextEvnt;
  }
  events = nullptr;

  Ingredient* delCurrIngd = ingredients;
  while(delCurrIngd != nullptr) {    // chase the ingredient list down and delete every LHS element
    Ingredient* delNextIngd = delCurrIngd->next;
    delete delCurrIngd;
    delCurrIngd = delNextIngd;
  }
  ingredients = nullptr;

  // now that LHS linked lists are deep destroyed, we can proceed to deep copy from RHS
  // deep copy of the events linked list until the end of list is reached
  // nothing to copy if RHS events list ptr is null, as LHS ptr already inited to null
  if(argRst.events != nullptr) {
    events = new Event(*argRst.events);             // copy first element
    Event* evntIterLHS = events;
    Event* evntIterRHS = argRst.events->next;

    while(evntIterRHS != nullptr) {                 // chase RHS list down until all copied to LHS
      evntIterLHS->next = new Event(*evntIterRHS);
      evntIterLHS = evntIterLHS->next;
      evntIterRHS = evntIterRHS->next;
    }
  }

  // deep copy of the ingredients linked list until the end of list is reached
  // nothing to copy if RHS ingredients list ptr is null, as LHS ptr already inited to null
  if(argRst.ingredients != nullptr) {
    ingredients = new Ingredient(*argRst.ingredients);
    Ingredient* ingdIterLHS = ingredients;
    Ingredient* ingdIterRHS = argRst.ingredients->next;

    while(ingdIterRHS != nullptr) {
      ingdIterLHS->next = new Ingredient(*ingdIterRHS);
      ingdIterLHS = ingdIterLHS->next;
      ingdIterRHS = ingdIterRHS->next;
    }
  }
  return *this;
}

Roast::~Roast() {   // roast deep destructor
  Event* delCurrEvnt = events;
  while(delCurrEvnt != nullptr) {
    Event* delNextEvnt = delCurrEvnt->next;
    delete delCurrEvnt;
    delCurrEvnt = delNextEvnt;
  }

  Ingredient* delCurrIngd = ingredients;
  while(delCurrIngd != nullptr) {
    Ingredient* delNextIngd = delCurrIngd->next;
    delete delCurrIngd;
    delCurrIngd = delNextIngd;
  }
}

// roast add functions
void Roast::addIngredient(Ingredient const& argIngd) {    // add a ingredient by reference
  if(ingredients == nullptr) {    // add ingredient at head node if list is empty
    ingredients = new Ingredient(argIngd);
  }
  else {    // chase the linked list down and new a ingredient to end to the list
    Ingredient* ingdIter = ingredients;
    while(ingdIter->next != nullptr) {
      ingdIter = ingdIter->next;
    }
    ingdIter->next = new Ingredient(argIngd);
  }
  num_ingredients++;

  // it is our responsibility to clean up argIngd passed into the function, else memory will leak
  delete &argIngd;
}

void Roast::addEvent(Event const& argEvnt) {    // add an event by reference
  if(events == nullptr) {     // at event at head node if list is empty
    events = new Event(argEvnt);
  }
  else {    // chase the linked list down and new an event to end to the list
    Event* evntIter = events;
    while(evntIter->next != nullptr) {
      evntIter = evntIter->next;
    }
    evntIter->next = new Event(argEvnt);
  }
  num_events++;

  // it is our responsibility to clean up argEvnt passed into the function, else memory will leak
  delete &argEvnt;
}

// roast get functions
long Roast::getId() const {
  return id;
}

long Roast::getTimestamp() const {
  return timestamp;
}

int Roast::getIngredientsCount() const {
  return num_ingredients;
}

int Roast::getEventCount() const {
  return num_events;
}

Ingredient& Roast::getIngredient(int index) const {   // get ingredient by linked list index
  if(ingredients != nullptr) {
    Ingredient* ingdIter = ingredients;
    int counter = 0;    // count how many elements we have chased down, until we hit the index
    while(ingdIter != nullptr) {
      if(counter == index) {
        return *ingdIter;
      }
      counter++;
      ingdIter = ingdIter->next;
    }
  }
  // if we reach here, the index given is invalid and we have nothing to return, call it quits
  throw INVALID_INDEX;
}

Event& Roast::getEvent(int index) const {     // get event by linked list index
  if(events != nullptr) {
    Event* evntIter = events;
    int counter = 0;    // count how many elements we have chased down, until we hit the index
    while(evntIter != nullptr) {
      if(counter == index) {
        return *evntIter;
      }
      counter++;
      evntIter = evntIter->next;
    }
  }
  // if we reach here, the index given is invalid and we have nothing to return, call it quits
  throw INVALID_INDEX;
}

// Roast remove functions
void Roast::removeEventByTimestamp(long const& argTimestamp) {
  if(events == nullptr) {     // nothing to remove if events list contains nothing
    return;
  }

  Event* delCurrEvnt = events;      // this is this iterator
  Event* delPrevEvnt = nullptr;     // this is to help us rejoin the link after deleting a node
  while(delCurrEvnt != nullptr) {   // iterate through the list and look for value to delete
    if(delCurrEvnt->getTimestamp() == argTimestamp) {
      if(delPrevEvnt == nullptr) {  // special case if first element is what we want to delete
        events = delCurrEvnt->next;
      }
      else {
        delPrevEvnt->next = delCurrEvnt->next;  // rejoin our broken list after deleting node
      }
      delete delCurrEvnt;   // delete element, free up memory, decrement events count and return
      num_events--;
      return;
    }
    // if we reached here, this isn't the element we're looking for, as Obi-Wan might say
    // so keep iterating - move along, move along
    delPrevEvnt = delCurrEvnt;
    delCurrEvnt = delCurrEvnt->next;
  }
}

// the logical sequence below is exactly the same as in the removeEventByTimestamp function
// refer to comments above for procedure explanations
void Roast::removeIngredientByBeanName(std::string const& argBeanName) {
  if(ingredients == nullptr) {
    return;
  }

  Ingredient* delCurrIngd = ingredients;
  Ingredient* delPrevIngd = nullptr;
  while(delCurrIngd != nullptr) {
    if(delCurrIngd->getBean().getName() == argBeanName) {
      if(delPrevIngd == nullptr) {
        ingredients = delCurrIngd->next;
      }
      else {
        delPrevIngd->next = delCurrIngd->next;
      }
      delete delCurrIngd;
      num_ingredients--;
      return;
    }

    delPrevIngd = delCurrIngd;
    delCurrIngd = delCurrIngd->next;
  }
}
