#include "RoastyModel.hpp"
#include <iostream>

// ================ BEAN ===================
Bean::Bean(std::string name) : name(name) {}

Bean::Bean(const Bean& bean) : name(bean.name) {

}

Bean& Bean::operator=(Bean const& bean) {
  if(&bean == this) {
    return *this;
  }
  this->name = bean.name;
  return *this;
}

Bean::Bean(Bean&& bean) : name(bean.name) {}

Bean& Bean::operator=(Bean&& bean) {
  if(&bean == this) {
    return *this;
  }
  this->name = bean.name;
  return*this;
}


std::string Bean::getName() const {
  return name;
}

// =============== INGREDIENT ===============
Ingredient::Ingredient(Bean& bean, int amount) : bean(bean), amount(amount) {
  delete &bean;
}

Ingredient::Ingredient(Ingredient const &ingd) : bean(ingd.bean),
                                                 amount(ingd.amount),
                                                 next(ingd.next) {

}

Ingredient& Ingredient::operator=(const Ingredient& ingd) {
  if(&ingd == this) {
    return *this;
  }
  delete next;

  this->bean = ingd.bean;
  this->amount = ingd.amount;
  this->next = ingd.next;
  return *this;
}



Bean Ingredient::getBean() const {
  return bean;
}

int Ingredient::getAmount() const {
  return amount;
}


// =========== EVENT VALUE =================

EventValue::EventValue(int value) : value(value) {}

EventValue::EventValue(EventValue const &evntVal) : value(evntVal.value) {}

EventValue& EventValue::operator=(const EventValue& evntVal) {
  if(&evntVal == this) {
    return *this;
  }
  this->value = evntVal.value;
  return *this;
}

int EventValue::getValue() const {
  return value;
}


// ================ EVENT ==================
Event::Event(std::string type, long timestamp, EventValue* eventValue)
    : type(type), timestamp(timestamp) {
  this->eventValue = new EventValue(*eventValue);
  delete eventValue;
}

Event::Event(std::string type, long timestamp)
    : type(type), timestamp(timestamp) {
}

Event::Event(Event const& evnt) : type(evnt.type),
                                  timestamp(evnt.timestamp),
                                  next(evnt.next) {
  if(evnt.eventValue != nullptr) {
    this->eventValue = new EventValue(*evnt.eventValue);
  }
}

Event& Event::operator=(const Event& evnt) {
  if(&evnt == this) {
    return *this;
  }
  // delete previous eventValue

  delete this->eventValue;
  delete next;

  this->type = evnt.type;
  this->timestamp = evnt.timestamp;

  if(evnt.eventValue != nullptr) {
    this->eventValue = new EventValue(*evnt.eventValue);
  }

  this->next = evnt.next;
  return *this;
}

Event::Event(Event&& evnt) : type(evnt.type),
                             timestamp(evnt.timestamp),
                             next(evnt.next) {
  if(evnt.eventValue != nullptr) {
    this->eventValue = new EventValue(*evnt.eventValue);
  }
  delete &evnt;
}


Event::~Event() {
  //std::cout<<"event destructor called" << std::endl;
  delete eventValue;
  //std::cout << "BAM" << std::endl;
  //std::cout << "event destroyed" << std::endl;
}






long Event::getTimestamp() const {
  //std::cout << "heres ur timestamp" << std::endl;
  return timestamp;
}


std::string Event::getType() const {
  return type;
}

bool Event::hasValue() const {
  return eventValue != nullptr;
}

EventValue* Event::getValue() const {
  return eventValue;
}


// ============== ROAST ===================
Roast::Roast(long id, long timestamp) : id(id), timestamp(timestamp) {
  num_ingredients = 0;
  num_events = 0;

  //std::cout << "Roast created" << std::endl;
}

