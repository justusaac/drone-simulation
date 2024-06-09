#ifndef BatteryDecorator_H_
#define BatteryDecorator_H_

#define BATTERY_USE_RATE 0.001
#define BATTERY_CHARGE_RATE 0.02

#include "Drone.h"

/**
 * @brief Decorator for Drone objects to give them a limited battery life.
 */
class BatteryDecorator : public Drone {
 public:
  /**
   * @brief Constructor for a BatteryDecorator
   * @param details JSON object containing the drone's information
   * @param drone A pointer to the drone that is being decorated
   */
  BatteryDecorator(JsonObject& details, Drone* drone);
  /**
   * @brief Destructor
   */
  ~BatteryDecorator();
  /**
   * @brief Gets the next delivery in the scheduler if there is sufficient
   * battery life, otherwise schedules the drone to go and charge its battery
   */
  void getNextDelivery();
  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   * 
   * The battery level decreases with every update unless the drone is near
   * to a charging station, in which case it increases. The drone will
   * deliver packages until it is low on battery, then it will go to a 
   * charging station to refill its battery.
   */
  void update(double dt);
  /**
   * @brief Gets the availability status of the drone.
   * @return The availability of the drone
   */
  bool getAvailable();
  /**
   * @brief Links this decorator and the drone it's decorating to a simulation
   * model, giving it access to the model's public variables and functions.
   * @param[in] model The simulation model to link.
   */
  void linkModel(SimulationModel* model);
  /**
   * @brief Make an IObserver recieve all of the messages from this decorator 
   * and the drone that it is decorating
   * @param observer A pointer to the observer that will recieve messages
   */
  virtual void attach(IObserver* observer);

  /**
   * @brief Remove any IObserver observing this entity and its drone
   */
  virtual void detach();

 protected:
  double BatteryLevel;
  Drone* drone;
  bool charging;
  IStrategy* chargingStrategy;
};

#endif
