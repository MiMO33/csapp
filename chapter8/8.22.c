#include<unistd.h>

int mysystem(char* command) {

    pid_t pid;
    int status;

    if((pid = fork()) == 0) {
        char* argv[4] = {"", "-c", command, NULL};
        execve("/bin//sh", argv, environ);
    }

    if(waitpid(pid, &status, 0) > 0) {
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        if (WIFSIGNALED(status))
            return WTERMSIG(status);
    }
}