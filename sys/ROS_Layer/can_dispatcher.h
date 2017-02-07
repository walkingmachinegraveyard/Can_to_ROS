#ifndef CAN_DISPATCHER_H_
#define CAN_DISPATCHER_H_

#include <linux/can.h>
#include "CAN/native_can.h"

#include "Registry_CAN/register_CAN.h"


class can_dispatcher: public register_can
{
    private:

   

    public:  
bool resend(uint32_t nodeId);
bool update (uint32_t nodeId, struct can_message_t message);
   can_driver * m_can_driver;
    uint32_t arb_id; 
   can_dispatcher();
    //can_dispatcher(can_event_cb_t event_callback);
//static void _event_cb(can_t *dev, can_event_t event, uint8_t *message , uint16_t lenght);
    
    void run (void);
  //static  void process_one(struct can_frame *frm);
    //void init_can_dispatcher(void);
};
static void /*can_dispatcher::*/_event_cb(can_t *dev, can_event_t event, uint8_t *message , uint16_t lenght);
 void process_one(struct can_frame *frm);

#endif /*CAN_DISPATCHER_H_*/
