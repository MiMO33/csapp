#include <unistd.h>     /* getopt*/
#include <getopt.h>     /* getopt*/
#include <stdio.h>      /* fopen fscanf*/
#include <stdlib.h>     /* atoi*/
#include <string.h>     /* malloc strcpy*/
#include "cachelab.h"   /* printSummary*/

long time_count = 0;

long get_mask(int t) {
    long mask = 0;
    for(int i = 0; i < t; i++) {
        mask = (mask << 1) + 1;
    }
    return mask;
}

int get_min(long** timer, int CI, int E) {
    int ret = 0;
    for(int i = 0; i < E; i++) {
        if(timer[CI][ret] < timer[CI][i]) {
            ret = i;
        }
    }
    return ret;
}

int main(int argc, char** argv) {
    
    int opt;
    int s, E, b, t, flag_v = 0;
    char* file_path = (char*)malloc(sizeof(char) * 128);

    while(-1 != (opt = getopt(argc, argv, "hvs:E:b:t:"))) {
        switch (opt) {
        case 'h':
            printf("Usage:                                              \
                    ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>");
            break;
        case 'v':
            flag_v = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(file_path, optarg);
            break;
        default:
            printf("wrong argument\n");
            break;
        }
    }

    t = 64 - s - b;
    //init
    long** M = (long**)malloc(sizeof(long*) * s);
    for(int i = 0; i < s * E; i++) {
        M[i] = (long*)malloc(sizeof(long) * E);
        memset(M[i], 0,sizeof(long) * E);
    }

    long** timer = (long**)malloc(sizeof(long*) * s);
    for(int i = 0; i < s * E; i++) {
        timer[i] = (long*)malloc(sizeof(long) * E);
        memset(timer[i], 0,sizeof(long) * E);
    }
    char access_type;
    long address;
    int size;

    int miss, hit, evictions;
    long CI, CT;

    int flag_hit;
    long time_count;
    long mask = 0;

    FILE *fp = fopen(file_path, "r");
    while(1 != feof(fp)) {
        fscanf(fp, "%c %lx,%d", &access_type, &address, &size);

        flag_hit = 0;
        mask = get_mask(b);
        //int CO = mask & address;
        mask = get_mask(s);
        CI = (mask << b) & address;
        CI >>= t;
        mask = get_mask(64 - s - t);
        CT = (mask << (t + s)) & address;
        CT >>= t + s;

        switch (access_type) {
        case 'I':
            break;
        case 'M':
            
            break;
        case 'S':
        case 'L':
            if(flag_v) {printf("%c %lx,%d ", access_type, address, size);}
            for(int i = 0; i < E; i++) {
                if((timer[CI][i] == 1) && 
                   (((M[CI][i] >> t) & get_mask(s)) == CI)
                  ) {
                    //hit
                    flag_hit = 1;
                    break;
                }
            }
            if(flag_hit) {
                if(flag_v) printf("hit\n");
                hit++;
                break;
            }
            if(flag_v) printf("miss ");
            miss++;
            time_count++;
            int flag_set = 0;
            for(int i = 0; i < E; i++) {
                if(timer[CI][i] == 0) {
                    M[CI][i] = CT;
                    timer[CI][i] = time_count;
                    flag_set = 1;
                    printf("\n");
                    break;
                }
            }
            if(flag_set) break;
            int min = get_min(timer, CI, E);
            M[CI][min] = CT;
            timer[CI][min] = time_count;
            evictions++;
            if(flag_v) printf("evictions\n");
            break;
        }
    }
    printSummary(hit, miss, evictions);
    fclose(fp);
    free(M);
    free(timer);
    return 0;
}
