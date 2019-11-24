#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
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

ALLEGRO_BITMAP *folha_sprite = NULL;

 //largura e altura de cada sprite dentro da folha
    int altura_sprite=72, largura_sprite=48;
    //quantos sprites tem em cada linha da folha, e a atualmente mostrada
    int colunas_folha=4, coluna_atual=0;
    //quantos sprites tem em cada coluna da folha, e a atualmente mostrada
    int linha_atual=0, linhas_folha=4;
    //posicoes X e Y da folha de sprites que serao mostradas na tela
    int regiao_x_folha=0, regiao_y_folha=0;
    //quantos frames devem se passar para atualizar para o proximo sprite
    int frames_sprite=6, cont_frames=0;
    //posicao X Y da janela em que sera mostrado o sprite
    int pos_x_sprite=64, pos_y_sprite=128;
    //velocidade X Y que o sprite ira se mover pela janela
  



bool inicializa()
{
    if (!al_init_font_addon()){
        puts("Erro ao inicializar o font addon");
        exit(1);
    }
    if (!al_init_ttf_addon()){
        puts("Erro ao inicializar o ttf addon");
        exit(1);
    }
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

void draw(Game j, ALLEGRO_BITMAP *bitmap, ALLEGRO_BITMAP *personagem, ALLEGRO_BITMAP *oil, ALLEGRO_FONT *fonte, int score)
{
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10, 0, ALLEGRO_ALIGN_LEFT, "Pontuaçao: %d", score);
    
    if(checkOil(j.oil_a))
        al_draw_bitmap(oil, j.oil_a.pos.x*TILE_WIDTH, j.oil_a.pos.y*TILE_HEIGHT, 0);
    if(checkOil(j.oil_b))
        al_draw_bitmap(oil, j.oil_b.pos.x*TILE_WIDTH, j.oil_b.pos.y*TILE_HEIGHT , 0);
    if(checkOil(j.oil_c))
        al_draw_bitmap(oil, j.oil_c.pos.x*TILE_WIDTH, j.oil_c.pos.y*TILE_HEIGHT , 0);
     al_draw_bitmap_region(personagem,
                    regiao_x_folha,regiao_y_folha,
                    largura_sprite,altura_sprite,
                     j.p1.pos.x*TILE_WIDTH,j.p1.pos.y*TILE_HEIGHT,0);
    
    al_flip_display();
}

int main()
{

    int score = 0;
    Game jogo;
    //jogo.oleo = (Oleo *)malloc(NUMBER_OF_STAINS*sizeof(Oleo));
    conecta();
    recvMsgFromServer(&jogo, WAIT_FOR_IT);
    printPlayer(jogo.p1);
    printf("(%d, %d); (%d, %d); (%d, %d)\n", jogo.oil_a.pos.x, jogo.oil_a.pos.y, jogo.oil_b.pos.x, jogo.oil_b.pos.y, jogo.oil_c.pos.x, jogo.oil_c.pos.y);

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *background = NULL, *imgP = NULL, *imgOil = NULL;
    ALLEGRO_EVENT_QUEUE *evQueue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *fonte = NULL;
    
    inicializa();

    display = al_create_display(WIDTH, HEIGHT);
    background = al_load_bitmap("praia.png");
    imgP = al_load_bitmap("irra.png");
    imgOil = al_load_bitmap("oil.png");
    evQueue = al_create_event_queue();
    folha_sprite = al_load_bitmap("chrono.png");
    fonte = al_load_font("arial.ttf", 20, 0);
    
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
                linha_atual=3;
                coluna_atual=0;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            case ALLEGRO_KEY_S:
                acao.act = DOWN;
                linha_atual=0;
                coluna_atual=0;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            case ALLEGRO_KEY_A:
                acao.act = LEFT;
                linha_atual=1;
                coluna_atual=0;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            case ALLEGRO_KEY_D:
                acao.act = RIGHT;
                linha_atual=2;
                coluna_atual=0;
                sendMsgToServer((Move *)&acao, sizeof(Move));
                break;
            default:
                break;
            }
        }
        regiao_x_folha = coluna_atual * largura_sprite;
        regiao_y_folha = linha_atual * altura_sprite;
        //JOGO É JOGO , USA JOGO QUANDO FOR USAR ALGO RELACIONADO AO JOGO 
        recvMsgFromServer(&jogo, DONT_WAIT);
        printPlayer(jogo.p1);
        printf("(%d, %d); (%d, %d); (%d, %d)\n", jogo.oil_a.pos.x, jogo.oil_a.pos.y, jogo.oil_b.pos.x, jogo.oil_b.pos.y, jogo.oil_c.pos.x, jogo.oil_c.pos.y);
        score = jogo.p1.oleo_points;
        draw(jogo, background,folha_sprite, imgOil, fonte, score);
        if(jogo.p1.game_result != -1){
            if(jogo.p1.game_result == 1){
                background = al_load_bitmap("win.jpg");
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(background, 110, 80, 0);
                fonte = al_load_font("arial.ttf", 100, 0);
                al_draw_text(fonte, al_map_rgb(0, 0, 255), 640, 512, ALLEGRO_ALIGN_CENTER, "Voce venceu!");
                al_flip_display();
                al_rest(10.0);
                exit(1);
            }
            else if(jogo.p1.game_result == 0){
                background = al_load_bitmap("lose.jpg");
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(background, 110, 80, 0);
                fonte = al_load_font("arial.ttf", 100, 0);
                al_draw_text(fonte, al_map_rgb(0, 0, 255), 640, 512, ALLEGRO_ALIGN_CENTER, "Voce perdeu!");
                al_flip_display();
                al_rest(10.0);
                exit(1);
            }
        }
    }
    
    return 0;
}
