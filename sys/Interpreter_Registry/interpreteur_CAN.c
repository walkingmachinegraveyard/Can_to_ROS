
#include "interpreteur_CAN.h"

uint32_t get_canFrame_id(struct can_frame *frm)
{
    //note prise de can.h
    /*
    * Controller Area Network Identifier structure
    *
    * bit 0-28     : CAN identifier (11/29 bit)
    * bit 29       : error message frame flag (0 = data frame, 1 = error message)
    * bit 30       : remote transmission request flag (1 = rtr frame)
    * bit 31       : frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
    */
    uint32_t result;
    result = (frm->can_id)&0x1FFFFFFF;//use to remove la unwanted bit

    return result;
}

struct can_message_t	parse_receive_message(struct can_frame *frm)
{
    struct can_message_t result;
    //uint8_t * m_ptr=(uint8_t*)frm;
    
    result.message_number=0;
    //result.dataType =
    //result.service_Code =
    //result.message_Code =
    result.lenght = frm->can_dlc-3; //TODO: check if it work

    for(uint8_t i=0; i!=result.lenght; i++)
    {
        result.data[i] = frm->data[i];
    }

    return result;
}

struct can_frame  parse_send_message(struct can_message_t* frm)
{
    struct can_frame result;
    //uint8_t * m_ptr=(uint8_t*)frm;
        
    //result.message_number=0;
    //result.dataType =
    //result.service_Code =
    //result.message_Code =
    result.can_dlc = frm->lenght +3; //TODO: check if it work

    for(uint8_t i=0; i!=frm->lenght; i++)
    {
        result.data[i] = frm->data[i];
    }

    return result;
}
#if 0
struct can_message_t
 {
  uint8_t       message_number; /* Identifiant diferentiant les messages*/
  uint8_t 	dataType;/* Id for CAN message data type. */		//TODO: a definire
  uint8_t 	service_Code;/* Service code, see CAN WM specification. */ //TODO: a definire
  uint8_t 	message_Code;/* Message code, see CAN WM specification. */ //TODO: a definire
  uint8_t 	lenght;/* the lenght of the data */ //can_dlc-(uint8_t*3)
  uint8_t* 	data;/* the message gived *///TODO: a definire
};

      canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
105         __u8    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
106         __u8    __pad;   /* padding */
107         __u8    __res0;  /* reserved / padding */
108         __u8    __res1;  /* reserved / padding */
109         __u8    data[CAN_MAX_DLEN] __attribute__((aligned(8)));
#endif

