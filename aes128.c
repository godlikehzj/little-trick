#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>
#include <openssl/aes.h>
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;

    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;

        highByte += 0x30;

        if (highByte > 0x39)
                dest[i * 2] = highByte + 0x07;
        else
                dest[i * 2] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}
char *getEncryptString(char *input)
{
    int inputlen = 0;
    int paddinglen = 0;
    int totallen = 0;

    inputlen = strlen(input);
    paddinglen = inputlen % 16;
    totallen = ((paddinglen == 0)? inputlen + 16:inputlen + 16 - paddinglen);
    char *out = malloc(totallen+1);
    if (out == NULL){
        return NULL;
    }
    strcpy(out, input);
    for (int i= inputlen; i <totallen; i++)
    {
        *(out + i) = totallen - inputlen;
    }
    *(out + totallen) =0;

    return out;
}

char *Aes_encrypt_ECB_PKCS5Padding(char *input, char *key){
    char *encrypt_string = NULL;
    int en_len = 0;
    int len = 0;
    char *in=NULL;
    char *out=NULL;
    AES_KEY aes;

    if (input == NULL || key == NULL){
        return NULL;
    }
    if (AES_set_encrypt_key(key, 128, &aes) < 0)
        printf("aes key failed\r\n");
    encrypt_string = getEncryptString(input);
    in = encrypt_string;
    len = strlen(encrypt_string);
    char *out_string = malloc(len * 2 +1);
    memset(out_string ,0, len*3+1);
    out = out_string;
    while(en_len<len)
    {
        AES_encrypt((unsigned char*)encrypt_string, (unsigned char*)out_string, &aes);
        encrypt_string+=AES_BLOCK_SIZE;
        out_string+=AES_BLOCK_SIZE;
        en_len+=AES_BLOCK_SIZE;
    }
    free(in);
    
    return out;
}
int main(){
  char *intemp = "70:2d:59:id:d1:b";

  char *pcKey = "1234567890987654";

  char outhex[1000];

  char *output2 = Aes_encrypt_ECB_PKCS5Padding(intemp, pcKey);
  ByteToHexStr(output2,outhex,strlen(output2));
  printf("encrypt data is %s\r\n", outhex);
  free(output2);
}
