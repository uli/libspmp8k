//////////////////////////////////////////////////////////////////////////
/*
    Provided by ������, Northeastern University (www.neu.edu.cn)
    Email: blackdrn@sohu.com
	This product is free for use.
*/
//////////////////////////////////////////////////////////////////////////

#include <stdint.h>

enum    {ENCRYPT,DECRYPT};
//enum    uint8_t{false,true}; // if uint8_t is not supported,use this or just replace with char
                            // and use 1 for true,0 for false;
//////////////////////////////////////////////////////////////////////////

// Type��ENCRYPT:����,DECRYPT:����
// ���������(Out)�ĳ��� >= ((datalen+7)/8)*8,����datalen�������8�ı�������С������
// In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
// ��keylen>8ʱϵͳ�Զ�ʹ��3��DES��/����,����ʹ�ñ�׼DES��/����.����16�ֽں�ֻȡǰ16�ֽ�

int Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,uint8_t Type = ENCRYPT);

//////////////////////////////////////////////////////////////////////////

