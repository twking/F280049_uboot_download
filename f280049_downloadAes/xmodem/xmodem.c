/*	
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* this code needs standard functions memcpy() and memset()
   and input/output functions _inbyte() and _outbyte().

   the prototypes of the input/output functions are:
     int32_t _inbyte(unsigned short timeout); // msec timeout
     void _outbyte(int32_t c);

 */

#include "crc16.h"
#include "xmodem.h"
#include <string.h>
#include "driverlib.h"


#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 5100
#define MAXRETRANS 25

void _outbyte(int16_t c)
{
        SCI_writeCharBlockingNonFIFO(SCIA_BASE, c);
}
int32_t _inbyte(unsigned short timeout) // msec timeout
{
        int16_t c=256;
        int32_t delay_nums=0;
        int32_t delay = timeout;
        while (c == 256)
        {

        	c=SCI_readCharNonBlocking(SCIA_BASE);
			if(delay_nums++>200)
			{
				delay_nums = 0;
				if (--delay == 0) return -1;
			}
        }
        return c & 0xFF;
}


static int32_t check(int32_t crc, const int16_t *buf, int32_t sz)
{
	if (crc) {
		unsigned short crc = crc16_ccitt(buf, sz);
		unsigned short tcrc = (buf[sz]<<8)+buf[sz+1];
		if (crc == tcrc)
			return 1;
	}
	else {
		int32_t i;
		int16_t cks = 0;
		for (i = 0; i < sz; ++i) {
			cks += buf[i];
		}
		if (cks == buf[sz])
		return 1;
	}

	return 0;
}

static void flushinput(void)
{
	while (_inbyte(((DLY_1S)*3)>>1) >= 0);
}

static int16_t xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */

int32_t xmodemReceive(uint16_t *dest, int32_t destsz)
{

	int16_t *p;
	int32_t bufsz, crc = 0;
	int16_t trychar = 'C';
	int16_t packetno = 1;
	int32_t i, c, len = 0;
	int32_t retry, retrans = MAXRETRANS;

	for(;;) {
		for( retry = 0; retry < 64; ++retry)
		{
			if (trychar) _outbyte(trychar);
			if ((c = _inbyte((DLY_1S))) >=0)
			{
				switch (c)
				{
                    case SOH:
                        bufsz = 128;
                        goto start_recv;
                    case STX:
                        bufsz = 1024;
                        goto start_recv;
                    case EOT:
                        flushinput();
                        _outbyte(ACK);
                        return len; /* normal end */
                    case CAN:
                        //delayms(5000);
                        if ((c = _inbyte(DLY_1S)) == CAN)
                        {
                            flushinput();
                            _outbyte(ACK);
                            return -1; /* canceled by remote */
                        }
                        break;
                    default:
                        break;
				}
			}
		}
		if (trychar == 'C') { trychar = NAK; continue; }
		flushinput();
		_outbyte(CAN);
		_outbyte(CAN);
		_outbyte(CAN);
		return -2; /* sync error */

	start_recv:
		if (trychar == 'C') crc = 1;
		trychar = 0;
		p = xbuff;
		*p++ = c;
		for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i)
		{
			if ((c = _inbyte(DLY_1S)) == -1)
			{
			    goto reject;
			}
			*p++ = c;
		}

		if (xbuff[1] == (0xff-xbuff[2])
			&& (xbuff[1] == packetno || xbuff[1] == packetno-1)
			&&check(crc, &xbuff[3], bufsz))
		{
			if (xbuff[1] == packetno)
			{
				register int32_t count = destsz - len;

				if (count > bufsz) count = bufsz;
				if (count == bufsz)
				{
					p = (int16_t *)&dest[len];
					for (i = 3;  i < count+3; i+=2)
                    {
					     *p++ = xbuff[i]|(xbuff[i+1]<<8);
                    }
					len += (count>>1);
				}
				else
				{
				    flushinput();
                    _outbyte(CAN);
                    _outbyte(CAN);
                    _outbyte(CAN);
                    return -5; /* too many retry error */
				}
				++packetno;
				packetno = packetno & 0xff;
				retrans = MAXRETRANS+1;
			}
			if (--retrans <= 0) {
				flushinput();
				_outbyte(CAN);
				_outbyte(CAN);
				_outbyte(CAN);
				return -3; /* too many retry error */
			}
			_outbyte(ACK);
			continue;
		}
	reject:
		flushinput();
		_outbyte(NAK);
	}
}


#ifdef TEST_XMODEM_RECEIVE
int32_t main(void)
{
	int32_t st;

	printf ("Send data using the xmodem protocol from your terminal emulator now...\n");
	/* the following should be changed for your environment:
	   0x30000 is the download address,
	   65536 is the maximum size to be written at this address
	 */
	st = xmodemReceive((char *)0x30000, 65536);
	if (st < 0) {
		printf ("Xmodem receive error: status: %d\n", st);
	}
	else  {
		printf ("Xmodem successfully received %d bytes\n", st);
	}

	return 0;
}
#endif

