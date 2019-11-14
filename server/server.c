#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (NICK_SIZE + 100)
#define NICK_SIZE 4
#define MAX_CLIENTS 2

typedef struct 
{
    int x, y;
} Pos;


typedef struct 
{
    //TODO definir jogador
} Jogador;


void addJogador()
{
    //TODO adicionar o jogador ao vetor de jogadores
    return;
}

int main()
{
    char clientes[MAX_CLIENTS][NICK_SIZE];
    char str_buffer[BUFFER_SIZE];
    serverInit(MAX_CLIENTS);
    puts("Servidor está online.");

    while(1) {
        int id = acceptConnection();
        if(id != NO_CONNECTION)
        {
            recvMsgFromClient(clientes[id], id, WAIT_FOR_IT);
            insereJogador();
            strcpy(str_buffer, clientes[id]);
            strcpy(str_buffer, "Conectou ao chat");
            puts(str_buffer);
        }

        void *msg = NULL; //definir o tipo de mensagem que o servidor vai receber
        struct msg_ret_t msg_ret = recvMsg(msg);
    }
    
    return 0;
}