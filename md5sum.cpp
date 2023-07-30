#include "md5.hpp"
#include <sys/stat.h>

void printres(unsigned int* result){
    unsigned char *p;

    // display result
    p=(unsigned char *) result;
    printf(
        "%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x\n", 
        p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
        p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]
    );
}

int main(int argc, char **argv) {
    Hash *hash = new Hash();
    fstream file = fstream(argv[1]);
    struct stat f_stat;
    stat(argv[1], &f_stat);
    unsigned int *result = hash->md5(&file, f_stat.st_size);
    printres(result);
    return 0;
}