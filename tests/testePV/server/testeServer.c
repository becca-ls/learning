#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "server.h"
#include "core.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (NICK_SIZE + 100)
#define MAX_CLIENTS 2

void imprime(int **grid)
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            //Procedimento para salvar um inteiro em vez de um char;
            printf("%d", grid[i][j]);
        }
            puts("");
    }
}

int main()
{
    Game aux;
    //Matriz do jogo
    int **grid = NULL;

    //Le o arquivo "map.txt" e aloca em grid
    grid = readGrid();
    
    Player *jogadores = (Player *)malloc(MAX_CLIENTS*sizeof(Player));
    
    aux.game_state = PREGAME;
    initOils(&aux, grid);
    //String auxiliar
    char str_buffer[NICK_SIZE+1];

    //Inicia servidor
    serverInit(MAX_CLIENTS);

    puts("Servidor está online.");

    int qtdPlayers = 0;
    while (1)
    {
        int id = acceptConnection();
        if (id != NO_CONNECTION)
        {
                Player new;
                recvMsgFromClient(str_buffer, id, WAIT_FOR_IT);
                printf("%s conectou ao jogo\n", str_buffer);
                new = player(str_buffer);
                addPlayer(jogadores, new, &qtdPlayers);
                if(id == 0)
                    aux.p1 = jogadores[id];
                else
                    aux.p2 = jogadores[id];
                
                printPlayer(jogadores[id]);
                sendMsgToClient((Game *)&aux, sizeof(Game), id);
                aux.game_state = INGAME;
        }

        //enviaInimigo(jogadores, numJogadores, &estado_jogo);

        /*Move act;
        while (estado_jogo == INGAME)
        {
            
            struct msg_ret_t msg_ret = recvMsg(&act);
            if (msg_ret.status == MESSAGE_OK)
            {
                movePlayer(grid, &jogadores[msg_ret.client_id], act);
                aux.p1 = jogadores[0];
                aux.p2 = jogadores[1];
                broadcast((Game *)&aux, sizeof(Game));
                //TODO TRATAR AS MENSAGENS RECEBIDAS
            }

            if (msg_ret.status == DISCONNECT_MSG)
            {
                qtdPlayers = qtdPlayers - 1;
                printf("Jogador %s do id %d desconectou.\nPosicao %d esta livre\n", jogadores[msg_ret.client_id].nick,
                       msg_ret.client_id, msg_ret.client_id);
            }
        }*/

        //TODO resto do servidor
    }
    // Dando free no grid
    freeGrid(grid);
    //free(oleos);
    free(jogadores);
    return 0;
}