Roast::Roast(const Roast& rst)
    : id(rst.id), timestamp(rst.timestamp),
      num_ingredients(rst.num_ingredients),
      num_events(rst.num_events) {

  if(rst.events == nullptr) {
    events = nullptr;
  }

  else {
    this->events = new Event(*rst.events);
    ev_count++;
    //std::cout << "Event created" << std::endl;
    Event* currentEvnt = this->events;
    Event* nextEvnt = rst.events->next;
    while(nextEvnt != nullptr) {
      currentEvnt->next = new Event(*nextEvnt);
      ev_count++;
      //std::cout << "Event created" << std::endl;
      currentEvnt = currentEvnt->next;
      nextEvnt = nextEvnt->next;
    }
  }

  if(rst.ingredients == nullptr) {
    ingredients = nullptr;
  }

  else {
    this->ingredients = new Ingredient(*rst.ingredients);
    ig_count++;
    //std::cout << "Ingredient created" << std::endl;
    Ingredient* currentIngd = this->ingredients;
    Ingredient* nextIngd = rst.ingredients->next;
    while(nextIngd != nullptr) {
      currentIngd->next = new Ingredient(*nextIngd);
      ig_count++;
      //std::cout << "Ingredient created" << std::endl;
      currentIngd = currentIngd->next;
      nextIngd = nextIngd->next;
    }
  }
}

Roast& Roast::operator=(const Roast& rst) {
  if(&rst == this) {
    return *this;
  }
  this->id = rst.id;
  this->timestamp = rst.timestamp;
  this->num_ingredients = rst.num_ingredients;
  this->num_events = rst.num_events;

  // delete all events in this first
  Event* delCurEvnt = this->events;
  while(delCurEvnt != nullptr) {
    Event* delNexEvnt = delCurEvnt->next;
    delete delCurEvnt;
    ev_del_count++;
    //std::cout << "Event deleted" << std::endl;
    delCurEvnt = delNexEvnt;
  }

  if(rst.events == nullptr) {
    this->events = nullptr;
  }

  else {



    this->events = new Event(*rst.events);
    ev_count++;
    //std::cout << "Event created" << std::endl;
    Event* currentEvnt = this->events;
    Event* nextEvnt = rst.events->next;
    while(nextEvnt != nullptr) {
      currentEvnt->next = new Event(*nextEvnt);
      ev_count++;
      //std::cout << "Event created" << std::endl;
      currentEvnt = currentEvnt->next;
      nextEvnt = nextEvnt->next;
    }
  }


  // delete all ingredients in this first
  Ingredient* delCurIngd = this->ingredients;
  while(delCurIngd != nullptr) {
    Ingredient* delNexIngd = delCurIngd->next;
    delete delCurIngd;
    ig_del_count++;
    //std::cout << "Ingredient deleted" << std::endl;
    delCurIngd = delNexIngd;
  }

  if(rst.ingredients == nullptr) {
    this->ingredients = nullptr;
  }

  else {




    this->ingredients = new Ingredient(*rst.ingredients);
    ig_count++;
    //std::cout << "Ingredient created" << std::endl;
    Ingredient* currentIngd = this->ingredients;
    Ingredient* nextIngd = rst.ingredients->next;
    while(nextIngd != nullptr) {
      currentIngd->next = new Ingredient(*nextIngd);
      ig_count++;
      //std::cout << "Ingredient created" << std::endl;
      currentIngd = currentIngd->next;
      nextIngd = nextIngd->next;
    }
  }

  return *this;
}









// ADD
void Roast::addIngredient(Ingredient const& ingd) {
  if(ingredients == nullptr) {
    ingredients = new Ingredient(ingd);
    ig_count++;
    //std::cout << "Ingredient created" << std::endl;
  }
  else {
    Ingredient* iterator = ingredients;
    while(iterator->next != nullptr) {
      iterator = iterator->next;
    }
    iterator->next = new Ingredient(ingd);
    ig_count++;
    //std::cout << "Ingredient created" << std::endl;
  }
  num_ingredients++;

  delete &ingd;
}

