#include <unistd.h>     /* getopt*/
#include <getopt.h>     /* getopt*/
#include <stdio.h>      /* fopen fscanf*/
#include <stdlib.h>     /* atoi callloc*/
#include <string.h>     /* strcpy*/
#include "cachelab.h"   /* printSummary*/

int time = 0;

typedef struct {
    long CI;
    long CO;
    long CT;
} address;

typedef struct {
    int tag;
    long address;
    long time_stamp;
}cache_line;

typedef struct {
    int s, E, b, t;
} cashe_arg;

typedef struct {
    long CI;
    long CT;
    long CO;
}addr_con;

long get_mask(int t) {
    long mask = 0;
    for(int i = 0; i < t; i++) {
        mask = (mask << 1) + 1;
    }
    return mask;
}

addr_con* split_addr(long addr_in, int s, int t, int b) {
    addr_con* ret = malloc(sizeof(addr_con));
    ret->CO = get_mask(b) & addr_in;
    ret->CI = ((get_mask(s) << b) & addr_in) >> b;
    ret->CT = ((get_mask(t) << (s + b)) & addr_in) >> (s + b);
    return ret;
}

int hit(cache_line* cache, addr_con* addr, int E, int t, int s) {

    for(int i = 0; i < E; i++) {
        if((cache[addr->CI * E + i]).tag == 1     &&
            cache[addr->CI * E + i].address == addr->CT) {
            return i;
        }
    }
    return -1;
}

int find_min(cache_line* cache, addr_con* addr, int E) {

    int ret = 0;
    for(int i = 0; i < E; i++) {
        if(cache[addr->CI + ret].time_stamp < cache[addr->CI + i].time_stamp) {
            ret = i;
        }
    }
    return ret;
}

int main(int argc, char** argv) {

    
    int opt, hits = 0, misses = 0, evictions = 0;
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

    cache_line* cache = (cache_line*) calloc(s * E, sizeof(cache_line));
    char access_type;
    long addr_in;
    int size;
    FILE *fp = fopen(file_path, "r");
    while(fscanf(fp, "%c %lx,%d", &access_type, &addr_in, &size) > 0) {

        addr_con* addr = split_addr(addr_in, s, t, b);
        switch (access_type) {
        case 'I':
            break;
        case 'M':
            if(flag_v) { printf("%c %lx,%d ", access_type, addr_in, size);}
            if(hit(cache, addr, E, s, t) == -1) {
                if(flag_v) printf("miss");
                misses++;
                int min = find_min(cache, addr, E);
                cache[addr->CI + min].tag = 1;
                cache[addr->CI + min].address = addr->CT;
                if(cache[addr->CI + min].time_stamp != 0) {
                    printf(" evictions");
                    evictions++;
                }
                cache[addr->CI + min].time_stamp = (++time);
                if(flag_v) printf(" hit");
                hits++;
                //printf("--->%ld\n", addr->CI + min);
                printf("\n");
                break;
            } else {
                if(flag_v) printf("hit hit\n");
                hits += 2;
                int i = hit(cache, addr, E, s, t);
                cache[addr->CI + i].time_stamp = (++time);
                printf("\n");
                break;
            }
            break;
        case 'S':
        case 'L':
            if(flag_v) {printf("%c %lx,%d ", access_type, addr_in, size);}
            if(hit(cache, addr, E, s, t) == -1) {
                if(flag_v) printf("miss");
                misses++;
                int min = find_min(cache, addr, E);
                cache[addr->CI + min].tag = 1;
                cache[addr->CI + min].address = addr->CT;
                if(cache[addr->CI + min].time_stamp != 0) {
                    printf(" evictions");
                    evictions++;
                }
                cache[addr->CI + min].time_stamp = (++time);
                //printf("--->%ld\n", addr->CI + min);
                break;
            } else {
                if(flag_v) printf("hit");
                hits++;
                int i = hit(cache, addr, E, s, t);
                cache[addr->CI + i].time_stamp = (++time);
                printf("\n");
                break;
            }
        }
    }
    free(cache);
    fclose(fp);
    printSummary(hits , misses, evictions);
    return 0;
}