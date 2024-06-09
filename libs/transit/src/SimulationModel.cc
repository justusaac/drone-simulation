#include "SimulationModel.h"

#include "BatteryFactory.h"
#include "DroneFactory.h"
#include "HelicopterFactory.h"
#include "HumanFactory.h"
#include "PackageFactory.h"
#include "RobotFactory.h"


SimulationModel::SimulationModel(IController& controller)
    : controller(controller) {
  entityFactory.AddFactory(new DroneFactory());
  entityFactory.AddFactory(new PackageFactory());
  entityFactory.AddFactory(new RobotFactory());
  entityFactory.AddFactory(new HumanFactory());
  entityFactory.AddFactory(new HelicopterFactory());
  entityFactory.AddFactory(new BatteryFactory());
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (auto& [id, entity] : entities) {
    delete entity;
  }
  delete graph;
}

IEntity* SimulationModel::createEntity(JsonObject& entity) {
  std::string name = entity["name"];
  JsonArray position = entity["position"];
  std::cout << name << ": " << position << std::endl;

  IEntity* myNewEntity = nullptr;
  if (myNewEntity = entityFactory.CreateEntity(entity)) {
    // Call AddEntity to add it to the view
    myNewEntity->linkModel(this);
    controller.addEntity(*myNewEntity);
    entities[myNewEntity->getId()] = myNewEntity;
    std::string type = entity["type"];
    if (ignoredTypes.count(type) == 0) {
      myNewEntity->attach(this);
    }
  }

  return myNewEntity;
}

void SimulationModel::removeEntity(int id) { removed.insert(id); }

/// Schedules a Delivery for an object in the scene
void SimulationModel::scheduleTrip(JsonObject& details) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  Robot* receiver = nullptr;

  for (auto& [id, entity] : entities) {
    if (name == entity->getName()) {
      if (Robot* r = dynamic_cast<Robot*>(entity)) {
        if (r->requestedDelivery) {
          receiver = r;
          break;
        }
      }
    }
  }

  Package* package = nullptr;

  for (auto& [id, entity] : entities) {
    if (name + "_package" == entity->getName()) {
      if (Package* p = dynamic_cast<Package*>(entity)) {
        if (p->requiresDelivery) {
          package = p;
          break;
        }
      }
    }
  }

  if (receiver && package) {
    package->initDelivery(receiver);
    std::string strategyName = details["search"];
    package->setStrategyName(strategyName);
    scheduledDeliveries.push_back(package);
    controller.sendEventToView("DeliveryScheduled", details);
  }
}

const routing::IGraph* SimulationModel::getGraph() { return graph; }

/// Updates the simulation
void SimulationModel::update(double dt) {
  for (auto& [id, entity] : entities) {
    entity->update(dt);
    controller.updateEntity(*entity);
  }
  for (int id : removed) {
    removeFromSim(id);
  }
  removed.clear();
}

void SimulationModel::stop(void) { controller.stop(); }

void SimulationModel::removeFromSim(int id) {
  IEntity* entity = entities[id];
  if (entity) {
    for (auto i = scheduledDeliveries.begin(); i != scheduledDeliveries.end();
         ++i) {
      if (*i == entity) {
        scheduledDeliveries.erase(i);
        break;
      }
    }
    controller.removeEntity(*entity);
    entities.erase(id);
    delete entity;
  }
}


Battery* SimulationModel::getNearestCharger(Vector3 pos) {
  Battery* output = nullptr;
  double bestDist = INFINITY;
  for (const auto& [key, value] : this->entities) {
    std::string type = value->getDetails()["type"];
    if (type.compare("battery") == 0) {
      if (value->getPosition().dist(pos) < bestDist) {
        bestDist = value->getPosition().dist(pos);
        output = dynamic_cast<Battery*>(value);
      }
    }
  }
  return output;
}

void SimulationModel::toggleMessages(JsonObject data) {
  std::string targetType = data["type"];
  bool currentObservation = (ignoredTypes.count(targetType) == 0);
  if (currentObservation) {
    // Detach all entities of the given type
    ignoredTypes.insert(targetType);
    for (const auto& [id, entityPtr] : this->entities) {
      std::string type = (entityPtr->getDetails())["type"];
      if (type == targetType) {
        entityPtr->detach();
      }
    }
  } else {
    // Attach all entities of the given type
    ignoredTypes.erase(targetType);
    for (const auto& [id, entityPtr] : this->entities) {
      std::string type = (entityPtr->getDetails())["type"];
      if (type == targetType) {
        entityPtr->attach(this);
      }
    }
  }
}

void SimulationModel::relayMessage(std::string message) {
      // std::cout << message << std::endl;
      JsonObject details;
      details["message"] = message;

      controller.sendEventToView("AddMessage", details);
}
