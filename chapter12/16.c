#include <pthread.h>
#include <stdio.h>

void *thread(void *vargp);

volatile long cnt = 0;

int main(int argc, char* argv[]) {
    if(argc != 2)
        printf("Usage: %s <n> \n", argv[0]);
    for(int i = 0; i < atoi(argv[1]); i++){
        pthread_t tid;
        pthread_create(&tid, NULL, thread, NULL);
    }
    pthread_exit(NULL);
}

void* thread(void *vargp) {
    printf("Hello, world\n");
    return NULL;
}
