#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"

typedef enum
{

	enFlag = 0x35,
	rstFlag = 0x55

}e_state;

#define MASTERNUM 0x1F


#define SLAVE_MAXNUM 25
#define ANGLE_CHECK_NUM 25   //每个倾角从机下属的倾角传感器数量

#define ANGLE_TYPE 2

#endif
