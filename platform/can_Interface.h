#ifndef H_CAN_INTERFACE
#define H_CAN_INTERFACE

#ifdef __cplusplus
extern "C" {
#endif

#include "CAN/lowLevel_CAN_definition.h"
namespace can {

  class canInterface
  {
  protected:

  /**
   * @brief Get mutually exclusive access to the given SPI bus
   *
   * In case the SPI device is busy, this function will block until the bus is free again.
   *
   * @param[in] dev       SPI device to access
   *
   * @return              0 on success
   * @return              -1 on error
   */
  virtual int can_acquire(can_t dev);

  public:
    canInterface();
    ~canInterface();


   /**
    * @brief Initialise le CAN
    * @param[in] recv_own_msgs Bool permetant de decider si l'on veux un feedback
    */
   virtual void  init(bool recv_own_msgs)=0;


  };

} //namespace can

#ifdef __cplusplus
}
#endif

#endif //H_CAN_INTERFACE
