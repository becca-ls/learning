#ifndef ENUM_H
#define ENUM_H

#include <stdbool.h>
#include <player.h> 

#define LINHA 16
#define COLUNA 20
#define CIMA 'w'
#define BAIXO 's'
#define ESQ 'a'
#define DIR 'd'
#define NUMBER_OF_STAINS 20


typedef struct 
{
  Jogador p1;
  Jogador p2; 
} Jogo;

typedef struct 
{
    char tipo_movimento;
    int cliente_id;
} Movimento;



enum Acao
{
    NENHUM,
    POSICAO,
    DANO,
    ACAO
};

enum Estado_jogo
{
    PREJOGO,
    JOGANDO,
    POSJOGO
};

enum position
{
    FREE_POS,
    OBSTACLE,
    PLAYER,
    OIL_STAIN
};

typedef struct {
    Pos pos;
    bool coletado;
} Oleo;

#endif