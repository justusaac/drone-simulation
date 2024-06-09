#ifndef IOBSERVER_H_
#define IOBSERVER_H_

#include "util/json.h"

#include "math/vector3.h"

/**
 * @brief Interface for observers that can receive messages
 **/
class IObserver{
 public:
  /**
   * @brief Receive a message
   * @param message The message that was received
   **/
  virtual void relayMessage(std::string message) = 0;
};


#endif
