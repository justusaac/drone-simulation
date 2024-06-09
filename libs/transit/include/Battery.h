#ifndef Battery_H_
#define Battery_H_

#include "IEntity.h"

/**
 * @brief Battery class representing positions where BatteryDecorator drones
 * can recharge their batteries.
 */
class Battery : public IEntity {
 public:
  /**
   * @brief Constructor for a Battery
   * @param obj JSON object containing the battery's information
   */
  Battery(JsonObject& obj);
  /**
   * @brief Destructor
   */
  ~Battery();
  /**
   * @brief Updates the Battery's position
   * @param dt Delta time
   */
  void update(double dt);
};

#endif
