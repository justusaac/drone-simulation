#include "BatteryDecorator.h"

#include "Battery.h"
#include "BeelineStrategy.h"
#include "SimulationModel.h"

BatteryDecorator::BatteryDecorator(JsonObject& details, Drone* drone)
    : Drone(details) {
  this->drone = drone;
  BatteryLevel = 1.0;
  charging = false;
  chargingStrategy = NULL;
}

BatteryDecorator::~BatteryDecorator() {
  delete this->chargingStrategy;
  delete this->drone;
}

void BatteryDecorator::linkModel(SimulationModel* model) {
  IEntity::linkModel(model);
  drone->linkModel(model);
}

void BatteryDecorator::attach(IObserver* observer) {
  IEntity::attach(observer);
  drone->attach(observer);
}

void BatteryDecorator::detach() {
  IEntity::detach();
  drone->detach();
}

void BatteryDecorator::update(double dt) {
  // std::cout<<BatteryLevel<<std::endl;
  double originalLevel = this->BatteryLevel;
  this->BatteryLevel -= BATTERY_USE_RATE * dt;
  Battery* nearest = (this->model)->getNearestCharger(this->position);
  if (nearest && nearest->getPosition().dist(this->position) < 5.0) {
    this->BatteryLevel += BATTERY_CHARGE_RATE * dt;
    if (this->BatteryLevel >= 1.0) {
      if (originalLevel < 1.0) {
        std::string new_message = name +
          std::string("'s battery is charged up.\n");
        sendMessage(new_message);
      }
      this->BatteryLevel = 1.0;
    }
  }
  if (this->BatteryLevel <= 0.0) {
    if (originalLevel > 0.0) {
      std::string new_message = name + std::string(" is out of battery.\n");
      sendMessage(new_message);
    }
    this->BatteryLevel = 0.0;
    return;
  }
  if (this->charging) {
    if (!chargingStrategy->isCompleted()) {
      chargingStrategy->move(this, dt);
      drone->setPosition(this->position);
      drone->setDirection(this->direction);
    }
    if (this->BatteryLevel >= 1.0) {
      this->charging = false;
    }
  } else {
    if (drone->getAvailable()) {
      this->getNextDelivery();
    }
    drone->update(dt);
  }
  this->position = drone->getPosition();
  this->direction = drone->getDirection();
}

void BatteryDecorator::getNextDelivery() {
  Battery* charger = (this->model)->getNearestCharger(this->position);
  if (charger && (this->BatteryLevel < 0.5 ||
      (this->BatteryLevel < 0.9 && model->scheduledDeliveries.size() == 0))) {
    // Set objective to go to the charger
    std::string new_message = name + std::string(" is headed to ");
    new_message += charger->getName() + std::string(" to charge up.\n");
    sendMessage(new_message);
    Vector3 destination = charger->getPosition();
    delete this->chargingStrategy;
    this->chargingStrategy = new BeelineStrategy(this->position, destination);
    this->charging = true;
  } else {
    // Deliver a package
    drone->getNextDelivery();
  }
}

bool BatteryDecorator::getAvailable() {
  return drone->getAvailable() && !this->charging;
}
