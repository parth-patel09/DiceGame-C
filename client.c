#include "project.h"

void handleArguements(int, char **, int *, char **);
int initializeSocket(int, char *);

int main(int argc, char *argv[])
{
    int dice, score = 0,port;
    char *ip_add, *buffer;
    int64_t dice_number;
    int size = sizeof(dice_number);
    handleArguements(argc, argv, &port, &ip_add);
    int server = initializeClient(port, ip_add);
    while (1)
    {
        if (read(server, buffer, MAXSIZE) < 0)
        {
            fprintf(stderr, "Error Reading Buffer.\n");
        }
        printf("\n%s\n", buffer);
        if (strcmp(buffer, WELCOMEPLAYER_MSG) == 0)
        {
            dice = (int)time(0) % 6 + 1;
            printf("You got: %d.\n", dice);
            score += dice;
            printf("Your total score: %d.\n\n", score);
            dice_number = htonl(dice);
            write(server, &dice_number, size);
        }
    }
    exit(0);
}
void handleArguements(int argc, char *argv[], int *PORT, char **IP_ADDRESS)
{
    int temp_variable;
    char temp_string[30];
    if ((USEDEFAULTS == 0) && (argc < 3))
    {
        printf("INPUT:%s <IP> <PORT>\n", argv[0]);
        exit(0);
    }
    if ((USEDEFAULTS == 0) && (argc == 3))
    {
        sscanf(argv[2], "%d", &temp_variable);
        sscanf(argv[1], "%s", &temp_string);
        *PORT = temp_variable;
        *IP_ADDRESS = temp_string;
    }
    else if (USEDEFAULTS == 1)
    {
        *PORT = DEFAULT_PORT;
        *IP_ADDRESS = DEFAULT_IP;
    }
}