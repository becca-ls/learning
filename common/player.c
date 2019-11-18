#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "core.h"

//Imprime as informações do jogador
void imprimeJogador(Jogador j)
{
    printf("%s %d %d %d (%d, %d)\n", j.nick, j.id, j.saude, j.seguranca, j.pos.x, j.pos.y);
}

//Inicia todos os jogadores com devidos atributos e posicoes
Jogador iniciaJogador(char *nick, int id)
{

    Jogador e;
    e.saude = 10;
    e.seguranca = 0;
    e.id = id;
    strcpy(e.nick, nick);
    e.pos.x = 10;
    e.pos.y = 10;

    return e;
}