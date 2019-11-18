#ifndef ENUM_H
#define ENUM_H

#define CIMA 'w'
#define BAIXO 's'
#define ESQ 'a'
#define DIR 'd'

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
