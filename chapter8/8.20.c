#include<unistd.h>

int main(int argc, char* argv) {
    if(execve("/bin/ls", argv, env) == -1) {
        printf("execve error");
        exit(1);
    }
    return 0;
}