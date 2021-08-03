#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int visit_cache_line(unsigned long addr);

typedef struct {
    int valid;
    int tag;
    int stamp;
}cache_line;


int h,v,s,E,b,B,S,t;
int hits , misses , evictions;
cache_line** cache;
FILE* fp = NULL;

void  updateStamp(int s,int E)
{
    for(int i = 0; i < 1<<s; ++i)
        for(int j = 0; j < E; ++j)
            if(cache[i][j].valid == 1)
                ++cache[i][j].stamp;
}

void print_usage()
{
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
            "Options:\n"
            "  -h         Print this help message.\n"
            "  -v         Optional verbose flag.\n"
            "  -s <num>   Number of set index bits.\n"
            "  -E <num>   Number of lines per set.\n"
            "  -b <num>   Number of block offset bits.\n"
            "  -t <file>  Trace file.\n\n"
            "Examples:\n"
            "  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
            "  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

int main(int argc, char* argv[])
{
    int opt;
    // int verbose_flag = 0;
    while(-1 != (opt = getopt(argc, argv, "hvs:E:b:t:"))) {
        switch (opt) {
        case 's':
            s = atoi(optarg);
            break;
        case 'h':
            print_usage(); break;
        case 'v':
             break;
        case 'E':
            E = atoi(optarg); break;
        case 'b':
            b = atoi(optarg); break;
        case 't':
			fp = fopen(optarg, "r"); break;
        }
    } 
    t = 64 - s - b;
    S = pow(2, s);
    B = pow(2, s);

    //init cache
    cache = ( cache_line**)malloc(sizeof(cache_line*) * S);
    for(int i=0;i<S;i++){
        cache[i]=( cache_line*)malloc(sizeof( cache_line)*E);
        for(int j=0;j<E;j++){
            cache[i][j].tag=-1;
            cache[i][j].stamp=-1;
            cache[i][j].valid=0;
        }
    }

    char buf[20], operation;
    unsigned long address;
    int size;

    while(fgets(buf, sizeof(buf), fp) != NULL) {
        if(buf[0] == 'I')
            continue;
        sscanf(buf, " %c %lx,%d", &operation, &address, &size);

        switch(operation) {
            case 'L': 
            case 'S':
                visit_cache_line(address);
                break;
            case 'M':
                visit_cache_line(address);
                visit_cache_line(address);
                break;
        }
        updateStamp(s, E);
    }

    printSummary(hits, misses, evictions);

    for (int i = 0; i < S; i++)
        free(cache[i]);
    free(cache);
    fclose(fp);
    return 0;
}

/**
 * return value
 * 0: hit
 * 1: miss
 * 2: cache miss, eviction
 */
int visit_cache_line(unsigned long addr) 
{
    int tag = addr >> (s + b);
    int index = (addr >> b) & ((-1U) >> (64 - s));
    int MaxStamp=INT_MIN;
    int MaxStamp_index=-1;
    //printf("%lx\n", ((~0UL)  >> (64 - s)));
    for(int i = 0; i < E; i++) {
        if(cache[index][i].tag == tag) {
            cache[index][i].stamp=0;
            hits++;
            return 0;
        }
    }

    for(int i = 0; i < E; i++) {
        if(cache[index][i].valid == 0) {
            cache[index][i].valid = 1;
            cache[index][i].tag = tag;
            cache[index][i].stamp=0;
            misses++;
            return 1;
        }
    }

    misses++;
    evictions++;
    for(int i = 0; i < E; ++i) {
        if(cache[index][i].stamp > MaxStamp) {
            MaxStamp = cache[index][i].stamp;
            MaxStamp_index = i;
        }
    }
    cache[index][MaxStamp_index].tag = tag;
    cache[index][MaxStamp_index].stamp = 0;
    return 2;
}
