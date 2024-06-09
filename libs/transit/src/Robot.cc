#include "Robot.h"
#include "SimulationModel.h"

Robot::Robot(JsonObject &obj) : IEntity(obj) {}

void Robot::update(double dt) {}

void Robot::receive(Package* p) {
  package = p;
  std::string new_message = name + std::string(" has picked up Package.\n");
  sendMessage(new_message);
}
