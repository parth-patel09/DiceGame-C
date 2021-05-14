#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define MAXGAMESALLOWED 200
#define MAXCONNECTIONS 30
#define DEFAULT_PORT 4455
#define USEDEFAULTS 1
#define MAXSCORE 30
#define MAXSIZE 1024
#define SHOWSCORECARD 1
#define WELCOMEPLAYER_MSG "You can now play"

char DEFAULT_IP[20] = "127.0.0.1";

struct Game
{
    char name[40];
    int player1_score;
    int player2_score;
    char player1_name[40];
    char player2_name[40];
    int id;
    char decision[40];
};

int initializeServer(int PORT)
{
    int socketfd, ret;
    struct sockaddr_in server_address;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Error creating Socket.\n");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons((uint16_t)PORT);
    ret = bind(socketfd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (ret < 0)
    {
        printf("Error in binding.\n");
        exit(1);
    }
    if (listen(socketfd, MAXCONNECTIONS) == 0)
    {
        printf("Listening to port %d...\n", PORT);
    }
    else
    {
        printf("Error in listening x.\n\n");
        exit(1);
    }
    return socketfd;
}

int initializeClient(int PORT, char *ip_address)
{
    int server;
    socklen_t len;
    struct sockaddr_in server_add;
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Error creating socket!.\n");
        exit(1);
    }
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons((uint16_t)PORT);
    if (inet_pton(AF_INET, ip_address, &server_add.sin_addr) < 0)
    {
        fprintf(stderr, "Connection in INET.\n");
        exit(2);
    }
    if (connect(server, (struct sockaddr *)&server_add, sizeof(server_add)) < 0)
    {
        fprintf(stderr, "Error in Connection.\n");
        exit(3);
    }
    printf("Succesfully Connected to server with %s on %d.\n", ip_address, PORT);
    return server;
}