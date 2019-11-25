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

bool checkEnd(Game j)
{
        
    if(j.p1.oleo_points == 10 || j.p2.oleo_points == 10)
        return true;

    return false;
}

Player player(char *nick, int id)
{
    Player e;

    e.oleo_points = 0;
    e.pos.x = id == 0? 3: COLUMN-3;
    e.pos.y = 2;
    e.sprites.x = 0;
    e.sprites.y = 0;
    strcpy(e.nick, nick);

    if(id == 0)
        strcpy(e.skin, "luffy.png");
    else
        strcpy(e.skin, "chrono.png");
    
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

bool poscmp(Pos a, Pos b)
{
    if (a.x == b.x && a.y == b.y)
        return true;

    return false;
}

int checkFreePos(int **grid, int i, int j)
{
    int r;
    if (grid[i][j] == FREE_POS)
        r = 1;
    else
        r = 0;
    return r;
}

bool freeToWalk(int **grid, int i, int j)
{
    bool r = false;
    if (grid[i][j] != OBSTACLE)
        r = true;

    return r;
}

void movePlayer(int **grid, Player *p, Move act)
{

    switch (act.act)
    {
    case UP:
        if (freeToWalk(grid, (p->pos.y) - 1, (p->pos.x)))
            p->pos.y -= 1;
        break;
    case DOWN:
        if (freeToWalk(grid, (p->pos.y) + 1, (p->pos.x)))
            p->pos.y += 1;
        break;
    case LEFT:
        if (freeToWalk(grid, (p->pos.y), (p->pos.x) - 1))
            p->pos.x -= 1;
        break;
    case RIGHT:
        if (freeToWalk(grid, (p->pos.y), (p->pos.x) + 1))
            p->pos.x += 1;
        break;
    default:
        break;
    }
}

bool isOil(int **grid, int i, int j)
{
    if (grid[i][j] == OIL)
        return true;

    return false;
}

Pos genNewPos(int **grid)
{
    Pos new;
    bool flag = false;
    while(!flag)
    {
        new.x = rand()%COLUMN;
        new.y = rand()%ROW;
        if(checkFreePos(grid, new.y, new.x))
            flag = true;
    }

    return new;
}

void catch (Player *player, Oleo *oleo, int **grid)
{
    oleo->inGame = true;
    player->oleo_points += 1;
    setFreePos(grid, oleo->pos.y, oleo->pos.x);
    oleo->pos = genNewPos(grid);
    printGrid(grid);
    grid[oleo->pos.y][oleo->pos.x] = OIL;
}

void actPlayer(int **grid, Game *g, Move act, int id)
{
    //Player *curr = id==0? &(g->p1):&(g->p2);
    if (id == 0)
    {
        movePlayer(grid, &(g->p1), act);
        if (isOil(grid, g->p1.pos.y, g->p1.pos.x))
        {
            if (poscmp(g->p1.pos, g->oil_a.pos))
                catch (&(g->p1), &(g->oil_a), grid);
            else if (poscmp(g->p1.pos, g->oil_b.pos))
                catch (&(g->p1), &(g->oil_b), grid);
            else if (poscmp(g->p1.pos, g->oil_c.pos))
                catch (&(g->p1), &(g->oil_c), grid);
        }
    }
    else if (id == 1)
    {
        movePlayer(grid, &(g->p2), act);
        if (isOil(grid, g->p2.pos.y, g->p2.pos.x))
        {
            if (poscmp(g->p2.pos, g->oil_a.pos))
                catch (&(g->p2), &(g->oil_a), grid);
            else if (poscmp(g->p2.pos, g->oil_b.pos))
                catch (&(g->p2), &(g->oil_b), grid);
            else if (poscmp(g->p2.pos, g->oil_c.pos))
                catch (&(g->p2), &(g->oil_c), grid);
        }
    }

}

bool outOfOil(Game g)
{
    if (checkOil(g.oil_a) || checkOil(g.oil_b) || checkOil(g.oil_c))
        return false;

    return true;
}

bool checkOil(Oleo e)
{
    return e.inGame;
}

Oleo genOil(int **grid)
{
    Oleo oleo;
    bool flag = true;
    int x, y;
    //srand(time(NULL));

    while (flag)
    {
        x = rand() % ROW;
        y = rand() % COLUMN;
        if (grid[y][x] == FREE_POS)
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
    for (int i = 0; i < NUMBER_OF_OILS; i++)
    {
        printf("(%d,%d)\n", oleos[i].pos.x, oleos[i].pos.y);
    }
}

void printPlayer(Player player)
{
    printf("%s %d (%d, %d)\n", player.nick, player.oleo_points, player.pos.x, player.pos.y);
}

void printGrid(int **grid)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COLUMN; j++)
        {
            printf("%d", grid[i][j]);
        }
        puts("");
    }
}

void freeGrid(int **grid)
{
    for (int i = 0; i < ROW; i++)
    {
        free(grid[i]);
    }

    free(grid);
}
