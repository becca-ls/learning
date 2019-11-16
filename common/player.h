#ifndef PLAYER_H
#define PLAYER_H

#define NICK_SIZE 4

typedef struct
{
    int x, y;
} Pos;

typedef struct
{
    char nick[NICK_SIZE];
    Pos pos;
    char saude;
    char seguranca;
    int id;

    //TODO definir jogador resto do que precisa do jogador
} Jogador;

typedef struct 
{
    char nick[NICK_SIZE];
    Pos pos;
    char saude;
} Jogador_inimigo;


Jogador iniciaJogador(char *nick, int id);

void imprimeJogador(Jogador j);

#endif