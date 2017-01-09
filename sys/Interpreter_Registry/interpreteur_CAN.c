
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
    
    //TODO

    return result;
}

struct can_frame  parse_send_message(struct can_message_t* frm)
{
    struct can_frame result;
    
    //TODO

    return result;
}

