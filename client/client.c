#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "client.h"
#include "core.h"

#define MSG_MAX_SIZE 50
#define FPS 60

#define TILE_HEIGHT 64
#define TILE_WIDTH 64
#define HEIGHT 1024
#define WIDTH 1280

//largura e altura de cada sprite dentro da folha
int altura_sprite = 72, largura_sprite = 48;
//quantos sprites tem em cada linha da folha, e a atualmente mostrada
int colunas_folha = 3, coluna_atual = 0;
//quantos sprites tem em cada coluna da folha, e a atualmente mostrada
int linha_atual = 0, linhas_folha = 3;
//posicoes X e Y da folha de sprites que serao mostradas na tela
int regiao_x_folha = 0, regiao_y_folha = 0;
//quantos frames devem se passar para atualizar para o proximo sprite
int frames_sprite = 6, cont_frames = 0;
//posicao X Y da janela em que sera mostrado o sprite
int pos_x_sprite = 64, pos_y_sprite = 128;
//velocidade X Y que o sprite ira se mover pela janela

int menu(Game *jogo)
{
  // Atributos da tela
  const int LARGURA = 1280;
  const int ALTURA = 1024;

  ALLEGRO_DISPLAY *janela = NULL;
  ALLEGRO_EVENT_QUEUE *fila = NULL;
  ALLEGRO_BITMAP *botao_sair = NULL, *botao_jogar = NULL, *botao_tuto = NULL;
  ALLEGRO_BITMAP *maconheiro_de_mangue = NULL;
  ALLEGRO_FONT *fonte = NULL;
  ALLEGRO_AUDIO_STREAM *musica = NULL;
  ALLEGRO_FONT *fonte2 = NULL;
  //ALLEGRO_SAMPLE *sample = NULL;
  // Flag que condicionará nosso looping
  int sair = 0;
  int flagTuto = 0, flagJogo = 0;

  if (!al_init())
  {
    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    return -1;
  }

  al_init_font_addon();

  if (!al_init_ttf_addon())
  {
    fprintf(stderr, "Falha ao inicializar o addon ttf.\n");
    return -1;
  }
  if (!al_init_image_addon())
  {
    fprintf(stderr, "Falha ao inicializar o addon de imagem.\n");
    return -1;
  }

  if (!al_install_audio())
  {
    fprintf(stderr, "Falha ao inicializar áudio.\n");
    return false;
  }

  if (!al_init_acodec_addon())
  {
    fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
    return false;
  }

  if (!al_reserve_samples(5))
  {
    fprintf(stderr, "Falha ao alocar canais de áudio.\n");
    return false;
  }

  //CARREGANDO O ARQUIVO DE AUDIO

  /*  sample = al_load_sample("palmas.wav");
  if (!sample)
  {
    fprintf(stderr, "Falha ao carregar sample.\n");
    al_destroy_display(janela);
    return false;
  }*/

  musica = al_load_audio_stream("mus.ogg", 4, 1024);
  if (!musica)
  {
    fprintf(stderr, "Falha ao carregar audio.\n");
  }

  //Carrega aS fonteS
  fonte = al_load_font("4b.ttf", 30, 0);

  fonte2 = al_load_font("fast99.ttf", 150, 0);

  if (!fonte)
  {
    fprintf(stderr, "Falha ao carregar fonte.\n");
    return -1;
  }
  //Cria janela
  janela = al_create_display(LARGURA, ALTURA);
  if (!janela)
  {
    fprintf(stderr, "Falha ao criar janela.\n");
    return -1;
  }

  //Instalando teclado
  if (!al_install_keyboard())
  {
    fprintf(stderr, "Falha ao inicializar o teclado.\n");
    return false;
  }

  // Configura o título da janela
  al_set_window_title(janela, "menuzao");

  // Torna apto o uso de mouse na aplicação
  if (!al_install_mouse())
  {
    fprintf(stderr, "Falha ao inicializar o mouse.\n");
    al_destroy_display(janela);
    return -1;
  }
  // Carrega imagem
  maconheiro_de_mangue = al_load_bitmap("chico.jpg");

  al_draw_bitmap(maconheiro_de_mangue, 0, 0, 0);

  // Atribui o cursor padrão do sistema para ser usado
  if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
  {
    fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
    al_destroy_display(janela);
    return -1;
  }

  // Botao de jogar
  botao_jogar = al_create_bitmap(LARGURA / 4, ALTURA / 11);
  if (!botao_jogar)
  {
    fprintf(stderr, "Falha ao criar bitmap.\n");
    al_destroy_display(janela);
    return -1;
  }
  // Botao de tutorial
  botao_tuto = al_create_bitmap(LARGURA / 4, ALTURA / 11);
  if (!botao_jogar)
  {
    fprintf(stderr, "Falha ao criar bitmap.\n");
    al_destroy_bitmap(botao_jogar);
    al_destroy_display(janela);
    return -1;
  }

  // Botao de sair
  botao_sair = al_create_bitmap(LARGURA / 4, ALTURA / 11);
  if (!botao_sair)
  {
    fprintf(stderr, "Falha ao criar botão de saída.\n");
    al_destroy_bitmap(botao_jogar);
    al_destroy_bitmap(botao_tuto);
    al_destroy_display(janela);
    return -1;
  }

  fila = al_create_event_queue();
  if (!fila)
  {
    fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
    al_destroy_display(janela);
    return -1;
  }
  //SETANDO O AUDIO

  // Dizemos que vamos tratar os eventos vindos do mouse
  al_register_event_source(fila, al_get_mouse_event_source());
  //E teclado
  al_register_event_source(fila, al_get_keyboard_event_source());

  // Flag indicando se o mouse está sobre o retângulo central
  int jogar = 0, tuto = 0, sair_cor = 0;

  //SETANDO O AUDIO
  al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
  al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);

  while (!sair) // Coloca a condiçao para mudar o estado de prejogo pra jogando aqui
  {

    flagTuto = 0;
    flagJogo = 0;
    // Verificamos se há eventos na fila
    while (!al_is_event_queue_empty(fila))
    {
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila, &evento);

      // Se o evento foi de movimentação do mouse
      if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
      {
        // Verificamos se ele está sobre a região do botao jogar
        if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
            evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
            evento.mouse.y >= (2 * ALTURA) / 11 &&
            evento.mouse.y <= (3 * ALTURA) / 11)
        {
          jogar = 1;
        }
        else
        {
          jogar = 0;
        }
        // Verificamos se ele está sobre a região do botao de tutorial
        if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_tuto) / 2 &&
            evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_tuto) / 2 &&
            evento.mouse.y >= (5 * ALTURA) / 11 &&
            evento.mouse.y <= (6 * ALTURA) / 11)
        {
          tuto = 1;
        }
        else
        {
          tuto = 0;
        }
        // Verificamos se ele está sobre a região do botao jogar
        if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
            evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
            evento.mouse.y >= (8 * ALTURA) / 11 &&
            evento.mouse.y <= (9 * ALTURA) / 11)
        {
          sair_cor = 1;
        }
        else
        {
          sair_cor = 0;
        }
      }
      // Ou se o evento foi um clique do mouse
      else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
      {
        //Clique no botao jogar
        if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
            evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
            evento.mouse.y >= (2 * ALTURA) / 11 &&
            evento.mouse.y <= (3 * ALTURA) / 11)
        {
          int flagoso = 1;
          int tecla, pressed = 0; // tecla para guardar o numero e pressed pra checar se foi pressionado ou nn um numero válido
          int contador = 0;
          char caractere;
          char buffer[12];
          //LIMPA BUFFER;
          strcpy(buffer, "\0");

          while (!flagJogo)
          {
            //Tela de conexão com o server
            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_scaled_bitmap(maconheiro_de_mangue, 0, 0, al_get_bitmap_width(maconheiro_de_mangue), al_get_bitmap_height(maconheiro_de_mangue), 0, 0, LARGURA, ALTURA, 0);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 200, 0, "     DIGITE O IP DO SERVIDOR");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 550, 0, "IP:");
            al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
            if (flagoso == 1)
            {
              al_set_target_bitmap(botao_sair);
              al_clear_to_color(al_map_rgb(65, 105, 255));
              al_set_target_bitmap(al_get_backbuffer(janela));
              al_draw_bitmap(botao_sair, LARGURA / 2 - al_get_bitmap_width(botao_sair) / 2, (ALTURA * 8) / 11, 0);
              al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
              al_flip_display();
              flagoso = 0;
            }
            al_wait_for_event(fila, &evento);
            if (evento.type == ALLEGRO_EVENT_KEY_UP)
            {
              switch (evento.keyboard.keycode)
              {
              case ALLEGRO_KEY_0:
                tecla = 0;
                pressed = 1;
                break;
              case ALLEGRO_KEY_1:
                tecla = 1;
                pressed = 1;
                break;
              case ALLEGRO_KEY_2:
                tecla = 2;
                pressed = 1;
                break;
              case ALLEGRO_KEY_3:
                tecla = 3;
                pressed = 1;
                break;
              case ALLEGRO_KEY_4:
                tecla = 4;
                pressed = 1;
                break;
              case ALLEGRO_KEY_5:
                tecla = 5;
                pressed = 1;
                break;
              case ALLEGRO_KEY_6:
                tecla = 6;
                pressed = 1;
                break;
              case ALLEGRO_KEY_7:
                tecla = 7;
                pressed = 1;
                break;
              case ALLEGRO_KEY_8:
                tecla = 8;
                pressed = 1;
                break;
              case ALLEGRO_KEY_9:
                tecla = 9;
                pressed = 1;
                break;
              case ALLEGRO_KEY_BACKSPACE:
                tecla = -1;
                pressed = 1;
                break;
              case ALLEGRO_KEY_ENTER:
                tecla = -2;
                pressed = 1;
                break;
              case ALLEGRO_KEY_FULLSTOP:
                tecla = 10;
                pressed = 1;
                break;
              default:
                break;
              }
              printf("%d  %d %d\n", tecla, contador, pressed);
              if (pressed == 1 && tecla > -1 && contador < 11)
              {
                printf("buffer: %s\n", buffer);
                contador++;
                if (tecla < 10)
                  caractere = tecla + '0';
                else
                  caractere = '.';
                printf("caractere: %c\n", caractere);
                strcat(buffer, &caractere);
                caractere = buffer[strlen(buffer) - 1];
                if ((caractere >= 'a' && caractere <= 'z') || (caractere >= 'A' && caractere <= 'Z'))
                  buffer[strlen(buffer) - 1] = '\0';
                pressed = 0;
              }
              else if (pressed == 1 && tecla == -1)
              {

                //Apagando o ultimo caractere
                int i;
                for (i = 0; buffer[i] != '\0'; i++)
                {
                }
                if (i != 0)
                  buffer[i - 1] = '\0';
                pressed = 0;
                contador--;
              }
              else if (pressed == 1 && tecla == -2)
              {

                // TRATAR O BUFFER PARA POR NA ENTRADA FORMATADA DA FUNÇÃO
                //QUANDO NÃO CONSEGUIR SE CONECTAR
                enum conn_ret_t K;
                if (buffer[0] == '0')
                  K = connectToServer(NULL);
                else
                  K = connectToServer(buffer);
                if (K != SERVER_UP)
                {
                  al_draw_scaled_bitmap(maconheiro_de_mangue, 0, 0, al_get_bitmap_width(maconheiro_de_mangue), al_get_bitmap_height(maconheiro_de_mangue), 0, 0, LARGURA, ALTURA, 0);
                  al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 200, 0, "     FALHA AO CONECTAR");
                  al_flip_display();
                  al_rest(3.0);
                  strcpy(buffer, "\0");
                  contador = 0;
                  pressed = 0;
                }
                else
                {
                  //######   TELA DO NICK   ########
                  int flagnick = 0;
                  //Cria e limpa o buffer
                  char buf2[20];
                  strcpy(buf2, "\0");
                  int flagoso2;
                  while (!flagnick)
                  {

                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_scaled_bitmap(maconheiro_de_mangue, 0, 0, al_get_bitmap_width(maconheiro_de_mangue), al_get_bitmap_height(maconheiro_de_mangue), 0, 0, LARGURA, ALTURA, 0);
                    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 200, 0, "     DIGITE O SEU NICKNAME");
                    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 550, 0, "NICK:");
                    al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
                    if (flagoso2 == 1)
                    {

                      al_set_target_bitmap(botao_sair);
                      al_clear_to_color(al_map_rgb(65, 105, 255));
                      al_set_target_bitmap(al_get_backbuffer(janela));
                      al_draw_bitmap(botao_sair, LARGURA / 2 - al_get_bitmap_width(botao_sair) / 2, (ALTURA * 8) / 11, 0);
                      al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
                      al_flip_display();
                      flagoso2 = 0;
                    }
                    al_wait_for_event(fila, &evento);
                    if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
                    { //SE FOR CHAR, BOTA NO BUFFER2
                      int key = evento.keyboard.unichar;
                      if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z'))
                      {
                        if (key >= 'a' && key <= 'z')
                          key = key - ('a' - 'A');
                        char letra = key;
                        printf("letra: %c\n numero: %d\n", letra, letra);
                        strcat(buf2, &letra);
                        printf("buf2: %s\n", buf2);
                      }
                      else if (key == 8)
                      { //BACKSPACE
                        int i;
                        printf("letra: %c\n numero: %d\n", key, key);
                        for (i = 0; buf2[i] != '\0'; i++)
                        {
                        }
                        if (i != 0)
                          buf2[i - 1] = '\0';
                      }
                      else if (key == 13)
                      { //ENTER
                        //MANDA MSG PRO SERVER AQ
                        int superflag = 0;
                        sendMsgToServer(buf2, (int)strlen(buf2) + 1);
                        recvMsgFromServer(jogo, WAIT_FOR_IT);
                        while (jogo->game_state == PREGAME)
                        {
                          al_clear_to_color(al_map_rgb(0, 0, 0));
                          al_draw_scaled_bitmap(maconheiro_de_mangue, 0, 0, al_get_bitmap_width(maconheiro_de_mangue), al_get_bitmap_height(maconheiro_de_mangue), 0, 0, LARGURA, ALTURA, 0);
                          al_set_target_bitmap(al_get_backbuffer(janela));
                          al_draw_text(fonte, al_map_rgb(0, 0, 0), (LARGURA / 8), ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "ESPERANDO SEGUNDO JOGADOR");
                          puts("Esperando segundo jogador");
                          al_flip_display();
                          if (recvMsgFromServer(jogo, WAIT_FOR_IT) == SERVER_DISCONNECTED)
                          {
                            al_clear_to_color(al_map_rgb(0, 0, 0));
                            al_draw_scaled_bitmap(maconheiro_de_mangue, 0, 0, al_get_bitmap_width(maconheiro_de_mangue), al_get_bitmap_height(maconheiro_de_mangue), 0, 0, LARGURA, ALTURA, 0);
                            al_set_target_bitmap(al_get_backbuffer(janela));
                            al_draw_text(fonte, al_map_rgb(0, 0, 0), (LARGURA / 4), ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "DESCONECTADO");
                            al_flip_display();
                            al_rest(3.0);
                            superflag = 1;
                            break;
                          }
                        }
                        if (!superflag)
                        {
                          //DESALOCANDO AS TRETAS
                          al_detach_audio_stream(musica);
                          al_destroy_bitmap(botao_sair);
                          al_destroy_bitmap(botao_jogar);
                          al_destroy_bitmap(botao_tuto);
                          al_destroy_bitmap(maconheiro_de_mangue);
                          al_destroy_display(janela);
                          al_unregister_event_source(fila, al_get_mouse_event_source());
                          al_destroy_event_queue(fila);
                          al_destroy_audio_stream(musica);
                          al_destroy_font(fonte);
                          al_destroy_font(fonte2);
                          return 1; // RETORNO PARA INICIAR O JOGO
                        }
                        else
                          break;
                      }
                    }
                    else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
                    {

                      if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                          evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                          evento.mouse.y >= (8 * ALTURA) / 11 &&
                          evento.mouse.y <= (9 * ALTURA) / 11)
                      {
                        al_set_target_bitmap(botao_sair);
                        al_clear_to_color(al_map_rgb(255, 0, 0));
                      }
                      else
                      {
                        al_set_target_bitmap(botao_sair);
                        al_clear_to_color(al_map_rgb(65, 105, 255));
                      }
                    }
                    else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                      if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                          evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                          evento.mouse.y >= (8 * ALTURA) / 11 &&
                          evento.mouse.y <= (9 * ALTURA) / 11)
                      {
                        flagnick = 1;
                        strcpy(buf2, "\0");
                        strcpy(buffer, "\0");
                      }
                    }
                    al_set_target_bitmap(al_get_backbuffer(janela));
                    al_draw_bitmap(botao_sair, LARGURA / 2 - al_get_bitmap_width(botao_sair) / 2, (ALTURA * 8) / 11, 0);
                    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 4, 550, 0, buf2);
                    al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
                    al_flip_display();
                  }
                  pressed = 0;
                  contador = 0;
                }
              }
            }
            else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {

              if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.y >= (8 * ALTURA) / 11 &&
                  evento.mouse.y <= (9 * ALTURA) / 11)
              {
                al_set_target_bitmap(botao_sair);
                al_clear_to_color(al_map_rgb(255, 0, 0));
              }
              else
              {
                al_set_target_bitmap(botao_sair);
                al_clear_to_color(al_map_rgb(65, 105, 255));
              }
            }
            else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
              if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.y >= (8 * ALTURA) / 11 &&
                  evento.mouse.y <= (9 * ALTURA) / 11)
              {
                flagJogo = 1;
              }
            }

            al_set_target_bitmap(al_get_backbuffer(janela));
            al_draw_bitmap(botao_sair, LARGURA / 2 - al_get_bitmap_width(botao_sair) / 2, (ALTURA * 8) / 11, 0);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8 + 50, 550, 0, buffer);
            al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
            al_flip_display();
          }
        }
        //Clique no botao tutorial
        else if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                 evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                 evento.mouse.y >= (5 * ALTURA) / 11 &&
                 evento.mouse.y <= (6 * ALTURA) / 11)
        {
          // printf("vai rebola pro pai\n");
          int flag = 1;
          // al_set_audio_stream_playing(musica, 0);
          while (!flagTuto)
          {

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_scaled_bitmap(maconheiro_de_mangue, 0, 0, al_get_bitmap_width(maconheiro_de_mangue), al_get_bitmap_height(maconheiro_de_mangue), 0, 0, LARGURA, ALTURA, 0);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 200, 0, "VOCE EH UM VOLUNTARIO CATANDO OLEO NA MAO!");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 250, 0, "PARABENS POR COLOCAR EM RISCO A SUA SAUDE ");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 300, 0, "PELO MEIO AMBIENTE!");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 400, 0, "                USE WASD PARA SE MOVER    ");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 8, 550, 0, "                           DIVIRTA-SE     ");
            al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
            if (flag == 1)
            {
              al_set_target_bitmap(botao_sair);
              al_clear_to_color(al_map_rgb(65, 105, 255));
              al_set_target_bitmap(al_get_backbuffer(janela));
              al_draw_bitmap(botao_sair, LARGURA / 2 - al_get_bitmap_width(botao_sair) / 2, (ALTURA * 8) / 11, 0);
              al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
              al_flip_display();
              flag = 0;
            }
            al_wait_for_event(fila, &evento);
            if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
              if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.y >= (8 * ALTURA) / 11 &&
                  evento.mouse.y <= (9 * ALTURA) / 11)
              {
                al_set_target_bitmap(botao_sair);
                al_clear_to_color(al_map_rgb(255, 0, 0));
              }
              else
              {
                al_set_target_bitmap(botao_sair);
                al_clear_to_color(al_map_rgb(65, 105, 255));
              }
            }
            else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
              if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                  evento.mouse.y >= (8 * ALTURA) / 11 &&
                  evento.mouse.y <= (9 * ALTURA) / 11)
              {
                flagTuto = 1;
              }
              else
              {
                al_set_target_bitmap(botao_sair);
                al_clear_to_color(al_map_rgb(65, 105, 255));
              }
            }
            al_set_target_bitmap(al_get_backbuffer(janela));
            al_draw_bitmap(botao_sair, LARGURA / 2 - al_get_bitmap_width(botao_sair) / 2, (ALTURA * 8) / 11, 0);
            al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");
            al_flip_display();
          }
          //al_set_audio_stream_playing(musica, 1);
          //sair = 1;
        }

        else if (evento.mouse.x >= LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                 evento.mouse.x <= LARGURA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                 evento.mouse.y >= (8 * ALTURA) / 11 &&
                 evento.mouse.y <= (9 * ALTURA) / 11)
        {
          sair = 1;
        }
      }
    }

    // Limpamos a tela
    al_clear_to_color(al_map_rgb(255, 165, 0));
    al_draw_scaled_bitmap(maconheiro_de_mangue, 0, 0, al_get_bitmap_width(maconheiro_de_mangue), al_get_bitmap_height(maconheiro_de_mangue), 0, 0, LARGURA, ALTURA, 0);

    // Colorimos o bitmap correspondente ao botao jogar,
    // com a cor condicionada ao conteúdo da flag jogar
    al_set_target_bitmap(botao_jogar);
    if (!jogar)
    {
      al_clear_to_color(al_map_rgb(65, 105, 255));
    }
    else
    {
      al_clear_to_color(al_map_rgb(238, 173, 45));
    }

    //Colorindo botao tuto

    al_set_target_bitmap(botao_tuto);
    if (!tuto)
    {
      al_clear_to_color(al_map_rgb(65, 105, 255));
    }
    else
    {
      al_clear_to_color(al_map_rgb(238, 173, 45));
    }
    //Colorindo botao sair
    al_set_target_bitmap(botao_sair);
    if (!sair_cor)
    {
      al_clear_to_color(al_map_rgb(65, 105, 255));
    }
    else
    {
      al_clear_to_color(al_map_rgb(255, 0, 0));
    }

    // Desenhamos os retângulos na tela
    al_set_target_bitmap(al_get_backbuffer(janela));
    al_draw_bitmap(botao_jogar, LARGURA / 2 - al_get_bitmap_width(botao_jogar) / 2,
                   (ALTURA * 2) / 11, 0);
    al_draw_bitmap(botao_tuto, LARGURA / 2 - al_get_bitmap_width(botao_tuto) / 2,
                   (ALTURA * 5) / 11, 0);
    al_draw_bitmap(botao_sair, LARGURA / 2 - al_get_bitmap_width(botao_sair) / 2,
                   (ALTURA * 8) / 11, 0);

    // Escrevendo as caixas de dialogos
    //TITULO
    al_draw_text(fonte2, al_map_rgb(255, 255, 0), (LARGURA / 7.8), ((ALTURA) / 60), 0, "PRAIA LIMPA");
    //BOTAO JOGAR
    al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 45, ((ALTURA * 2) / 11) + al_get_bitmap_height(botao_jogar) / 4, 0, "JOGAR");
    //BOTAO TUTO
    al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 70, ((ALTURA * 5) / 11) + al_get_bitmap_height(botao_tuto) / 4, 0, "TUTORIAL");
    //BOTAO SAIR
    al_draw_text(fonte, al_map_rgb(255, 255, 255), (LARGURA / 2) - 40, ((ALTURA * 8) / 11) + al_get_bitmap_height(botao_sair) / 4, 0, "SAIR");

    // Atualiza a tela
    al_flip_display();
  }

  // Desaloca os recursos utilizados na aplicação
  al_detach_audio_stream(musica);
  al_destroy_bitmap(botao_sair);
  al_destroy_bitmap(botao_jogar);
  al_destroy_bitmap(botao_tuto);
  al_destroy_bitmap(maconheiro_de_mangue);
  al_destroy_display(janela);
  al_destroy_font(fonte);
  al_destroy_font(fonte2);
  al_unregister_event_source(fila, al_get_mouse_event_source());
  al_destroy_event_queue(fila);
  al_destroy_audio_stream(musica);

  return 0; // RETORNO PARA SAIR DO JOGO
}

