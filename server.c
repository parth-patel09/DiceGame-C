#include "project.h"

struct Game games[MAXGAMESALLOWED];

void servicePlayers(int, int, int);
int handleArguements(int, char **);
int initializeSocket(int);
struct Game initializeStats(struct Game, int);
int initializeGame(int);
void sig_handler(int);

int p_count = 0, gameid = 1, gamecount = 0;
pid_t pid[5];

int main(int argc, char *argv[])
{
    int socketfd, success = 0;
    socketfd = initializeServer(handleArguements(argc, argv));
    printf("\n***********Welcome to DICE Game***********\n");
    printf("\n\n\n");
    printf("Waiting for participants......\n\n");
    while (success != -1)
    {
        success = initializeGame(socketfd);
    }
    if (SHOWSCORECARD == 1)
    {
        printf("\n\n~~~~~~~SCORE CARD~~~~~~~\n\n");
        printScoreboard();
    }
    return 0;
}
void printScoreboard()
{
    printf("Game Counts: %d\n", gamecount);
    printf("Player 1\tPlayer 2\tPlayer 1 Score\tPlayer 2 Score\tDecision\n");
    for (int i = 1; i < gameid; i++)
    {
        printf("%s\t\t%s\t\t%d\t\t%d\t\t%s\n", games[i].player1_name, games[i].player2_name, games[i].player1_score, games[i].player2_score, games[i].decision);
    }
}
int checkProcess()
{
    int status;
    size_t n = sizeof(pid) / sizeof(pid[0]);
    for (int i = 0; i < n; i++)
    {
        pid_t return_pid = waitpid(pid[i], &status, WNOHANG);
        if (return_pid == -1)
        {
            return -1;
        }
        else if (return_pid == pid[i])
        {
            return 1;
        }
    }
    return 0;
}
int initializeGame(int socketfd)
{
    int i = 0, players[30], player_count = 0;
    while (player_count < (2 * MAXGAMESALLOWED))
    {
        players[player_count] = accept(socketfd, (struct sockaddr *)NULL, NULL);
        player_count++;
        if (player_count % 2 == 0)
        {
            playGame(players[player_count - 2], players[player_count - 1], gamecount);
            gameid++;
            gamecount++;
        }
        else
        {
            printf("PLAYER 1 IS READY!\nWAITING FOR ANOTHER PLAYER...\n");
        }
        sleep(2);
    }
    printf("Completed!!");
    while (checkProcess() == 0)
    {
        return -1;
    }
}
void playGame(int player1, int player2, int gamecount)
{
    games[gameid] = initializeStats(games[gameid], gameid);
    printf("PLAYER 2 IS READY!\n");
    printf("###########STARTING DICE GAME###########\n");
    if ((pid[p_count] = fork()) == 0)
    {
        servicePlayers(player1, player2, gamecount);
        p_count++;
    }
}
struct Game initializeStats(struct Game g, int id)
{
    g.id = id;
    strcpy(g.player1_name, "TITI");
    strcpy(g.player2_name, "TOTO");
    g.player1_score = 0;
    g.player2_score = 0;
    return g;
}

void servicePlayers(int player1, int player2, int gamecount)
{
    int player1_score = 0;
    int player2_score = 0;
    int size = sizeof(int64_t);
    int64_t player1_dice, player2_dice;
    while (1)
    {
        sleep(1);
        write(player1, WELCOMEPLAYER_MSG, MAXSIZE);
        if (read(player1, &player1_dice, size) < 0)
        {
            printf("Error in Reading from Player 1.\n");
        }
        player1_score += ntohl(player1_dice);

        if (player1_score >= MAXSCORE)
        {
            strcpy(games[gameid].decision, "Player 1 won the game");
            printf("Player 1 won the game with %d points over Player 2!\n", (player1_score - player2_score));
            write(player1, "Game Over: You won the game", MAXSIZE);
            write(player2, "Game Over: You lost the game", MAXSIZE);
            break;
        }
        sleep(2);
        write(player2, WELCOMEPLAYER_MSG, MAXSIZE);
        if (read(player2, &player2_dice, size) < 0)
        {
            printf("Error in Reading from Player 2.\n");
        }
        player2_score += ntohl(player2_dice);
        if (player2_score >= MAXSCORE)
        {
            strcpy(games[gameid].decision, "Player 2 won the game");
            printf("Player 2 won the game with %d points over Player 1!\n", (player2_score - player1_score));
            write(player1, "Game Over: You lost the game", MAXSIZE);
            write(player2, "Game Over: You won the game", MAXSIZE);
            break;
        }
    }
    games[gameid].player1_score = player1_score;
    games[gameid].player2_score = player2_score;
    close(player1);
    close(player2);
    printf("###########GAME OVER###########\n");
}

int handleArguements(int argc, char *argv[])
{
    int port;
    if ((USEDEFAULTS == 0) && argc != 2)
    {
        fprintf(stderr, "<INPUT>: %s <PORT>\n", argv[0]);
        exit(0);
    }
    else if ((USEDEFAULTS == 0) && argc == 2)
    {
        sscanf(argv[1], "%d", &port);
        return port;
    }
    else
    {
        return DEFAULT_PORT;
    }
}
