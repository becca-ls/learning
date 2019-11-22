#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "server.h"
#include "core.h"
#include "player.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (NICK_SIZE + 100)
#define MAX_CLIENTS 2

void imprime(int **grid)
{
    int i, j;
    for (i = 0; i < LINHA; i++)
    {
        for (j = 0; j < COLUNA; j++)
        {
            //Procedimento para salvar um inteiro em vez de um char;
            printf("%d", grid[i][j]);
        }
            puts("");
    }
}

int **readGrid()
{

    int i, j, aux;
    FILE *f;
    char auxi;
    int **matrix;

    //Alocando memória
    matrix = (int **)malloc(LINHA * (sizeof(int *)));
    for (i = 0; i < LINHA; i++)
    {
        matrix[i] = (int *)malloc(COLUNA * (sizeof(int)));
    }
    //Abrindo o arquivo de texto
    f = fopen("mapTeste.txt", "r"); //Por obséquio coloquem "map.txt" na msm pasta do server ou mudem o path
    for (i = 0; i < LINHA; i++)
    {
        for (j = 0; j < COLUNA; j++)
        {
            //Procedimento para salvar um inteiro em vez de um char;
            fscanf(f, " %c", &auxi);
            aux = auxi - '0';
            matrix[i][j] = aux;
        }
    }
    //fechando o f
    imprime(matrix);
    fclose(f);
    return matrix;
}

void finishGrid(int** matrix){

    int i;
    for(i=0; i<LINHA;i++){
        free(matrix[i]);
    }
    free(matrix);
}

void insereJogador(Jogador *jogadores, char *nick, int id, short *numJogadores)
{
    //TODO adicionar o jogador ao vetor de jogadores
    Jogador e = iniciaJogador(nick, id);
    jogadores[id] = e;
    (*numJogadores)++;
    return;
}

void setFreePos(int **grid, int i, int j)
{
    grid[i][j] = FREE_POS;
}

int checkfreePos(int **grid, int i, int j)
{
    int r;
    if(grid[i][j] == FREE_POS) r = 1;
    else r = 0;
    return r;
}

void movePlayer(int **grid, Jogador *p, Movimento acao)
{

    switch (acao.tipo_movimento)
    {
    case CIMA:
        if (checkfreePos(grid, (p->pos.y) - 1, (p->pos.x)))
            p->pos.y -= 1;
        break;
    case BAIXO:
        if (checkfreePos(grid, (p->pos.y) + 1, (p->pos.x)))
            p->pos.y += 1;
        break;
    case ESQ:
        if (checkfreePos(grid, (p->pos.y), (p->pos.x) - 1))
            p->pos.x -= 1;
        break;
    case DIR:
        if (checkfreePos(grid, (p->pos.y), (p->pos.x) + 1))
            p->pos.x += 1;
        break;
    default:
        break;
    }
}

void imprimeOleo(Oleo *oleos)
{
    for(int i = 0; i < NUMBER_OF_STAINS; i++)
    {
        printf("(%d,%d)", oleos[i].pos.x, oleos[i].pos.y);
    }
}

Oleo *genOleo(int** grid)
{ 
    Oleo *oleos = NULL;
    int i = 0, x, y;
    //srand(time(NULL));
    oleos = (Oleo *)malloc(NUMBER_OF_STAINS*sizeof(Oleo));
    if(oleos == NULL)
        exit(1);

    while(i < NUMBER_OF_STAINS)
    {
        x = rand()%LINHA;
        y = rand()%COLUNA;
        if(grid[y][x] == FREE_POS)
        {
            oleos[i].pos.x = x;
            oleos[i].pos.y = y;
            grid[y][x] = OIL_STAIN;
            i++;
        }
    }
     
    return oleos;
}

int main()
{
    Jogo aux;
    //Matriz do jogo
    int **grid = NULL;

    //Le o arquivo "map.txt" e aloca em grid
    grid = readGrid();
    //srand(time(NULL));
    aux.oleo = genOleo(grid);

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
                estado_jogo = JOGANDO;
            }
        }

        //enviaInimigo(jogadores, numJogadores, &estado_jogo);

        Movimento acao;
        while (estado_jogo == JOGANDO)
        {
            
            struct msg_ret_t msg_ret = recvMsg(&acao);
            if (msg_ret.status == MESSAGE_OK)
            {
                movePlayer(grid, &jogadores[msg_ret.client_id], acao);
                aux.p1 = jogadores[0];
                aux.p2 = jogadores[1];
                broadcast((Jogo *)&aux, sizeof(Jogo));
                //TODO TRATAR AS MENSAGENS RECEBIDAS
            }

            if (msg_ret.status == DISCONNECT_MSG)
            {
                numJogadores = numJogadores - 1;
                printf("Jogador %s do id %d desconectou.\nPosicao %d esta livre\n", jogadores[msg_ret.client_id].nick,
                       msg_ret.client_id, msg_ret.client_id);
            }
        }

        //TODO resto do servidor
    }
    // Dando free no grid
    finishGrid(grid);
    //free(oleos);
    free(jogadores);
    return 0;
}