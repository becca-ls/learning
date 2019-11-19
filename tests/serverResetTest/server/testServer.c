#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "server.h"
#include "player.h"
#include "core.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (NICK_SIZE + 100)
#define MAX_CLIENTS 2

void insereJogador(Jogador *jogadores, char *nick, int id, short *numJogadores)
{
  //TODO adicionar o jogador ao vetor de jogadores
  Jogador e = iniciaJogador(nick, id);
  jogadores[id] = e;
  (*numJogadores)++;
  return;
}

void enviaInimigo(Jogador *jogadores, int totalJogadores, char *estado)
{
  if (totalJogadores == MAX_CLIENTS)
  {
    //TODO envia dados do inimigo ao cliente
    Jogador_inimigo inimigo;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      int id_cliente_inimigo = (i == 0 ? 1 : 0);
      inimigo.pos = jogadores[i].pos;
      inimigo.saude = jogadores[i].saude;
      strcpy(inimigo.nick, jogadores[i].nick);
      sendMsgToClient((Jogador_inimigo *)&inimigo, sizeof(Jogador_inimigo), id_cliente_inimigo);
    }

    *estado = JOGANDO;
  }
}

void moveJogador(Jogador *p, char id_movimento)
{
  if (id_movimento == CIMA)
  {
    p->pos.y -= 1;
  }
  else if (id_movimento == BAIXO)
  {
    p->pos.y += 1;
  }
  else if (id_movimento == ESQ)
  {
    p->pos.x -= 1;
  }
  else if (id_movimento == DIR)
  {
    p->pos.x += 1;
  }
}

int main()
{
  //Matriz do jogo
  int **grid = NULL;

  //Le o arquivo "map.txt" e aloca em grid
  //grid = readGrid();

  //Gera as manchas de oleo na matriz
  //generateOilStain(grid);

  //Vetor que armazena os jogadores no jogo
  Jogador *jogadores = (Jogador *)malloc(MAX_CLIENTS * sizeof(Jogador));

  //Matriz que armazena os clientes presente no jogo (Provavelmente desnecessaria)
  char clientes[MAX_CLIENTS][NICK_SIZE];

  char estado_jogo = PREJOGO;

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
        printf("%s conectou ao jogo\n", clientes[id]);
        insereJogador(jogadores, clientes[id], id, &numJogadores);
        imprimeJogador(jogadores[id]);
        sendMsgToClient((Jogador *)&jogadores[id], sizeof(Jogador), id);
      }
    }

    char *msg = malloc(350 * sizeof(char));
    struct msg_ret_t msg_ret = recvMsg(msg);

    if (msg_ret.status == DISCONNECT_MSG)
    {
      numJogadores = numJogadores - 1;
      printf("Jogador %s do id %d desconectou.\nPosicao %d esta livre\n", jogadores[msg_ret.client_id].nick,
             msg_ret.client_id, msg_ret.client_id);
    }

    enviaInimigo(jogadores, numJogadores, &estado_jogo);

    char id_movimento;
    while (estado_jogo == JOGANDO)
    {

      struct msg_ret_t msg_ret = recvMsg(&id_movimento);
      if (msg_ret.status == MESSAGE_OK)
      {
        moveJogador(&jogadores[msg_ret.client_id], id_movimento);
        broadcast((Jogador *)&jogadores[msg_ret.client_id], sizeof(Jogador));
        //TODO TRATAR AS MENSAGENS RECEBIDAS
      }
    }

    //TODO resto do servidor
  }
  // Dando free no grid
  finishGrid(grid);

  free(jogadores);
  return 0;
}