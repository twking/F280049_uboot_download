
#include <aes_encrypt/aes.h>
#include <stdlib.h>

uint8_t key[16];
uint8_t iv[16];
uint8_t buff[16];

void encrypt_data(uint32_t UID,const uint16_t *pS,uint16_t *pD,uint16_t l)
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

        AES_CBC_encrypt_buffer(&ctx, buff, 16);

        for(j=0;j<8;j++)
        {
            pD[j] = buff[j<<1]+(buff[(j<<1)+1]<<8);
        }
        pD+=8;
        //UARTprintf ("%d\r\n",i);
    }
}

void SetCodeStatus(uint32_t UID,const uint16_t *pS,uint16_t *pD)
{

    uint16_t *pUID;
    uint16_t i;
    UID = (UID/7+782736)/24+26375;
    pUID = (uint16_t *)UID;
    UID = (uint32_t)pS;
    for(i=0;i<16;i++)
    {
         pD[i] = pUID[i]^((UID>>4)&0xffff);
    }
}

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




