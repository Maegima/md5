#include "md5.hpp"

Hash::Hash() {}

unsigned int* Hash::md5(std::stringstream* stream){
    return md5(stream->str().c_str());
}

unsigned int* Hash::md5(fstream *msg, u_int64_t size){
    unsigned int *result = (unsigned int*) malloc(sizeof(unsigned int)*4);
    result[0] = 0x67452301;   //A
    result[1] = 0xefcdab89;   //B
    result[2] = 0x98badcfe;   //C
    result[3] = 0x10325476;   //D
    u_int64_t msglen = size;
    u_int64_t bitslen = 8*msglen;

    u_int64_t offset = 0;
    char *buff = (char *) malloc(sizeof(char)*64);
    if(msglen > 63){
        for(offset = 0; offset < msglen - 63; offset += 64 ){
            msg->read(buff, 64);
            md5step((unsigned int *) buff, result);
        }
    }
    bffzero(buff, 64);
    msg->read(buff, msglen - offset);
    buff[msglen - offset] = 0b10000000;
    if(msglen - offset > 55){
        md5step((unsigned int *) buff, result);
        bffzero(buff, 56);
    }
    bffcpy(buff + 56, &bitslen, 8);
    md5step((unsigned int *) buff, result);
    free(buff);
    return result;
}

unsigned int* Hash::md5(const char *msg){
    unsigned int *result = (unsigned int*) malloc(sizeof(unsigned int)*4);
    result[0] = 0x67452301;   //A
    result[1] = 0xefcdab89;   //B
    result[2] = 0x98badcfe;   //C
    result[3] = 0x10325476;   //D
    int msglen = strsize(msg);
    u_int64_t bitslen = 8*msglen;

    int offset; 
    for(offset = 0; offset < msglen - 63; offset += 64 ){
        md5step((unsigned int *) (msg + offset), result);
    }
    
    char *lastmsg = (char*) malloc(sizeof(char)*64);
    bffzero(lastmsg, 64);
    bffcpy(lastmsg, (void *) (msg + offset), msglen - offset);
    lastmsg[msglen - offset] = 0b10000000;
    if(msglen - offset > 55){
        md5step((unsigned int *) lastmsg, result);
        bffzero(lastmsg, 56);
    }
    bffcpy(lastmsg + 56, &bitslen, 8);
    md5step((unsigned int *) lastmsg, result);
    return result;
}

void Hash::md5step(unsigned int *wd, unsigned int *result){
    unsigned int F, G;
    unsigned int A = result[0];
    unsigned int B = result[1];
    unsigned int C = result[2];
    unsigned int D = result[3];
    for(int i = 0; i<64; i++) {
        if( i < 16 ){
            F = (B & C) | ((~B) & D);
            G = i;
        }
        else if( i < 32 ){
            F = (D & B) | ((~D) & C);
            G = (5*i + 1)%16;
        }
        else if( i < 48 ){
            F = B ^ C ^ D;
            G = (3*i + 5)%16;
        }
        else{
            F = C ^ (B | (~D));
            G = (7*i)%16;
        }
        F += A + sequence[i] + wd[G];
        A = D;
        D = C;
        C = B;
        B = B + leftrotate(F, shift[i]);
    }
    result[0] += A;
    result[1] += B;
    result[2] += C;
    result[3] += D;
}

size_t Hash::strsize(const char *str){
    size_t i = 0;
    if(str == NULL) return 0;
    while(str[i] != 0) i++;
    return i;
}

void Hash::bffcpy(void *dst, void *src, size_t n){
    char *s = (char*)src;
    char *d = (char*)dst;
    for(size_t i = 0; i < n; i++) d[i] = s[i];
}

void Hash::bffzero(char *bff, size_t n){
    for(size_t i = 0; i < n; i++) bff[i] = 0;
}

unsigned int Hash::leftrotate(unsigned int x, unsigned int c){
    return (((x) << (c)) | ((x) >> (32 - (c))));
}
