#include "md5.hpp"

void printres(int* result){
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
    std::stringstream stream;
    stream << file.rdbuf(); 
    int *result = hash->md5sum(&stream);
    printres(result);
    return 0;
}