#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "player.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (NICK_SIZE + 100)
#define MAX_CLIENTS 2


void insereJogador(Jogador jogadores[], char *nick, int id)
{
    //TODO adicionar o jogador ao vetor de jogadores
    Jogador e = iniciaJogador(nick, id);
    jogadores[id] = e;
    return;
}

int main()
{
    //Vetor que armazena os jogadores no jogo
    Jogador jogadores[2];
    
    //Matriz que armazena os clientes presente no jogo (Provavelmente desnecessaria)
    char clientes[MAX_CLIENTS][NICK_SIZE];
    
    //String auxiliar
    char str_buffer[BUFFER_SIZE];
    
    //Inicia servidor
    serverInit(MAX_CLIENTS);

    puts("Servidor está online.");

    short numJogadores = 0;
    while (1)
    {
        int id = acceptConnection();
        if (id != NO_CONNECTION)
        {
            if (numJogadores < 2)
            {
                recvMsgFromClient(clientes[id], id, WAIT_FOR_IT);
                insereJogador(jogadores, clientes[id], id);
                imprimeJogador(jogadores[id]);
                strcpy(str_buffer, clientes[id]);
                strcpy(str_buffer, " Conectou ao chat");
                puts(str_buffer);
                sendMsgToClient((Jogador *)&jogadores[id], sizeof(Jogador), id);
            }
        }

        char *msg = malloc(350*sizeof(char));
        struct msg_ret_t msg_ret = recvMsg(msg);
        
        if(msg_ret.status == DISCONNECT_MSG)
        {
            numJogadores = numJogadores - 1;
            printf("Jogador %s do id %d desconectou.\nPosicao %d esta livre\n", jogadores[msg_ret.client_id].nick, 
                msg_ret.client_id, msg_ret.client_id);
        }

        //TODO resto do server

    }

    return 0;
}