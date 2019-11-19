#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "core.h"
#include "client.h"
#include "player.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#define MSG_MAX_SIZE 50

const int largura_t = 640;
const int altura_t = 480;

ALLEGRO_DISPLAY *display=NULL;

void imprimeInimigo(Jogador e)
{
    printf("%s %d (%d, %d)\n", e.nick, e.saude, e.pos.x, e.pos.y);
}

enum conn_ret_t criaConexao() {
    char end[MSG_MAX_SIZE];
    printf("Digite o ip do servidor\n");
    scanf(" %s", end);
    getchar();
    return connectToServer(end);
}

void conecta() {

    enum conn_ret_t con = criaConexao();
    
    while(con != SERVER_UP) {
        puts("Problema na conexao");
        int flag;
        flag = tolower(getchar());
        while(flag != 'n' && flag != 'y'){
            puts("Digita de novo");
            flag = tolower(getchar());
        }

        if(flag == 'n')
            exit(EXIT_SUCCESS);
        else 
            con = criaConexao();
    }

    char nick[NICK_SIZE + 4];
    
    printf("Digite um nick com %d caracteres: \n", NICK_SIZE-1);
    scanf("%s", nick);
    
    while((int)strlen(nick) > 3) {
        printf("Digite um nick com %d caracteres: \n", NICK_SIZE-1);
        scanf(" %s", nick);
    }

    sendMsgToServer(nick, (int)strlen(nick)+1);
}

int main() {

    if(!al_init()){
    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }
    Jogador j;
    Jogador aux;
    conecta();
    recvMsgFromServer(&j, WAIT_FOR_IT);
    imprimeJogador(j);
    Jogador inimigo;
    recvMsgFromServer(&inimigo, WAIT_FOR_IT);
    imprimeInimigo(inimigo);
    display = al_create_display(largura_t,altura_t);
    if(!display){
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
    return -1;
    }
    al_install_keyboard();
    al_init_primitives_addon();
    while(1)
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
        if(j.id==aux.id)j=aux;
        else if(aux.id==inimigo.id)inimigo=aux;
        al_draw_filled_rectangle(j.pos.x,j.pos.y,j.pos.x+30,j.pos.y+30,al_map_rgb(255,255,0));
        al_draw_filled_rectangle(inimigo.pos.x,inimigo.pos.y,inimigo.pos.x+30,inimigo.pos.y+30,al_map_rgb(255,255,0));
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        imprimeJogador(aux);
    }
     al_destroy_display(display);
    return 0;
}