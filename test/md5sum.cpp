#include "../md5.hpp"
#include <sys/stat.h>
#include <iostream>

int main(int argc, char **argv) {
    Hash *hash = new Hash();
    fstream file = fstream(argv[1]);
    struct stat f_stat;
    stat(argv[1], &f_stat);
    Digest result = hash->md5(&file, f_stat.st_size);
    std::cout << (std::string) result << std::endl;
    return 0;
}