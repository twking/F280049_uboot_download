/*
 * cmd.c
 *
 *  Created on: 2018-8-17
 *      Author: Administrator
 */

#include"cmd.h"
#include "string.h"
#include "uartStdio.h"


#define CMD_NUMS 4

#define CMD_MAX_LENGTH 15
char *uart0_cmd[CMD_NUMS]={"up_dsp","up_load","up_uboot","reboot"};
char *cmd_discp[CMD_NUMS]={"up dsp code","up download code","up_uboot","reboot"};
static char uart0_CMD[CMD_MAX_LENGTH+1] = "";

int32_t gethelp(void)
{
	int32_t i;
	UARTprintf("S...:***********COMMAND INSTRUCYIONS**********\r\n");
	for(i=0;i<CMD_NUMS;i++)
	{
		UARTprintf("%d:%s===>%s\r\n",i+1,uart0_cmd[i],cmd_discp[i]);
	}
	UARTprintf("*******************END*******************\r\n");
	return 1;

}
int32_t getCMDlines(void)
{
	int32_t retVal;
	int32_t i;
	UARTprintf("Please put cmd \r\n");
	retVal = UARTGets(uart0_CMD,CMD_MAX_LENGTH);
	UARTprintf("\r\n");
	if(retVal>0)
	{
		for(i=0;i<CMD_NUMS;i++)
		{
			if(!strcmp((const char *)uart0_cmd[i], (const char *)uart0_CMD))
			{
				return i;
			}
		}
	}
	return -1;
}


