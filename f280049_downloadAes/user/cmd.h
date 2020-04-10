/*
 * cmd.h
 *
 *  Created on: 2018-8-17
 *      Author: Administrator
 */

#ifndef CMD_H_
#define CMD_H_


#define UP_DSP 0
#define UP_LOAD 1
#define UP_UBOOT 2
#define REBOOT 3


#include "stdint.h"

int32_t gethelp(void);
int32_t getCMDlines(void);

#endif /* CMD_H_ */
