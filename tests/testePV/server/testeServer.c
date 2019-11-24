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


int main()
{
    Game aux;
    //Matriz do jogo
    int **grid = NULL;
    int secs = 10;
    //Le o arquivo "map.txt" e aloca em grid
    grid = readGrid();

    Player *jogadores = (Player *)malloc(MAX_CLIENTS * sizeof(Player));

    aux.game_state = PREGAME;
    initOils(&aux, grid);
    //String auxiliar
    char str_buffer[NICK_SIZE + 1];

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
            new = player(str_buffer, id);
            addPlayer(jogadores, new, &qtdPlayers);
            if (qtdPlayers == 2)
            {
                aux.game_state = INGAME;
            }
            if (id == 0)
                aux.p1 = jogadores[id];
            else
                aux.p2 = jogadores[id];

            printPlayer(jogadores[id]);
            sendMsgToClient((Game *)&aux, sizeof(Game), id);
        }

        //enviaInimigo(jogadores, numJogadores, &estado_jogo);

        Move act;
        while (aux.game_state == INGAME)
        {

            struct msg_ret_t msg_ret = recvMsg(&act);
            if (msg_ret.status == MESSAGE_OK)
            {
                actPlayer(grid, &aux, act, msg_ret.client_id);
                if(checkEnd(aux))
                {
                    aux.game_state = POSGAME;
                }
                jogadores[0] = aux.p1;
                jogadores[1] = aux.p2;
                //if(outOfOil(aux))
                //  initOils(&aux, grid);
                broadcast((Game *)&aux, sizeof(Game));
                //TODO TRATAR AS MENSAGENS RECEBIDAS
            }

            if (msg_ret.status == DISCONNECT_MSG)
            {
                qtdPlayers = qtdPlayers - 1;
                printf("Jogador %s do id %d desconectou. Finalizando servidor.\n\n", jogadores[msg_ret.client_id].nick,
                       msg_ret.client_id, msg_ret.client_id);
                aux.game_state = POSGAME;
                broadcast((void *)&aux, sizeof(Game));
            }
        }

        if(aux.game_state == POSGAME)
            break;
        //TODO resto do servidor
    }
    // Dando free no grid
    freeGrid(grid);
    //free(oleos);
    free(jogadores);
    return 0;
}