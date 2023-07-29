#include <iostream>
#include <list>

using namespace std;

class Hash {
private:
    constexpr static int shift[] = { 
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };
    constexpr static unsigned int sequence[] = { 
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 
    };
public:
    Hash() {}

    void msgcpy(void *dst, void *msg, size_t n){
    char *s = (char*)msg;
    char *d = (char*)dst;
    for(int i = 0; i < n; i++) d[i] = s[i];
}

    size_t msgtam(char *str){
        size_t i = 0;
        if(str == NULL) return 0;
        while(str[i] != 0) i++;
        return i;
    }

    void bffzero(char *bff, size_t n){
        size_t i;
        for(i = 0; i < n; i++) bff[i] = 0;
    }

    unsigned int leftrotate(unsigned int x, unsigned int c){
        return (((x) << (c)) | ((x) >> (32 - (c))));
    }

    void step(int *wd, int *a0, int *b0, int *c0, int *d0){
        /*printf("offset: %d %x\n", offset, offset);

        int j;
        for(j =0; j < 64; j++) printf("%x ", ((char *) wd)[j]);
        printf("\n");*/
        int F, G;
        int A = *a0;
        int B = *b0;
        int C = *c0;
        int D = *d0;
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
            //printf("f=%x g=%d w[g]=%x\n", F, G, wd[G]);
             //printf("rotateLeft(%x + %x + %x + %x, %d)\n", A, F, k[i], wd[G], s[i]);
            F = A + F + sequence[i] + wd[G];
            A = D;
            D = C;
            C = B;
            B = B + leftrotate(F, shift[i]);
        }
        *a0 += A;
        *b0 += B;
        *c0 += C;
        *d0 += D;
    }

    void md5(char *msg){
    int a0 = 0x67452301;   //A
    int b0 = 0xefcdab89;   //B
    int c0 = 0x98badcfe;   //C
    int d0 = 0x10325476;   //D
    u_int64_t msglen, bitslen;
    int len;
    char *hashmsg;
    msglen = msgtam(msg);
    len = msglen;
    bitslen = 8*msglen;
    len = bitslen + 1;
    while( len%512 != 448 ) len++;
    len = len/8;
    hashmsg = (char*) malloc(sizeof(char)*(len+64));
    
    bffzero(hashmsg + msglen, len-msglen);
    msgcpy(hashmsg, msg, msglen);
    hashmsg[msglen] = 0b10000000;
    msgcpy(hashmsg + len, &bitslen, 8);
    
    int offset; 
    for(offset = 0; offset < len; offset += 64 ){
        step((int *) (msg + offset), &a0, &b0, &c0, &d0);
    }
    

    
    unsigned char *p;

    // display result
    p=(unsigned char *)&a0;
    printf("%.2x%.2x%.2x%.2x", p[0], p[1], p[2], p[3]);

    p=(unsigned char *)&b0;
    printf("%.2x%.2x%.2x%.2x", p[0], p[1], p[2], p[3]);

    p=(unsigned char *)&c0;
    printf("%.2x%.2x%.2x%.2x", p[0], p[1], p[2], p[3]);

    p=(unsigned char *)&d0;
    printf("%.2x%.2x%.2x%.2x\n", p[0], p[1], p[2], p[3]);
}
};

int main() {
    Hash *hash = new Hash();
    hash->md5((char*) "The earliest known appearance of the phrase was in The Boston Journal. In an article titled Current Notes in the February 9, 1885, edition, the phrase is mentioned as a good practice sentence for writing students: A favorite copy set by writing teachers for their pupils is the following, because it contains every letter of the alphabet: 'A quick brown fox jumps over the lazy dog.'[1] Dozens of other newspapers published the phrase over the next few months, all using the version of the sentence starting with A rather than The.[2] The earliest known use of the phrase starting with The is from the 1888 book Illustrative Shorthand by Linda Bronson.[3] The modern form (starting with The) became more common even though it is slightly longer than the original (starting with A). ");
    return 0;

}