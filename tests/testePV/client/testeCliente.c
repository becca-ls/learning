#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "client.h"
#include "core.h"


#define MSG_MAX_SIZE 50
#define FPS 60

#define TILE_HEIGHT 64
#define TILE_WIDTH 64
#define HEIGHT 1024
#define WIDTH 1280

bool inicializa()
{
    if (!al_init())
    {
        puts("Incapaz de iniciar o allegro");
        exit(1);
    }
    if (!al_init_image_addon())
    {
        puts("Incapaz de iniciar o image addon");
        exit(1);
    }
    if (!al_install_keyboard())
    {
        puts("Incapaz de instalar o keyboard addon");
        exit(1);
    }

    if (!al_init_primitives_addon())
    {
        puts("Incapaz de iniciar o primitives addon");
        exit(1);
    }

    return true;
}

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

    printf("Digite um nick com %d caracteres: \n", NICK_SIZE);
    scanf("%s", nick);

    while ((int)strlen(nick) > 3)
    {
        printf("Digite um nick com %d caracteres: \n", NICK_SIZE);
        scanf(" %s", nick);
    }

    sendMsgToServer(nick, (int)strlen(nick) + 1);
}

void draw(Game j, ALLEGRO_BITMAP *bitmap, ALLEGRO_BITMAP *personagem, ALLEGRO_BITMAP *oil)
{
    al_draw_bitmap(bitmap, 0, 0, 0);
    
    if(checkOil(j.oil_a))
        al_draw_bitmap(oil, j.oil_a.pos.x*TILE_WIDTH, j.oil_a.pos.y*TILE_HEIGHT, 0);
    if(checkOil(j.oil_b))
        al_draw_bitmap(oil, j.oil_b.pos.x*TILE_WIDTH, j.oil_b.pos.y*TILE_HEIGHT , 0);
    if(checkOil(j.oil_c))
        al_draw_bitmap(oil, j.oil_c.pos.x*TILE_WIDTH, j.oil_c.pos.y*TILE_HEIGHT , 0);
    
    al_draw_bitmap(personagem, j.p1.pos.x*TILE_WIDTH, j.p1.pos.y*TILE_HEIGHT, 0);
    
    al_flip_display();
}

int main()
{
    Game jogo;
    //jogo.oleo = (Oleo *)malloc(NUMBER_OF_STAINS*sizeof(Oleo));
    conecta();
    recvMsgFromServer(&jogo, WAIT_FOR_IT);
    printPlayer(jogo.p1);
    printf("(%d, %d); (%d, %d); (%d, %d)\n", jogo.oil_a.pos.x, jogo.oil_a.pos.y, jogo.oil_b.pos.x, jogo.oil_b.pos.y, jogo.oil_c.pos.x, jogo.oil_c.pos.y);

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *background = NULL, *imgP = NULL, *imgOil = NULL;
    ALLEGRO_EVENT_QUEUE *evQueue = NULL;

    inicializa();

    display = al_create_display(WIDTH, HEIGHT);
    background = al_load_bitmap("praia.png");
    imgP = al_load_bitmap("irra.png");
    imgOil = al_load_bitmap("oil.png");
    evQueue = al_create_event_queue();

    al_register_event_source(evQueue, al_get_display_event_source(display));
    al_register_event_source(evQueue, al_get_keyboard_event_source());

    al_draw_bitmap(background, 0, 0, 0);
    al_flip_display();

    ALLEGRO_EVENT event;
    printf("opa\n");
    while (1)
    {
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 1 / FPS);
        al_wait_for_event_until(evQueue, &event, &timeout);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            Move acao;
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_W:
                acao.act = UP;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            case ALLEGRO_KEY_S:
                acao.act = DOWN;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            case ALLEGRO_KEY_A:
                acao.act = LEFT;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            case ALLEGRO_KEY_D:
                acao.act = RIGHT;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            default:
                break;
            }
        }
        recvMsgFromServer(&jogo, DONT_WAIT);
        printPlayer(jogo.p1);
        printf("(%d, %d); (%d, %d); (%d, %d)\n", jogo.oil_a.pos.x, jogo.oil_a.pos.y, jogo.oil_b.pos.x, jogo.oil_b.pos.y, jogo.oil_c.pos.x, jogo.oil_c.pos.y);
        draw(jogo, background, imgP, imgOil);
    }
    
    return 0;
}
