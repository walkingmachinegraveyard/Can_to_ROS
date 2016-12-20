#ifndef H_INTERPRETEUR_CAN
#define H_INTERPRETEUR_CAN

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>



struct can_message_t {
  uint8_t 	dataType, /* Id for CAN Aerospace message data type. */

  uint8_t 	serviceCode, /* Service code, see CAN Aerospace specification. */

  uint8_t 	messageCode, /*   	Message code, see CAN WM specification. */

  uint8_t* 	lenght, /* the lenght of the data */

  uint8_t* 	data, /* the message gived */
};


#ifdef __cplusplus
}
#endif

#endif //H_INTERPRETEUR_CAN
