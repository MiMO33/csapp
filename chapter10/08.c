#include "../include/csapp.h"

int main(int args, char** argv)
{
        struct stat stat;
        char* type, *readok;

        Fstat( atoi(argv[1]), &stat);
        if (S_ISREG(stat.st_mode))
                type = "regular";
        else if (S_ISDIR(stat.st_mode))
                type = "dir";
        else
                type = "other";
        if ((stat.st_mode & S_IRUSR))
                readok = "yes";
        else
                readok = "no";
        
        printf("type: %s, read: %s\n", type, readok);
        return 0;
}