void Roast::addEvent(Event const& evnt) {
  //std::cout << "Hi Imma add event" << std::endl;
  if(events == nullptr) {
    //std::cout << "No event so making one" << std::endl;
    events = new Event(evnt);
    ev_count++;
    //std::cout << "Event created" << std::endl;
  }
  else {
    //std::cout << "adding to event list" << std::endl;
    Event* current_node = events;
    while(current_node->next != nullptr) {
      current_node = current_node->next;
    }
    current_node->next = new Event(evnt);
    ev_count++;
    //std::cout << "Event created" << std::endl;
  }
  //std::cout << "Event added" << std::endl;
  num_events++;

  delete &evnt;
}

// GET

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

Ingredient& Roast::getIngredient(int index) const {
  if(index >= num_ingredients) {
    exit(-1);
  }
  if(ingredients != nullptr) {
    Ingredient* iterator = ingredients;
    int counter = 0;
    while(iterator != nullptr) {
      if(counter == index) {
        return *iterator;
      }
      counter++;
      iterator = iterator->next;
    }
  }
}

Event& Roast::getEvent(int index) const {
  //std::cout << "Someone wants to getEvent at index: " << index << std::endl;
  if(events != nullptr) {
    Event* iterator = events;
    int counter = 0;
    while(iterator != nullptr) {
      if(counter == index) {
        //std::cout << "Event returned" << std::endl;
        return *iterator;
      }
      counter++;
      iterator = iterator->next;
    }
  }
}

// REMOVE

void Roast::removeEventByTimestamp(long timestamp) {
  if(events == nullptr) {
    return;
  }
  else {
    Event** iterator = &events;
    while((*iterator) != nullptr) {
      if((*iterator)->getTimestamp() == timestamp) {
        Event* old = (*iterator);
        (*iterator) = (*iterator)->next;
        delete old;
        ev_del_count++;
        //std::cout << "Event deleted" << std::endl;
        num_events--;
        return;
      }
      iterator = &((*iterator)->next);
    }
  }
}

void Roast::removeIngredientByBeanName(std::string beanName) {
  if(ingredients == nullptr) {
    return;
  }
  else {
    Ingredient** iterator = &ingredients;
    while((*iterator) != nullptr) {
      if((*iterator)->getBean().getName() == beanName) {
        Ingredient* old = (*iterator);
        (*iterator) = (*iterator)->next;
        delete old;
        ig_del_count++;
        //std::cout << "Ingredient deleted" << std::endl;
        num_ingredients--;
        //std::cout << "ingredient removed: " << beanName << std::endl;

        return;
      }
      iterator = &((*iterator)->next);
    }
  }
}


// DESTRUCTOR

Roast::~Roast() {
  //std::cout << "roast destructor called" << std::endl;
  Event* currentEvnt = events;
  while(currentEvnt != nullptr) {
    Event* nextEvnt = currentEvnt->next;
    delete currentEvnt;
    //std::cout << "BOOM" << std::endl;
    ev_del_count++;
    //std::cout << "Event deleted" << std::endl;
    currentEvnt = nextEvnt;
  }

  Ingredient* currentIngd = ingredients;
  while(currentIngd != nullptr) {
    Ingredient* nextIngd = currentIngd->next;
    delete currentIngd;
    ig_del_count++;
    //std::cout << "Ingredient deleted" << std::endl;
    currentIngd = nextIngd;
  }

  std::cout << "Ig Count: " << ig_count << std::endl;
  std::cout << "Ev Count: " << ev_count << std::endl;
  std::cout << "Ig Del Count: " << ig_del_count << std::endl;
  std::cout << "Ev Del Count: " << ev_del_count << std::endl;
  std::cout << "Num ig: " << num_ingredients << std::endl;
  std::cout << "Num ev: " << num_events << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  //std::cout << "roast destroyed" << std::endl;
}
