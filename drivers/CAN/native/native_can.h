#ifndef H_NATIVE_CAN
#define H_NATIVE_CAN

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_0_EN 1
#define CAN_NUMOF 1
#define CAN_INTERFACE_NAME "can0"

#include "CAN/lowLevel_CAN_definition.h"
#include "can_Interface.h"

//typedef can::mode_t mode_t

class can_driver : public canInterface {
  private:
    uint8_t set_in_receive_mode(can_t canID);
    uint8_t set_in_transmit_mode(can_t canID);
  protected:

    /**
     * @breif Permet de modifier letat dun bus can
     * @param[in] canID Id identifiant le bus
     * @param[in] mode Letat dans lequel il doit etre
     */
    virtual void setMode(can_t canID, mode_t mode);

  public:
    can_driver();
    ~can_driver();

    /**
     * @brief Initialise le CAN
     * @param[in] recv_own_msgs Bool permetant de decider si l'on veux un feedback
     * @return              0 on success
     * @return              other on error
     */
    virtual uint8_t  init(bool recv_own_msgs);

    /**
     * @brief Same as /a init(bool recv_own_msgs)
     * @param[in] recv_own_msgs Bool permetant de decider si l'on veux un feedback
     * @param[in] canID Id identifiant le bus
     * @return              0 on success
     * @return              other on error
     */
    uint8_t  init(bool recv_own_msgs, can_t canID);

    /**
     * @brief Envois un message par le biais d'un bus CAN
     * @param[in] canID Id identifiant le bus
     * @param[in] message message to send
     * @return              return the number of byte send on success
     * @return              0 on error
     */
    uint8_t send_message(can_t canID, can_frame *message);

};

#ifdef __cplusplus
}
#endif

#endif /* H_NATIVE_CAN */
