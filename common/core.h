#ifndef CORE_H
#define CORE_H

#define NICK_SIZE 3

#define NUMBER_OF_OILS 3

#define ROW 16
#define COLUMN 20
#define UP 'W'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

typedef struct
{
    int x, y;
} Pos;

typedef struct 
{
    Pos pos;
    bool inGame;
} Oleo;


typedef struct
{
    char act;
    int idClient;
} Move;

typedef struct
{
    int regiaox;
    int regiaoy;
    char nick[NICK_SIZE+1];
    char skin[20];
    Pos pos, sprites;
    int oleo_points;
    
} Player;

typedef struct
{
    Player p1;
    Player p2;
    Oleo oil_a, oil_b, oil_c;
    int game_state;
} Game;

enum GAME_STATE
{
    PREGAME,
    INGAME,
    POSGAME
};

enum MAP
{
    FREE_POS,
    OBSTACLE,
    OIL
};

int **readGrid();

bool checkEnd(Game j);

Player player(char *nick, int id);

void addPlayer(Player *v, Player e, int *qtdPlayers);

void setFreePos(int **grid, int i, int j);

int checkFreePos(int **grid, int i, int j);

bool freeToWalk(int **grid, int i, int j);

void movePlayer(int **grid, Player *p, Move act);

bool isOil(int **grid, int i, int j);

Pos genNewPos(int **grid);

void catch(Player *player, Oleo *oleo, int **grid);

void actPlayer(int **grd, Game *g, Move act, int id);

void printOil(Oleo *oleos);

bool outOfOil(Game g);

bool checkOil(Oleo e);

Oleo genOil(int **grid);

void initOils(Game *g, int **grid);

void printPlayer(Player player);

void printGrid(int **grid);

void freeGrid(int **grid);



#endif