
#include <aes_decrypt/aes.h>
#include <stdlib.h>
#include "device.h"



#pragma DATA_SECTION(key,".aes_test");
uint8_t key[16];
#pragma DATA_SECTION(iv,".aes_test");
uint8_t iv[16];
#pragma DATA_SECTION(buff,".aes_test");
uint8_t buff[16];


#ifndef __cplusplus
#pragma CODE_SECTION(decrypt_data, ".TI.ramfunc");
#endif
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#endif
void decrypt_data(uint32_t UID,const uint16_t *pS,uint16_t *pD,uint16_t l)
{

    uint8_t randkey;
    uint16_t temp;
    uint16_t *pUID;
    struct AES_ctx ctx;
    uint32_t i,j;

    UID = (UID/7+782736)/24+26375;
    pUID = (uint16_t *)UID;

    srand(2008);
    for(i=0;i<16;i++)
    {
        temp = rand();
        srand(temp);
        randkey = temp%256;

        temp = *pUID++;

        key[i] = temp&0xff;
        iv[i] = (temp>>8)&0xff;

        key[i] = key[i]^randkey;
        iv[i] = iv[i]^randkey;

    }

    temp = UID&0xFFFF;
    key[3] = key[3]^(temp&0xff);
    iv[7]  = iv[7]^((temp>>8)&0xff);
    temp = (UID>>16)&0xFFFF;
    key[6] = key[6]^(temp&0xff);
    iv[11]  = iv[11]^((temp>>8)&0xff);

    AES_init_ctx_iv(&ctx, (const uint8_t *)key, (const uint8_t *)iv);

    for(i=0;i<l;i+=8)
    {
        for(j=0;j<8;j++)
        {
            temp = pS[j];
            buff[j<<1]   = temp&0xff;
            buff[(j<<1)+1] = (temp>>8)&0xff;
        }
        pS+=8;

        AES_CBC_decrypt_buffer(&ctx, buff, 16);

        for(j=0;j<8;j++)
        {
            pD[j] = buff[j<<1]+(buff[(j<<1)+1]<<8);
        }
        pD+=8;
        //SCI_writeCharBlockingNonFIFO(SCIA_BASE, i%256);
    }

}

#ifndef __cplusplus
#pragma CODE_SECTION(getCodeStatus, ".TI.ramfunc");
#endif
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#endif
uint16_t getCodeStatus(uint32_t UID,const uint16_t *pS)
{

    uint16_t *pUID;
    uint16_t i;
    uint16_t temp;
    UID = (UID/7+782736)/24+26375;
    pUID = (uint16_t *)UID;
    UID = (uint32_t)pS;
    pS = (uint16_t *)(UID&0xFFFFF000);
    for(i=0;i<16;i++)
    {
        temp = *pUID++;
        if(pS[i]!=(temp^((UID>>4)&0xffff)))
        {
            return 1;
        }
    }
    return 0;
}



