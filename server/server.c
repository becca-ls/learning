#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "server.h"
#include "player.h"
#include "core.h"
#include <time.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (NICK_SIZE + 100)
#define MAX_CLIENTS 2

void generateOilStain(int **matrix){
    
    int stains = NUMBER_OF_STAINS;
    
    // Gerando um numero aleatório
    srand(time(NULL));
    while(stains>0){
        int y = rand()%LINHA;
        int x = rand()%COLUNA;
        if (matrix[y][x] == FREE_POS){
            matrix[y][x] = OIL_STAIN;
            stains--;
        }
    }

}

int** readGrid(){
    
    int i,j, aux;
    FILE *f;
    char auxi;
    int ** matrix;

    //Alocando memória
    matrix = (int**) malloc( LINHA*(sizeof(int*)) );
    for(i=0; i<LINHA;i++){
        matrix[i] = (int*) malloc(COLUNA*(sizeof(int)));
    }
    //Abrindo o arquivo de texto
    f = fopen("map.txt", "r"); //Por obséquio coloquem "map.txt" na msm pasta do server ou mudem o path
    for(i=0;i<LINHA;i++){
        for(j=0;j<COLUNA;j++){
            //Procedimento para salvar um inteiro em vez de um char;
            fscanf(f, " %c", &auxi);
            aux = auxi - '0'; 
            matrix[i][j]= aux;
        }
    }
    //fechando o f
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

void wrongPlaceDude(Jogador *player, char damage){               
    switch(damage){
        case OIL_STAIN:
            player->saude = player->saude - 20; //Valor a ser definido, 20 eh o exemplo
            break;
    }
}


bool outOfBounds(int x, int y){
    if(x<0 || x>=COLUNA) return true;
    if(y<0 || y>=LINHA) return true;
    return false;
}

void Catch(int **matrix, Jogador *player){
    switch(player->face){
        case CIMA:
            if(outOfBounds((*player).pos.x, (*player).pos.y-1) == false && matrix[(*player).pos.x][(*player).pos.y - 1] == OIL_STAIN){
                matrix[(*player).pos.x][(*player).pos.y-1] = FREE_POS;
                player->oleo_coletado++;
            }
            break;
        case BAIXO:
            if(outOfBounds((*player).pos.x, (*player).pos.y+1) == false && matrix[(*player).pos.x][(*player).pos.y+1] == OIL_STAIN){
                matrix[(*player).pos.x][(*player).pos.y+1] = FREE_POS;
                player->oleo_coletado++;
            }
            break;
        case ESQ:
            if(outOfBounds((*player).pos.x-1, (*player).pos.y) == false && matrix[(*player).pos.x-1][(*player).pos.y] == OIL_STAIN){
                matrix[(*player).pos.x-1][(*player).pos.y] = FREE_POS;
                player->oleo_coletado++;
            }
            break;
        case DIR:
            if(outOfBounds((*player).pos.x+1, (*player).pos.y) == false && matrix[(*player).pos.x+1][(*player).pos.y] == OIL_STAIN){
                matrix[(*player).pos.x+1][(*player).pos.y] = FREE_POS;
                player->oleo_coletado++;
            }
            break;
    }
}

bool checkCollision(int** matrix,char command, Jogador *player){

    switch(command){
        case CIMA:
            player->face = CIMA;
            if( outOfBounds((*player).pos.x,(*player).pos.y-1) == true || matrix[(*player).pos.y-1][(*player).pos.x] == OBSTACLE || matrix[(*player).pos.y-1][(*player).pos.x] == PLAYER ) return true;
            return false;
            break;
        case BAIXO:
            player->face = BAIXO;
            if( outOfBounds((*player).pos.x,(*player).pos.y+1) == true || matrix[(*player).pos.y+1][(*player).pos.x] == OBSTACLE || matrix[(*player).pos.y+1][(*player).pos.x] == PLAYER ) return true;
            return false;
            break;
        case DIR:
            player->face = DIR;
            if( outOfBounds((*player).pos.x+1,(*player).pos.y) == true || matrix[(*player).pos.y][(*player).pos.x+1] == OBSTACLE || matrix[(*player).pos.y][(*player).pos.x+1] == PLAYER ) return true;
            return false;
            break;
        case ESQ:
            player->face = ESQ;
            if( outOfBounds((*player).pos.x-1,(*player).pos.y) == true || matrix[(*player).pos.y][(*player).pos.x-1] == OBSTACLE || matrix[(*player).pos.y][(*player).pos.x-1] == PLAYER ) return true;
            return false;
            break;
        default:
            return true; //Invalid command
    }

}

void takeAnAction(int **matrix, Jogador *player, char tipo_movimento){
    player->tipo_movimento = tipo_movimento;
    switch(tipo_movimento){
        case CIMA:
            if(checkCollision(matrix,CIMA, player) == false){
                matrix[(*player).pos.y][(*player).pos.x] = FREE_POS;
                player->pos.y = player->pos.y -1;
                if(matrix[(*player).pos.y][(*player).pos.x]== OIL_STAIN) wrongPlaceDude(player, OIL_STAIN);
            }
            break;
        case BAIXO:
            if(checkCollision(matrix,BAIXO, player) == false){
                matrix[(*player).pos.y][(*player).pos.x] = FREE_POS;
                player->pos.y = player->pos.y +1;
                if(matrix[(*player).pos.y][(*player).pos.x]== OIL_STAIN) wrongPlaceDude(player, OIL_STAIN);
            }
            break;
        case ESQ:
            if(checkCollision(matrix,ESQ, player) == false){
                matrix[(*player).pos.y][(*player).pos.x] = FREE_POS;
                player->pos.x = player->pos.x -1;
                if(matrix[(*player).pos.y][(*player).pos.x]== OIL_STAIN) wrongPlaceDude(player, OIL_STAIN);
            }
            break;
        case DIR:
            if(checkCollision(matrix,DIR, player) == false){
                matrix[(*player).pos.y][(*player).pos.x] = FREE_POS;
                player->pos.x = player->pos.x + 1;
                if(matrix[(*player).pos.y][(*player).pos.x]== OIL_STAIN) wrongPlaceDude(player, OIL_STAIN);
            }
            break;
        case ACAO:
            Catch(matrix, player);
        break;
        player->tipo_movimento = NENHUM;
    }
}

int main()
{
    //Matriz do jogo
    int **grid = NULL;

    //Le o arquivo "map.txt" e aloca em grid
    grid = readGrid();

    //Gera as manchas de oleo na matriz
    generateOilStain(grid);

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
                takeAnAction(grid,&jogadores[msg_ret.client_id], id_movimento);
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