#ifndef H_INTERPRETEUR_CAN
#define H_INTERPRETEUR_CAN

#include <stdint.h>
#include <linux/can.h>

#ifdef __cplusplus
extern "C" {
#endif




struct can_message_t
 {
  uint8_t       message_number; /* Identifiant diferentiant les messages*/
  uint8_t 	dataType;/* Id for CAN message data type. */		//TODO: a definire
  uint8_t 	service_Code;/* Service code, see CAN WM specification. */ //TODO: a definire
  uint8_t 	message_Code;/* Message code, see CAN WM specification. */ //TODO: a definire
  uint8_t 	lenght;/* the lenght of the data */ //can_dlc-(uint8_t*3)
  uint8_t* 	data;/* the message gived *///TODO: a definire
};


uint32_t get_canFrame_id(struct can_frame *frm);

struct can_message_t	parse_receive_message(struct can_frame *frm);
struct can_frame  parse_send_message(struct can_message_t* frm);

#ifdef __cplusplus
}
#endif

#endif //H_INTERPRETEUR_CAN