bool inicializa()
{
  if (!al_init())
  {
    puts("Incapaz de iniciar o allegro");
    exit(1);
  }
  if (!al_init_ttf_addon())
  {
    puts("Incapaz de iniciar o ttf addon");
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

<<<<<<< HEAD
void draw(Game j, ALLEGRO_BITMAP *bitmap, ALLEGRO_BITMAP *p1, ALLEGRO_BITMAP *p2, ALLEGRO_BITMAP *oil, ALLEGRO_FONT *fonte)
=======
void draw(Game j, ALLEGRO_BITMAP *bitmap, ALLEGRO_BITMAP *personagem1, ALLEGRO_BITMAP *personagem2, ALLEGRO_BITMAP *oil, ALLEGRO_FONT *fonte)
>>>>>>> 229a151466fd776ea822e7b9df7a2c35b0a6ce20
{
  al_draw_bitmap(bitmap, 0, 0, 0);
  al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10, 0, ALLEGRO_ALIGN_LEFT, "PONTUACAO: p1 %d X p2 %d", j.p1.oleo_points * 10, j.p2.oleo_points * 10);
  if (checkOil(j.oil_a))
    al_draw_bitmap(oil, j.oil_a.pos.x * TILE_WIDTH, j.oil_a.pos.y * TILE_HEIGHT, 0);
  if (checkOil(j.oil_b))
    al_draw_bitmap(oil, j.oil_b.pos.x * TILE_WIDTH, j.oil_b.pos.y * TILE_HEIGHT, 0);
  if (checkOil(j.oil_c))
    al_draw_bitmap(oil, j.oil_c.pos.x * TILE_WIDTH, j.oil_c.pos.y * TILE_HEIGHT, 0);

<<<<<<< HEAD
  al_draw_bitmap_region(p1, j.p1.sprites.x*largura_sprite, j.p1.sprites.y*altura_sprite, largura_sprite, altura_sprite, j.p1.pos.x * TILE_WIDTH, j.p1.pos.y * TILE_HEIGHT, 0);
  al_draw_bitmap_region(p2, j.p2.sprites.x*largura_sprite, j.p2.sprites.y*altura_sprite, largura_sprite, altura_sprite, j.p2.pos.x * TILE_WIDTH, j.p2.pos.y * TILE_HEIGHT, 0);
=======
  al_draw_bitmap_region(personagem1,  regiao_x_folha, regiao_y_folha, largura_sprite, altura_sprite, j.p1.pos.x * TILE_WIDTH, j.p1.pos.y * TILE_HEIGHT, 0);
  al_draw_bitmap_region(personagem2,  regiao_x_folha, regiao_y_folha, largura_sprite, altura_sprite, j.p2.pos.x * TILE_WIDTH, j.p2.pos.y * TILE_HEIGHT, 0);
>>>>>>> 229a151466fd776ea822e7b9df7a2c35b0a6ce20
  al_flip_display();
}

int main()
{
  Game jogo;
  //jogo.oleo = (Oleo *)malloc(NUMBER_OF_STAINS*sizeof(Oleo));
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_BITMAP *background = NULL, *imgP1 = NULL, *imgP2 = NULL, *imgOil = NULL;
  ALLEGRO_EVENT_QUEUE *evQueue = NULL;
  ALLEGRO_FONT *fonte = NULL;
  if (menu(&jogo))
  {
    printPlayer(jogo.p1);
    printf("(%d, %d); (%d, %d); (%d, %d)\n", jogo.oil_a.pos.x, jogo.oil_a.pos.y, jogo.oil_b.pos.x, jogo.oil_b.pos.y, jogo.oil_c.pos.x, jogo.oil_c.pos.y);

    inicializa();
    recvMsgFromServer(&jogo, DONT_WAIT);
    display = al_create_display(WIDTH, HEIGHT);
    background = al_load_bitmap("praia.png");
    imgP1 = al_load_bitmap(jogo.p1.skin);
    imgP2 = al_load_bitmap(jogo.p2.skin);
    imgOil = al_load_bitmap("oil.png");
    evQueue = al_create_event_queue();
    fonte = al_load_font("arial.ttf", 20, 0);

    al_register_event_source(evQueue, al_get_display_event_source(display));
    al_register_event_source(evQueue, al_get_keyboard_event_source());

    al_draw_bitmap(background, 0, 0, 0);
    al_flip_display();
    int k;
    ALLEGRO_EVENT event;
    printf("opa\n");
    while (jogo.game_state == INGAME)
    {
      ALLEGRO_TIMEOUT timeout;
      al_init_timeout(&timeout, 1 / FPS);
      al_wait_for_event_until(evQueue, &event, &timeout);
      cont_frames++;
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        break;

      if (event.type == ALLEGRO_EVENT_KEY_DOWN)
      {
        Move acao;
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_W:
          acao.act = UP;
          jogo.p1.sprites.y=3;
          sendMsgToServer((Move *)&acao, sizeof(Move));
          break;
        case ALLEGRO_KEY_S:
          acao.act = DOWN;
          jogo.p1.sprites.y=0;
          sendMsgToServer((Move *)&acao, sizeof(Move));
          break;
        case ALLEGRO_KEY_A:
          acao.act = LEFT;
          jogo.p1.sprites.y=1;
          sendMsgToServer((Move *)&acao, sizeof(Move));
          break;
        case ALLEGRO_KEY_D:
          acao.act = RIGHT;
          jogo.p1.sprites.y=2;
          sendMsgToServer((Move *)&acao, sizeof(Move));
          break;
        default:
          break;
        }
      }
     

      recvMsgFromServer(&jogo, DONT_WAIT);
      regiao_x_folha = jogo.p1.sprites.x * largura_sprite;
      regiao_y_folha = jogo.p1.sprites.y * altura_sprite;
      printPlayer(jogo.p1);
      printf("(%d, %d); (%d, %d); (%d, %d)\n", jogo.oil_a.pos.x, jogo.oil_a.pos.y, jogo.oil_b.pos.x, jogo.oil_b.pos.y, jogo.oil_c.pos.x, jogo.oil_c.pos.y);
<<<<<<< HEAD
      draw(jogo, background, imgP1, imgP2, imgOil, fonte);
=======
      draw(jogo, background, imgP1,imgP2, imgOil, fonte);
>>>>>>> 229a151466fd776ea822e7b9df7a2c35b0a6ce20
    }
  }

  if (jogo.game_state == POSGAME)
  {
    char texto[100] = "O VENCEDOR EH ";
    if (jogo.p1.oleo_points > jogo.p2.oleo_points)
      strcat(texto, jogo.p1.nick);
    else
      strcat(texto, jogo.p2.nick);
    int allign = (int)strlen(texto);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    fonte = al_load_ttf_font("4b.ttf", 38, 0);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, texto);
    al_flip_display();
    al_rest(10);
  }

  al_destroy_bitmap(imgOil);
  al_destroy_bitmap(imgP1);
  al_destroy_bitmap(imgP2);
  al_destroy_bitmap(background);
  al_destroy_font(fonte);
  al_destroy_display(display);
  al_destroy_event_queue(evQueue);

  return 0;
}
