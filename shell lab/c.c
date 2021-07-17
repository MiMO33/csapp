
#include <stdio.h>
#include <string.h>
int builtin_cmd(char **argv) 
{
    if(strcmp(argv[0], "quit") == 0) {
        exit(0);
    } else if((strcmp(argv[0], "bg") == 0) || (strcmp(argv[0], "fg") == 0)){
        do_bgfg(argv);
    } else if(strcmp(argv[0], "jobs") == 0) {
        listjobs(jobs);
    } else {
        return 0;
    }
    return 1;
}
int main(void)
{
    assert(builtin_cmd(["bg"]) == 1);
    return 0;
}