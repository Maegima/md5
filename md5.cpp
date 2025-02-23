#include "md5.hpp"

Hash::Hash() {}

Digest Hash::md5(std::stringstream* stream){
    return md5(stream->str().c_str());
}

Digest Hash::md5(fstream *msg, u_int64_t size){
    Digest digest(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
    u_int64_t msglen = size;
    u_int64_t bitslen = 8*msglen;

    u_int64_t offset = 0;
    char *buff = (char *) malloc(sizeof(char)*64);
    if(msglen > 63){
        for(offset = 0; offset < msglen - 63; offset += 64 ){
            msg->read(buff, 64);
            digest += md5step((unsigned int *) buff, digest);
        }
    }
    bffzero(buff, 64);
    msg->read(buff, msglen - offset);
    buff[msglen - offset] = 0b10000000;
    if(msglen - offset > 55){
        digest += md5step((unsigned int *) buff, digest);
        bffzero(buff, 56);
    }
    bffcpy(buff + 56, &bitslen, 8);
    digest += md5step((unsigned int *) buff, digest);
    free(buff);
    return digest;
}

Digest Hash::md5(const char *msg){
    Digest digest(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
    int msglen = strsize(msg);
    u_int64_t bitslen = 8*msglen;

    int offset; 
    for(offset = 0; offset < msglen - 63; offset += 64 ){
        digest += md5step((unsigned int *) (msg + offset), digest);
    }
    
    char *lastmsg = (char*) malloc(sizeof(char)*64);
    bffzero(lastmsg, 64);
    bffcpy(lastmsg, (void *) (msg + offset), msglen - offset);
    lastmsg[msglen - offset] = 0b10000000;
    if(msglen - offset > 55){
        digest += md5step((unsigned int *) lastmsg, digest);
        bffzero(lastmsg, 56);
    }
    bffcpy(lastmsg + 56, &bitslen, 8);
    digest += md5step((unsigned int *) lastmsg, digest);
    return digest;
}

Digest Hash::md5step(unsigned int *wd, const Digest &digest){
    unsigned int F, G;
    Digest step = digest;
    for(int i = 0; i<64; i++) {
        if( i < 16 ){
            F = (step.B & step.C) | ((~step.B) & step.D);
            G = i;
        }
        else if( i < 32 ){
            F = (step.D & step.B) | ((~step.D) & step.C);
            G = (5*i + 1)%16;
        }
        else if( i < 48 ){
            F = step.B ^ step.C ^ step.D;
            G = (3*i + 5)%16;
        }
        else{
            F = step.C ^ (step.B | (~step.D));
            G = (7*i)%16;
        }
        F += step.A + sequence[i] + wd[G];
        step.A = step.D;
        step.D = step.C;
        step.C = step.B;
        step.B = step.B + leftrotate(F, shift[i]);
    }
    return step;
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
