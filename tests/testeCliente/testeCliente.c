#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <allegro5/allegro.h>



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

    return true;
}

int main()
{

    return 0;
}