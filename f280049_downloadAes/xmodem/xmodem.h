/*
 * xmodem.h
 *
 *  Created on: 2019-1-11
 *      Author: Administrator
 */

#ifndef XMODEM_H_
#define XMODEM_H_

#include "stdint.h"

int32_t xmodemReceive(uint16_t *dest, int32_t destsz);

#endif /* XMODEM_H_ */
