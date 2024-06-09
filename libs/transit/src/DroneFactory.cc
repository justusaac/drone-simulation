#include "DroneFactory.h"

#include "BatteryDecorator.h"

IEntity* DroneFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("drone") == 0) {
    std::cout << "Drone Created" << std::endl;
    // return new Drone(entity);
    Drone* orig = new Drone(entity);
    return new BatteryDecorator(entity, orig);
  }
  return nullptr;
}
