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

//use by can
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

//typedef can::mode_t mode_t

class can_driver : public canInterface {
  private:
    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
    uint8_t set_in_receive_mode(can_t canID);
    uint8_t set_in_transmit_mode(can_t canID);
    #endif

  protected:
    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
    /**
     * @breif Permet de modifier letat dun bus can
     * @param[in] canID Id identifiant le bus
     * @param[in] mode Letat dans lequel il doit etre
     */
    virtual void setMode(can_t canID, canInterface::mode_t mode);
    #endif
    /**
     * @note Unused for the moment
     * @brief Get mutually exclusive access to the given CAN bus
     *
     * In case the CAN device is busy, this function will block until the bus is free again.
     *
     * @param[in] dev       CAN device to access
     *
     * @return              0 on success
     * @return              -1 on error
     */
    virtual int can_acquire(can_t dev){return 0;};

    /**
     * @note Unused for the moment
     * @brief Release the given CAN device to be used by others
     *
     * @param[in] dev       CAN device to release
     *
     * @return              0 on success
     * @return              -1 on error
     */
    virtual int can_release(can_t dev){return 0;};

  public:
    can_driver(can_event_cb_t event_callback);
    ~can_driver();

    virtual void run(void);
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

    /**
     * @breif Fonction utiliser pour recevoir une fonction
     * @param[in] frm Frame contenant le format du message CAN
     * @param[out] buffer Buffer de taille /a CAN_MAX_DLEN pour aceuillire le message.
     */
    void process_one(struct can_frame *frm, uint8_t* buffer);

};

#ifdef __cplusplus
}
#endif

#endif /* H_NATIVE_CAN */
