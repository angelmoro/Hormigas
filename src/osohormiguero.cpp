/*
 * osohormiguero.cpp
 *
 *  Created on: 30/1/2016
 *      Author: Usuario
 */

#include <stdio.h>
#include <allegro.h>
#include <allegro5/allegro_native_dialog.h>

#include "bool.h"
#include "osohormiguero.h"
#include "game.h"
#include "sprite.h"
#include "records.h"
#include "level.h"

OsoHormiguero::OsoHormiguero(ActorManager * b,Score * s,Score * s1)
{
	ALLEGRO_PATH   *path;
	am = b;
	sc = s;
	size = 20; //tamño en pixels del movimiento
	sc_vidas = s1;
	sc_vidas->suma(18);
	activo = TRUE;

	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_remove_path_component(path,-1);
	al_append_path_component(path, "resources");

	al_set_path_filename(path, "chewbacca3.wav");

	sonido_comer_hormiga = al_load_sample(al_path_cstr(path, '/'));
	if(sonido_comer_hormiga == NULL)
	{
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource sonido comer hormiga",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}

	al_set_path_filename(path, "richerlandtv__arcade-game.wav");

	sonido_comer_bolsa = al_load_sample(al_path_cstr(path, '/'));
	if(sonido_comer_bolsa == NULL)
	{
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource sonido comer bolsa",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}

	al_set_path_filename(path, "game-fail.wav");

	sonido_perdida_vida = al_load_sample(al_path_cstr(path, '/'));
	if(sonido_perdida_vida == NULL)
	{
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource sonido game over",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}

	al_destroy_path(path);


}
OsoHormiguero::~OsoHormiguero()
{

}
void OsoHormiguero::do_action(ControllableObject::action_t act, int magnitude)
{


	switch (act)
	{
		case DOWN:

	//		y+=size;
			set_y(get_y()+size);
			break;
		case UP:
//			y-=size;
			set_y(get_y()-size);
			break;
		case LEFT:
//			x-=size;
			set_x(get_x()-size);
			break;
		case RIGHT:
//			x+=size;
			set_x(get_x()+size);
			break;
		case ANIQUILACION:
			aniquilacion();
			break;
		default:
			break;
	}

	if (get_x()<0) set_x(0);
	if (get_x()>Game::gfx_w-get_w()) set_x(Game::gfx_w-get_w());

	if (get_y()<0) set_y(0);
	if (get_y()>Game::gfx_h-get_h()) set_y(Game::gfx_h-get_h());
}
void OsoHormiguero::hit(Actor *who, int damage)
{

	switch (who->get_team())
	{
		case TEAM_HORMIGAS_ROJAS:
			al_play_sample(sonido_perdida_vida, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			sc_vidas->resta(1);
			set_x(0);
			set_y(Game::gfx_w-get_w());
			break;
		case TEAM_HORMIGAS_VERDES:
			al_play_sample(sonido_comer_hormiga, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			sc->suma(10);
			break;
		case TEAM_OSO:
			break;
		case TEAM_COMIDA:
			break;
		case TEAM_AGUA:
			break;
		case TEAM_VENENO:
			al_play_sample(sonido_perdida_vida, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			sc_vidas->resta(1);
			set_x(0);
			set_y(Game::gfx_w-get_w());
			break;
		case TEAM_HORMIGUERO:
			set_x(Game::rnd(0,Game::gfx_w-get_w()));
			set_y(Game::rnd(0,Game::gfx_h-get_h()));
			al_play_sample(sonido_perdida_vida, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			sc_vidas->resta(1);
			set_x(0);
			set_y(Game::gfx_w-get_w());
			break;
		case TEAM_RANA:
			al_play_sample(sonido_comer_hormiga, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			sc->suma(20);
			break;
		case TEAM_BOLSA_DINERO:
			al_play_sample(sonido_comer_bolsa, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			if (sc_vidas->get_score() < 18) sc_vidas->suma(1);
			sc->suma(30);
			break;
		default:
			break;
	}

	check_salud();

}

void OsoHormiguero::hit(std::string objeto, int damage)
{
	/*
	 * Los strings con los que comparamos son los valores de la property "objeto"
	 * del mapa
	 */
	if (objeto == "agua") {
//		deshacer_posicion();
	} else
	if (objeto == "piedra") {
		deshacer_posicion();
	}

}

OsoHormiguero* OsoHormiguero::crear_osohormiguero(ActorManager *actmgr,Score * score_tmp,
										Score * score_tmp1)
{
	  ALLEGRO_BITMAP *bmp1,*bmp2,*bmp3;
	  Sprite     	*sp;
	  OsoHormiguero	*osohormiguero_tmp;
	  ALLEGRO_PATH   *path;

	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	 path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	 al_remove_path_component(path,-1);
	 al_append_path_component(path, "resources");

	 osohormiguero_tmp = new OsoHormiguero(actmgr,score_tmp,score_tmp1);

	 al_set_path_filename(path, "oso1.png");

	 bmp1 = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp1 == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 // Hacemos que no se vea el color negro en el bitmap de oso
	 al_convert_mask_to_alpha(bmp1, al_map_rgb(0,0,0));

	 al_set_path_filename(path, "oso2.png");

	 bmp2 = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp2 == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 // Hacemos que no se vea el color negro en el bitmap de oso
	 al_convert_mask_to_alpha(bmp2, al_map_rgb(0,0,0));

	 al_set_path_filename(path, "oso3.png");

	 bmp3 = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp3 == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 // Hacemos que no se vea el color negro en el bitmap de oso
	 al_convert_mask_to_alpha(bmp3, al_map_rgb(0,0,0));

	 sp=new Sprite(osohormiguero_tmp);
	 sp->add_frame(bmp1, al_get_bitmap_width(bmp1)/2, al_get_bitmap_height(bmp1)/2, 0);
	 sp->add_frame(bmp2, al_get_bitmap_width(bmp2)/2, al_get_bitmap_height(bmp2)/2, 0);
	 sp->add_frame(bmp3, al_get_bitmap_width(bmp3)/2, al_get_bitmap_height(bmp3)/2, 0);

	 osohormiguero_tmp->set_actor_graphic(sp);

	 osohormiguero_tmp->set_x(Game::rnd(50,Game::gfx_w-osohormiguero_tmp->get_w()));
	 osohormiguero_tmp->set_y(Game::rnd(0,Game::gfx_h-osohormiguero_tmp->get_h()));

	 osohormiguero_tmp->set_is_detected(true);
	 osohormiguero_tmp->set_team(TEAM_OSO);
	 osohormiguero_tmp->set_collision_method(CollisionManager::PP_COLLISION);

	 actmgr->add(osohormiguero_tmp);

	 al_destroy_path(path);

	 return osohormiguero_tmp;
}
int OsoHormiguero::get_puntuacion()
{
	int p;

	p = sc->get_score();
	return p;
}
void OsoHormiguero::aniquilacion()
{

	list<Actor*>::iterator 	actors_iter_tmp;



	/*
	 * Recorro la lista de actores y elimino todas las hormigas rojas
	 */

	for (actors_iter_tmp=am->get_begin_iterator();
		 actors_iter_tmp!=am->get_end_iterator();
		 actors_iter_tmp++)
	{
		if((*actors_iter_tmp)->get_team() == TEAM_HORMIGAS_ROJAS) {

			am->del(*actors_iter_tmp);

		}
	}

	sc_vidas->resta(2);
	check_salud();
}
void OsoHormiguero::check_salud()
{
	list<Actor*>::iterator 	actors_iter_tmp;
	Marca 					* marca_tmp;
	int 					nivel_alcanzado;


	/*
	 * Si el oso hormiguero se queda sin vidas, se elimina el oso hormiguero y
	 * pasamos a registrar records y finalizar el juego
	 */
	if (sc_vidas->get_score() <= 0)	{

		/*
		 * Centramos el oso
		 */

		 set_x(350);
		 set_y(300);

		/*
		 * no lo puedo eliminar porque esta asociado a un control y al periferico
		 * teclado, si pulso el cursos despues de muerto el puntero a la instacia
		 * del oso hormiguero ya no es valido.
		 * Por tanto lo hago invisible
		 *
		 * am->del(this);
		 */

		/*
		 * Uso la invisivilidad para no volver a entrar aqui, aunque su primer uso
		 * es para no dibujar al oso hormiguero
		 * Esto es necesario porque cuando el score llega a cero, el oso puede seguir
		 * colisionando y vuelve a esta rutina y por cada colision se vuelve a
		 * crear otra marca, cuando solo se debe crear una
		 */

		if(activo) {


			/*
			 * Obtenemos el nivel en el que estamos para añadirlo a la nueva marca
			 * y lo destruimos
			 */

			for (actors_iter_tmp=am->get_begin_iterator();
							 actors_iter_tmp!=am->get_end_iterator();
				 actors_iter_tmp++)
			{
				if(((*actors_iter_tmp)->get_team() == TEAM_LEVEL) &&
				   ((*actors_iter_tmp)->get_estado() == CREADO))	{

					nivel_alcanzado = (*actors_iter_tmp)->get_level();
					((Level*)(*actors_iter_tmp))->set_last_level(TRUE);
					((Level*)(*actors_iter_tmp))->destroy();

				}
			}

			activo = FALSE;
		}
	}
}
