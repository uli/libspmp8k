//////////////////////////////////////////////////////////////////////////
/*
    Provided by 王俊川, Northeastern University (www.neu.edu.cn)
    Email: blackdrn@sohu.com
	This product is free for use.
*/
//////////////////////////////////////////////////////////////////////////

#include <stdint.h>

enum    {ENCRYPT,DECRYPT};
//enum    uint8_t{false,true}; // if uint8_t is not supported,use this or just replace with char
                            // and use 1 for true,0 for false;
//////////////////////////////////////////////////////////////////////////

// Type—ENCRYPT:加密,DECRYPT:解密
// 输出缓冲区(Out)的长度 >= ((datalen+7)/8)*8,即比datalen大的且是8的倍数的最小正整数
// In 可以= Out,此时加/解密后将覆盖输入缓冲区(In)的内容
// 当keylen>8时系统自动使用3次DES加/解密,否则使用标准DES加/解密.超过16字节后只取前16字节

int Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,uint8_t Type = ENCRYPT);

//////////////////////////////////////////////////////////////////////////

