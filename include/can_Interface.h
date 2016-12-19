#ifndef H_CAN_INTERFACE
#define H_CAN_INTERFACE



#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CAN/lowLevel_CAN_definition.h"

#if defined( WITH_NAMESPACE_ )
namespace can {
#endif

	typedef enum  {
        CAN_EVENT_RX_COMPLETE,
       } can_event_t;

  class canInterface
  {
  public:
    //TODO: definir les term de can_event_t
    // se fier a https://github.com/RIOT-OS/RIOT/blob/master/drivers/include/net/netdev2.h
    

    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
      enum class mode_t : uint8_t {receive, transmit};
    #endif
    /**
     * @brief   Event callback for signaling event to upper layers
     *
     * @param[in] type          type of the event
     */
    typedef void (*can_event_cb_t)(can_t *dev, can_event_t event, uint8_t *message, uint16_t lenght);

  private:


  protected:
    int frame_cnt[CAN_NUMOF];
    int valid_frame_cnt[CAN_NUMOF];

    //#if defined(CAN_NUMOF)
    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
      mode_t m_current_mode[CAN_NUMOF];
    #endif
    uint16_t m_socket[CAN_NUMOF];
    /*#else
    mode_t m_current_mode[];
    #endif*/
#if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
  /**
   * @breif Permet de modifier letat dun bus can
   * @param[in] canID Id identifiant le bus
   * @param[in] mode Letat dans lequel il doit etre
   */
  virtual void setMode(can_t canID, mode_t mode)=0;
#endif
  /**
   * @brief Get mutually exclusive access to the given CAN bus
   *
   * In case the CAN device is busy, this function will block until the bus is free again.
   *
   * @param[in] dev       CAN device to access
   *
   * @return              0 on success
   * @return              -1 on error
   */
  virtual int can_acquire(can_t dev);

  /**
   * @brief Release the given CAN device to be used by others
   *
   * @param[in] dev       CAN device to release
   *
   * @return              0 on success
   * @return              -1 on error
   */
  virtual int can_release(can_t dev);

  can_event_cb_t m_event_callback;

  public:
    canInterface(can_event_cb_t event_callback){m_event_callback = event_callback;};
    ~canInterface();
    virtual void run()=0;


   /**
    * @brief Initialise le CAN
    * @param[in] recv_own_msgs Bool permetant de decider si l'on veux un feedback
    * @return              0 on success
    * @return              other on error
    */
   virtual uint8_t  init(bool recv_own_msgs)=0;

  };

  #if defined( WITH_NAMESPACE_ )
} //namespace can
#endif


#ifdef __cplusplus
}
#endif

#endif //H_CAN_INTERFACE
