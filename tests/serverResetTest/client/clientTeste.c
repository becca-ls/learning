#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "core.h"
#include "client.h"
#include "player.h"

#define MSG_MAX_SIZE 50

enum conn_ret_t criaConexao()
{
    char end[MSG_MAX_SIZE];
    printf("Digite o ip do servidor\n");
    scanf(" %s", end);
    getchar();
    return connectToServer(end);
}

void conecta()
{

    enum conn_ret_t con = criaConexao();

    while (con != SERVER_UP)
    {
        puts("Problema na conexao");
        int flag;
        flag = tolower(getchar());
        while (flag != 'n' && flag != 'y')
        {
            puts("Digita de novo");
            flag = tolower(getchar());
        }

        if (flag == 'n')
            exit(EXIT_SUCCESS);
        else
            con = criaConexao();
    }

    char nick[NICK_SIZE + 4];

    printf("Digite um nick com %d caracteres: \n", NICK_SIZE - 1);
    scanf("%s", nick);

    while ((int)strlen(nick) > 3)
    {
        printf("Digite um nick com %d caracteres: \n", NICK_SIZE - 1);
        scanf(" %s", nick);
    }

    sendMsgToServer(nick, (int)strlen(nick) + 1);
}

int main()
{
    Jogador j, inimigo;
    Jogador aux;
    conecta();
    recvMsgFromServer(&j, WAIT_FOR_IT);
    imprimeJogador(j);
    recvMsgFromServer(&inimigo, WAIT_FOR_IT);
    imprimeJogador(inimigo);
    while (1)
    {
        char c = getch();
        switch (c)
        {
        case CIMA:
        case BAIXO:
        case ESQ:
        case DIR:
            sendMsgToServer((char *)&c, sizeof(char));
            break;
        default:
            break;
        }
        recvMsgFromServer(&aux, DONT_WAIT);
        imprimeJogador(aux);
    }
    return 0;
}