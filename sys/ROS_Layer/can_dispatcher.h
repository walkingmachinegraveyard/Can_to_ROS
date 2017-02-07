#ifndef CAN_DISPATCHER_H_
#define CAN_DISPATCHER_H_



//static void _event_cb(can_t *dev, can_event_t event, uint8_t *message, uint16_t lenght);
void run (void);
void process_one(struct can_frame *frm);
void init_can_dispatcher(void);

#endif /*CAN_DISPATCHER_H_*/
