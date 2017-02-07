



/*
                /register_CAN //used to keep the data
can_to_regiser-<
                \interpreteur_CAN//used to procces the data from the can
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//#define USE_CURSE


#include <string.h>
#include "ROS_Layer/can_dispatcher.h"

#if defined( USE_CURSE )
	#include <curses.h>
#endif

can_dispatcher *myCAN_To_ROS_interfface;


int main(int argc, char **argv)
{
	/*if (argc != 2)
	{
		printf("Syntax: %s <Arbitration ID>\n", argv[0]);
		exit(1);
	}*/
    myCAN_To_ROS_interfface = new can_dispatcher();
	
    while(1)//TODO le reste du code.
    {
	//run();
    }

	return 0;
}
