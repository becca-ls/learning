#ifndef ENUM_H
#define ENUM_H

#define LINHA 0
#define COLUNA 0
#define CIMA 'w'
#define BAIXO 's'
#define ESQ 'a'
#define DIR 'd'
#define NUMBER_OF_STAINS 20

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

#endif
