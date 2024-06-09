#ifndef SIMULATION_MODEL_H_
#define SIMULATION_MODEL_H_

#include <deque>
#include <map>
#include <set>

#include "Battery.h"
#include "CompositeFactory.h"
#include "Drone.h"
#include "IController.h"
#include "IEntity.h"
#include "Robot.h"
#include "graph.h"
#include "math/vector3.h"
#include <deque>
#include <map>
#include <set>
#include "IObserver.h"

//--------------------  Model ----------------------------

/// Simulation Model handling the transit simulation.  The model can communicate
/// with the controller.
/**
 * @brief Class SimulationModel handling the transit simulation. it can
 * communicate with the controller
 **/
class SimulationModel:public IObserver {
 public:
  /**
   * @brief Default constructor that create the SimulationModel object
   **/
  SimulationModel(IController& controller);

  /**
   * @brief Destructor
   */
  ~SimulationModel();

  /**
   * @brief Set the Graph for the SimulationModel
   * @param graph Type IGraph* contain the new graph for SimulationModel
   **/
  void setGraph(const routing::IGraph* graph) { this->graph = graph; }

  /**
   * @brief Creates a new simulation entity
   * @param entity Type JsonObject contain the entity's reference to decide
   *which entity to create
   **/
  IEntity* createEntity(JsonObject& entity);

  /**
   * @brief Removes entity with given ID from the simulation
   *
   * @param id of the entity to be removed
   */
  void removeEntity(int id);

  /**
   * @brief Schedule a trip for an object in the scene
   * @param detail Type JsonObject contain the entity's reference to schedule
   *the detail of the trip being scheduled
   **/
  void scheduleTrip(JsonObject& details);

  /**
   * @brief Update the simulation
   * @param dt Type double contain the time since update was last called.
   **/
  void update(double dt);

  /**
   * @brief Stops the simulation
   * @return Void
   **/
  void stop();
  /**
   * @brief Receive a message and send it to the user's view via the controller
   * @param message The message that was received
   **/
  void relayMessage(std::string message);

  /**
   * @brief Returns the graph of the map
   *
   * @returns IGraph* graph pointer
   */
  const routing::IGraph* getGraph();
  /**
   * @brief Find the battery that is nearest to the specified position
   * @param pos The position relative to which the distances will be calculated
   * @return A pointer to the nearest Battery or a nullptr if there aren't any
   * batteries in the simulation
   */
  Battery* getNearestCharger(Vector3 pos);
  /**
   * @brief Switch whether messages are recieved or not from a given type of 
   * entity in the simulation.
   * @param data A Json object where the "type" field is the specific type of
   * entity to toggle messages for.
   **/
  void toggleMessages(JsonObject data);

  std::deque<Package*> scheduledDeliveries;

 protected:
  std::set<std::string> ignoredTypes;
  IController& controller;
  std::map<int, IEntity*> entities;
  std::set<int> removed;
  void removeFromSim(int id);
  const routing::IGraph* graph;
  CompositeFactory entityFactory;
};
#endif
