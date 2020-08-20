/*
 * game.cpp
 *
 *  Created on: 29/1/2016
 *      Author: Usuario
 */

#include <stdio.h>
#include <allegro.h>
#include <allegro5/allegro_primitives.h> // Addon de primitivas (figuras)
#include <allegro5/allegro_image.h> // Addon de imagenes
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#include "bool.h"
#include "actormanager.h"
#include "stagemanager.h"
#include "collisionmanager.h"
#include "game.h"
using namespace std;

int Game::gfx_w;
int Game::gfx_h;

Game::Game()
{
	actor_manager=NULL;
	stage_manager=NULL;
	control_manager=NULL;
	collision_manager=NULL;
	level_manager=NULL;
	tick = 0;
}
Game::~Game()
{

}
void Game::init(void)
{
	// Inicializar los addons
	if(!al_init())
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
					                   "error fatal", "Error al inicializar allegro",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}

	if(!al_init_primitives_addon())
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
					                   "error fatal", "Error al inicializar addon de primitivas",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}

	if(!al_init_image_addon())
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
					                   "error fatal", "Error al inicializar addon de imagenes",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}

	if(!al_install_keyboard())
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
					                   "error fatal", "Error al inicializar el teclado",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}

	if(!al_install_audio())
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
									   "error fatal", "Error al inicializar el audio",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}

	if(!al_init_acodec_addon())
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
									   "error fatal", "Error al inicializar el acodec",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}


	if (!al_reserve_samples(40))
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
									   "error fatal", "Error al reservar samples",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}


	 al_init_font_addon();

	if(!al_init_ttf_addon())
	{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
									   "error fatal", "Error al inicializar fonts ttf",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
	}

	create_actormanager();
	create_stagemanager();
	create_controlmanager();
	create_collisionmanager();
	create_levelmanager();

	//creamos una cola a traves de la cual nos notificarán todos los eventos que ocurran
	event_queue = al_create_event_queue();

	//registramos la ventana como fuente de eventos
	al_register_event_source(event_queue, al_get_display_event_source(stage_manager->get_display()));

	//timer = al_create_timer(1.0 / 60);
	timer = al_create_timer(1.0/70); //nos genera 70 ticks por segundo

	//registramos el timer como fuente de eventos
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//registramos el teclado como fuente de eventos
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	start();
}

void Game::shutdown(string message)
{
	if (actor_manager) delete actor_manager;
	if (stage_manager) delete stage_manager;
	if (control_manager) delete control_manager;
	if (collision_manager) delete collision_manager;
//	if (level_manager) delete level_manager;

	al_shutdown_primitives_addon(); // Finalizar el addon de primitivas
	al_shutdown_image_addon(); // Finalizar el addon de imagenes
	al_destroy_event_queue(event_queue); //Destruir la cola de eventos


}
void Game::create_actormanager()
{
	actor_manager = new ActorManager(this);
}
void Game::create_stagemanager()
{
	stage_manager = new StageManager(this, gfx_w, gfx_h,name);
}

void Game::create_controlmanager()
{
	control_manager = new ControlManager();
}
void Game::create_collisionmanager()
{
	collision_manager = new CollisionManager(this);
}
void Game::create_levelmanager()
{
	level_manager = new LevelManager(this);
}
void Game::start()
{
	main();
	shutdown();
}
void Game::main()
{
	//while (!key[KEY_ESC]);
	al_start_timer(timer);
	while (TRUE)
	{
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			update();
		}
	}
}
void Game::set_name(string n)
{
	name=n;
}
void Game::set_ancho(int an)
{
	gfx_w=an;
}

void Game::set_alto(int al)
{
	gfx_h=al;
}
string Game::get_name()
{
	return name;
}
void Game::update()
{
	tick++;

	if (control_manager) control_manager->update();

	if (tick > 10){
		if (collision_manager) collision_manager->update();
		if (actor_manager) actor_manager->update();
		if (stage_manager) stage_manager->update();
		tick = 0;
	}
}
int Game::rnd (int rb, int ra)
{

	// variable = limite_inferior + rand() % (limite_superior +1 - limite_inferior) ;

	int retorno;
	retorno = rb + rand() % (ra+1 -rb);
	return (retorno);
}

