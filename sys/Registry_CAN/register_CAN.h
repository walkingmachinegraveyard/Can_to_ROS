#ifndef H_REGISTER_CAN
#define H_REGISTER_CAN

#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>

#define MAX_CLIEN_CAN 10
#define MAX_MESSAGE_CAN 20

#include "Interpreter_Registry/interpreteur_CAN.h"

#define DEBUG
struct can_data_t {
  uint32_t 	nodeId; /*    	Id of transmitting/receiving node. */

  uint8_t counter_message;
  std::list<can_message_t> m_list_of_message;

};


class register_can {
private:
	register_can* self_ptr=0;
	std::list<can_data_t> m_list_of_client;
	uint8_t m_max_client;
    uint8_t m_max_messages;
    void set_max_client(uint8_t number);
    void set_max_messages(uint8_t number);
    
    
protected:
	can_data_t * get_client(uint32_t nodeId);
	can_message_t * get_message(uint32_t nodeId, uint8_t messageid);
public:
	register_can();
    void init();

	
    //TODO: print client(no param == all, else is boundry)
    #if defined(DEBUG)
    void print_list_clientId(void);
    #endif
	bool add_client(uint32_t nodeId);
    bool is_client_exist(uint32_t nodeId);
    //TODO: clean client(no param == all, else is boundry)
    void clean_client(void);

    
    //TODO: print message(no param == all, else is boundry)
	bool add_message(uint32_t nodeId, struct can_message_t message);
    //TODO: is_message_exist? (pas vraiment sure)
    //TODO: clean message(no param == all, else is boundry)
    void clean_message(uint32_t nodeId);

	//TODO:resend (en parametre le client et le numero du message a envoyer)
    //bool resend(uint32_t nodeId);
    //TODO:update (qui va permetre dajouter un message et ensuite dutiliser la funct. resend)
    bool update (uint32_t nodeId, struct can_message_t message);

    

};


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif //H_REGISTER_CAN
