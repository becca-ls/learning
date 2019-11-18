#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>


//variaveis globsais
enum teclas {CIMA,BAIXO,DIREITA,ESQUERDA};

const int largura_t = 640;
const int altura_t = 480;

//funcoes
void initpraiero(voluntario *praiero,ALLEGRO_BITMAP *image){
    praiero->x = 100;
    praiero->y = 100;
    praiero->id = jogador;
    praiero->vida = 100;
    praiero->velocidade = 5;
    praiero->borda_x = 6;
    praiero->borda_y = 6;
    praiero->pontos = 0;

    praiero->maxframe = 3;
    praiero->curframe = 0;

    praiero->image = image;
}



int main(){

//variaveis do game:
  int pos_x = 100;
  int pos_y = 100;

  char fim=0;
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

  char teclas[] = {0,0,0,0};

  int fps=60;

  ALLEGRO_TIMER *timer = NULL;


//incializacao de objetos

  voluntario praiero;

// inicializacao do allegro e display:
  ALLEGRO_DISPLAY *display=NULL;
  ALLEGRO_BITMAP *praieroimage;

  if(!al_init()){
   fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    return -1;
  }
  display = al_create_display(largura_t,altura_t);

  if(!display){
    fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
    return -1;
  }

// incializacao de addons e instalacoes
  al_install_keyboard();
  al_install_mouse();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_image_addon();
  timer = al_create_timer(1.0/fps);

  
//criacao da fila eventos 

  fila_eventos = al_create_event_queue();

//registro de source(da onde vem os eventos)

al_register_event_source(fila_eventos,al_get_keyboard_event_source());
al_register_event_source(fila_eventos,al_get_display_event_source(display));
al_register_event_source(fila_eventos,al_get_mouse_event_source());
al_register_event_source(fila_eventos,al_get_timer_event_source(timer));

//funcoes inciais

initpraiero(&praiero,praieroimage);


//loop principal;

al_start_timer(timer);

while(!fim){
  ALLEGRO_EVENT ev;
  al_wait_for_event(fila_eventos,&ev);//nao faz nada ate algum evento acontecer

  //eventos e logica
  if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
    if(ev.keyboard.keycode== ALLEGRO_KEY_ESCAPE){
      fim=1;
    }
    switch(ev.keyboard.keycode)
    {
      case ALLEGRO_KEY_UP:
       char key = UP_ARROW;
       sendMsgToServer((char *)&key,1);
       break;
      case ALLEGRO_KEY_DOWN:
        char key = UP_DOWN;
        sendMsgToServer((char *)&key,1);
        break;
      case ALLEGRO_KEY_RIGHT:
        char key = UP_RIGHT;
        sendMsgToServer((char *)&key,1);
        break;
      case ALLEGRO_KEY_LEFT:
        char key = KEY_LEFT;
        sendMsgToServer((char *)&key,1);
        break;
    }

    recv()
  }

  if(ev.type == ALLEGRO_EVENT_KEY_UP){
       switch(ev.keyboard.keycode)
    {
      case ALLEGRO_KEY_UP:
        teclas[CIMA]=0;
        break;
      case ALLEGRO_KEY_DOWN:
        teclas[BAIXO]=0;
        break;
      case ALLEGRO_KEY_RIGHT:
        teclas[DIREITA]=0;
        break;
      case ALLEGRO_KEY_LEFT:
        teclas[ESQUERDA]=0;
        break;
    }
  }

 else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
    fim = 1;
  }


  else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){
    
  }

  else if(ev.type == ALLEGRO_EVENT_TIMER){
    pos_y-=teclas[CIMA]*10;
    pos_y+=teclas[BAIXO]*10;
    pos_x+=teclas[DIREITA]*10;
    pos_x-=teclas[ESQUERDA]*10;
  }

//DESENHOSS
  if(al_is_event_queue_empty(fila_eventos)){
    al_draw_filled_rectangle(pos_x,pos_y,pos_x+30,pos_y+30,al_map_rgb(255,255,0));
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));
  }

}
//finalizar o programa

  al_destroy_display(display);
  al_destroy_event_queue(fila_eventos);
  al_destroy_timer(timer);
  al_destroy_bitmap(praieroimage);


}