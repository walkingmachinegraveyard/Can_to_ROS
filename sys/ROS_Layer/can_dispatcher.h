#ifndef CAN_DISPATCHER_H_
#define CAN_DISPATCHER_H_

#include <linux/can.h>
#include "CAN/native_can.h"
//use to get can_t

class can_dispatcher/*: private canInterface*/
{
    
    public:  
    can_dispatcher();
    //can_dispatcher(can_event_cb_t event_callback);
static void _event_cb(can_t *dev, can_event_t event, uint8_t *message , uint16_t lenght);
    
    void run (void);
  static  void process_one(struct can_frame *frm);
    //void init_can_dispatcher(void);
};

#endif /*CAN_DISPATCHER_H_*/
