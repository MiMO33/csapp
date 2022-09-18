#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";


typedef struct ReqLine {
    char host[255];
    char port[64];
    char path[255];
} ReqLine;

typedef struct ReqHeader {
    char name[255];
    char value[255];
} ReqHeader;

void parse_request(rio_t* rio, ReqLine* req_line, ReqHeader req_headers[], int* num_headers);
int send_to_server(ReqLine *line, ReqHeader *headers, int num_hds);
ReqHeader* parse_header(char* buff);
void parse_uri(char* uri, ReqLine *linep);



void doit(int fd) {
    rio_t rio;
    Rio_readinitb(&rio, fd);
    ReqLine req_line;
    ReqHeader headers[255];
    int num_headers;
    parse_request(&rio, &req_line, headers, &num_headers);

    char buff[MAXLINE];
    int n = 0;
    int connfd = send_to_server(&req_line, headers, num_headers);

    rio_t server_rio;
    Rio_readinitb(&server_rio, connfd);

    //printf("here\n");
    while ((n = Rio_readlineb(&server_rio, buff, MAXLINE)) != 0) {
        Rio_writen(fd, buff, n);
        // strcpy(object_buf + total_size, buff);
        // total_size += n;
    }
    Close(connfd);
    printf("return\n");
}

void parse_request(rio_t* rio, ReqLine* req_line, ReqHeader req_headers[], int* num_headers) {
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char buff[MAXBUF] = {0};
    if (!Rio_readlineb(rio, buff, MAXLINE))
        return;
    sscanf(buff, "%s %s %s", method, uri, version);
    parse_uri(uri, req_line);
    *num_headers = 0;
    Rio_readlineb(rio, buff, MAXLINE);
    while(strcmp(buff, "\r\n")) {
        ReqHeader* res = parse_header(buff);
        req_headers[*num_headers] = *res;
        ++(*num_headers);
        Rio_readlineb(rio, buff, MAXLINE);
    }
}

ReqHeader* parse_header(char* buff) {
    struct ReqHeader* res = malloc(sizeof(ReqHeader));
    char* c = (char*)strstr(buff, ": ");
    *c = '\0';
    strcpy(res->name, buff);
    buff = c + 2;
    strcpy(res->value, buff);
    return res;
}

int send_to_server(ReqLine *line, ReqHeader *headers, int num_hds) {
    int clientfd;
    char buf[MAXLINE], *buf_head = buf;
    rio_t rio;

    clientfd = Open_clientfd(line->host, line->port);
    if (clientfd <= 0) unix_error("send to server\n");
    Rio_readinitb(&rio, clientfd);
    sprintf(buf_head, "GET %s HTTP/1.0\r\n", line->path);
    buf_head = buf + strlen(buf);
    for (int i = 0; i < num_hds; ++i) {
        sprintf(buf_head, "%s: %s", headers[i].name, headers[i].value);
        buf_head = buf + strlen(buf);
    }
    sprintf(buf_head, "\r\n");
    Rio_writen(clientfd, buf, MAXLINE);

    return clientfd;
}

// void parse_uri(char* uri, ReqLine *linep) {
//     if (strstr(uri, "http://") != uri) unix_error("invalid uri.\n");
//     uri += strlen("http://");
//     char *c = strstr(uri, "/");
//     *c = '\0';
//     strcpy(linep->host, uri);
//     *c = '/';
//     strcpy(linep->path, c);
//     strcpy(linep->port, "80");
// }

void parse_uri(char *uri, ReqLine* linep) {
    strcpy(linep->port, "80");
    char* pos = strstr(uri,"//");
    pos = pos!=NULL? pos+2:uri;
    int port = 80;
    char*pos2 = strstr(pos, ":");
    if(pos2!=NULL)
    {
        printf("%s\n", pos2);
        *pos2 = '\0';
        sscanf(pos, "%s", linep->host);
        sscanf(pos2+1,"%d%s", &port, linep->path);
        sprintf(linep->port, "%d", port);
    }
    else
    {
        pos2 = strstr(pos,"/");
        if(pos2!=NULL)
        {
            *pos2 = '\0';
            sscanf(pos,"%s", linep->host);
            *pos2 = '/';
            sscanf(pos2,"%s",linep->path);
        }
        else
        {
            sscanf(pos,"%s", linep->host);
        }
    }
    return;
}


void* thread(void* vargp) {
    int connfd = *((int * )vargp);
    Pthread_detach(pthread_self());
    doit(connfd);
    Close(connfd);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(EXIT_FAILURE);
    }
    signal(SIGPIPE, SIG_IGN);
    int listen_fd = Open_listenfd(argv[1]);
    int connect_fd;
    pthread_t tid;
    while(1) {
        struct sockaddr clientaddr;
        socklen_t len = sizeof(clientaddr);
        connect_fd = Accept(listen_fd, (SA * )&clientaddr, &len);
        Pthread_create(&tid, NULL, thread, &connect_fd);
    }
    Close(listen_fd);
    exit(EXIT_SUCCESS);
}
