#include <iostream>
#include <algorithm>
#include "global_pointer.h"
#include "register_CAN.h"


register_can::register_can()
{
    init();
}

void register_can::init()
{
    if (self_ptr == 0)
    {
        //First init
        self_ptr=this;

         m_list_of_client.clear();
        m_max_client = MAX_CLIEN_CAN;
        m_max_messages = MAX_MESSAGE_CAN;
    }
    //All other init
}

void register_can::clean_client(void)
{
    m_list_of_client.clear();
}

void register_can::clean_message(uint32_t nodeId)
{
    can_data_t * ptr_to_client = get_client(nodeId);

    if(ptr_to_client != 0 )
    {
        ptr_to_client->m_list_of_message.clear();
    }
}

void register_can::set_max_client(uint8_t number)
{
    m_max_client = number;
}

void register_can::set_max_messages(uint8_t number)
{
    m_max_messages = number;
}



bool register_can::add_client(uint32_t nodeId)
{
    bool result=false;

    if (!is_client_exist(nodeId))
    {
        m_list_of_client.emplace_back( can_data_t{nodeId,0} );
    }
    if(m_list_of_client.size() > m_max_client)
    {
        m_list_of_client.pop_front();
    }
    result=true;
    
    return result;
}


bool register_can::add_message(uint32_t nodeId,struct can_message_t message)
{
    bool result=false;
    
    can_data_t * ptr_to_client = get_client(nodeId);

    if(ptr_to_client != 0 )
    {
        message.message_number = ++ptr_to_client->counter_message;        

        ptr_to_client->m_list_of_message.push_back( message );
        
        if(ptr_to_client->m_list_of_message.size() > m_max_messages)
        {
         ptr_to_client->m_list_of_message.pop_front();
        }
        result=true;
    }
    
    return result;
}

bool register_can::resend(uint32_t nodeId)
{
    bool result=false;

    can_data_t * ptr_to_client = get_client(nodeId);
    
    if(ptr_to_client != 0 )
    {
        can_message_t* message_found = 0;
        message_found = get_message(nodeId,ptr_to_client->counter_message-1);
        if( message_found != 0)
        {
         //message trouver
         can_frame payload;

         payload = parse_send_message(message_found);

         m_can_driver->send_message(CAN_0,&payload);
        }
        result=true;
    }
    return result;
}
bool register_can::update (uint32_t nodeId, struct can_message_t message)
{
    bool result=false;

    can_data_t * ptr_to_client = get_client(nodeId);
    
    if(ptr_to_client != 0 )
    {
        result= add_message(nodeId, message);
        if(result)
        {
         resend(nodeId);
        }
    }
    return result;
}


bool register_can::is_client_exist(uint32_t nodeId)
{
    bool result=false;

    can_data_t * ptr_to_client = get_client(nodeId);

    if(ptr_to_client != 0 )
    {
        result = true;
    }
    
    return result;
}
can_data_t * register_can::get_client(uint32_t nodeId)
{
    can_data_t * result=0;
    for(std::list<can_data_t>::iterator it=m_list_of_client.begin();(it != m_list_of_client.end())&&(result==0) ; ++it)
    {
      if( (*it).nodeId == nodeId )
        {
            result = &(*it);
        }
    }
    
    return result;
}

can_message_t * register_can::get_message(uint32_t nodeId, uint8_t messageid)
{
    can_message_t * result=0; 
    can_data_t * ptr_to_client = 0;

    ptr_to_client = get_client(nodeId);

    if(ptr_to_client != 0 )
    {
        for(std::list<can_message_t>::iterator it=ptr_to_client->m_list_of_message.begin();(it != ptr_to_client->m_list_of_message.end())&&(result==0) ; ++it)
        {
          if( (*it).message_number == messageid )
            {
                result = &(*it);
            }
        }
    }
    return result;
}

#if defined(DEBUG)
    void register_can::print_list_clientId(void)
    {
        for(std::list<can_data_t>::iterator it=m_list_of_client.begin();(it != m_list_of_client.end()) ; ++it)
        {
          std::cout <<  (*it).nodeId << '\n';
        }
    }
#endif
