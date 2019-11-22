#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

int **readGrid()
{

    int i, j, aux;
    FILE *f;
    char auxi;
    int **matrix;

    //Alocando memória
    matrix = (int **)malloc(ROW * (sizeof(int *)));
    for (i = 0; i < ROW; i++)
    {
        matrix[i] = (int *)malloc(COLUMN * (sizeof(int)));
    }
    //Abrindo o arquivo de texto
    f = fopen("mapTeste.txt", "r"); //Por obséquio coloquem "map.txt" na msm pasta do server ou mudem o path
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COLUMN; j++)
        {
            //Procedimento para salvar um inteiro em vez de um char;
            fscanf(f, " %c", &auxi);
            aux = auxi - '0';
            matrix[i][j] = aux;
        }
    }
    //fechando o f
    printGrid(matrix);
    fclose(f);
    return matrix;
}

Player player(char *nick)
{
    Player e;

    e.oleo_coletado = 0;
    e.pos.x = 3;
    e.pos.y = 2;
    e.saude = 10;
    strcpy(e.nick, nick);

    return e;
}

void addPlayer(Player *v, Player e, int *qtdPlayers)
{
    v[*qtdPlayers] = e;
    (*qtdPlayers)++;
}

void setFreePos(int **grid, int i, int j)
{
    grid[i][j] = FREE_POS;
}

int checkFreePos(int **grid, int i, int j)
{
    int r;
    if(grid[i][j] == FREE_POS) r = 1;
    else r = 0;
    return r;
}

void movePlayer(int **grid, Player *p, Move act)
{

    switch (act.act)
    {
    case UP:
        if (checkFreePos(grid, (p->pos.y) - 1, (p->pos.x)))
            p->pos.y -= 1;
        break;
    case DOWN:
        if (checkFreePos(grid, (p->pos.y) + 1, (p->pos.x)))
            p->pos.y += 1;
        break;
    case LEFT:
        if (checkFreePos(grid, (p->pos.y), (p->pos.x) - 1))
            p->pos.x -= 1;
        break;
    case RIGHT:
        if (checkFreePos(grid, (p->pos.y), (p->pos.x) + 1))
            p->pos.x += 1;
        break;
    default:
        break;
    }
}

Oleo genOil(int** grid)
{ 
    Oleo oleo;
    bool flag = true;
    int x, y;
    //srand(time(NULL));

    while(flag)
    {
        x = rand()%ROW;
        y = rand()%COLUMN;
        if(grid[y][x] == FREE_POS)
        {
            oleo.pos.x = x;
            oleo.pos.y = y;
            oleo.inGame = true;
            grid[y][x] = OIL;
            flag = false;
        }
    }
     
    return oleo;
}

void initOils(Game *g, int **grid)
{
    g->oil_a = genOil(grid);
    g->oil_b = genOil(grid);
    g->oil_c = genOil(grid);
}

void printOil(Oleo *oleos)
{
    for(int i = 0; i < NUMBER_OF_OILS; i++)
    {
        printf("(%d,%d)\n", oleos[i].pos.x, oleos[i].pos.y);
    }
}

void printPlayer(Player player)
{
    printf("%s %d %d (%d, %d)\n", player.nick, player.saude, player.oleo_coletado, player.pos.x, player.pos.y);
}

void printGrid(int **grid)
{
    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            printf("%d", grid[i][j]);
        }
        puts("");
    }
}

void freeGrid(int **grid)
{
    for(int i =0; i < ROW; i++)
    {
        free(grid[i]);
    }

    free(grid);
}
