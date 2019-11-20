#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "client.h"
#include "core.h"
#include "player.h"

#define MSG_MAX_SIZE 50
#define FPS 60

#define TILE_HEIGHT 64
#define TILE_WIDTH 64
#define HEIGHT 1024
#define WIDTH 1280

bool inicializa()
{
    if(!al_init())
    {
        puts("Incapaz de iniciar o allegro");
        exit(1);
    }
    if(!al_init_image_addon())
    {
        puts("Incapaz de iniciar o image addon");
        exit(1);
    }
    if(!al_install_keyboard())
    {
        puts("Incapaz de instalar o keyboard addon");
        exit(1);
    }

    if(!al_init_primitives_addon())
    {
        puts("Incapaz de iniciar o primitives addon");
        exit(1);
    }

    return true;
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

void draw(Jogador j, ALLEGRO_BITMAP *bitmap)
{
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_draw_filled_rectangle((j.pos.x*TILE_WIDTH), (j.pos.y*TILE_HEIGHT),(j.pos.x*TILE_WIDTH)+30, (j.pos.x*TILE_HEIGHT)+30, al_map_rgb(100, 125, 99));
    al_flip_display();
}

int main()
{
    Jogador p;
    conecta();
    recvMsgFromServer(&p, WAIT_FOR_IT);
    imprimeJogador(p);
    
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *background = NULL;
    
    inicializa();

    display = al_create_display(WIDTH, HEIGHT);
    background = al_load_bitmap("praia.png");
    al_draw_bitmap(background, 0, 0, 0);
    al_flip_display();



    



    return 0;
}