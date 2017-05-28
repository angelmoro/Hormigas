/*
 * explosion.cpp
 *
 *  Created on: 3/4/2016
 *      Author: Usuario
 */

#include <stdio.h>
#include <allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include "osohormiguero.h"
#include "game.h"
#include "sprite.h"
#include "records.h"
#include "level.h"

#include "explosion.h"

Explosion::Explosion(ActorManager * b)
{
	ALLEGRO_PATH   *path;
	am = b;

	movimientos = 6;

	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_remove_path_component(path,-1);
	al_append_path_component(path, "resources");

	//TBD cambiar por otro sonido
	al_set_path_filename(path, "61015__kayyy__splash.wav");

	sonido_explosion = al_load_sample(al_path_cstr(path, '/'));
	if(sonido_explosion == NULL)
	{
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource sonido explosion",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}
	al_play_sample(sonido_explosion, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

	al_destroy_path(path);


}
Explosion::~Explosion()
{

}

void Explosion::hit(Actor *who, int damage)
{

	switch (who->get_team())
	{
		case TEAM_HORMIGAS_ROJAS:

			break;
		case TEAM_HORMIGAS_VERDES:

			break;
		case TEAM_OSO:
			break;
		case TEAM_COMIDA:
			break;
		case TEAM_AGUA:
			break;
		case TEAM_VENENO:

			break;
		case TEAM_HORMIGUERO:

			break;
		case TEAM_RANA:

			break;
		case TEAM_BOLSA_DINERO:

			break;
		default:
			break;
	}


}

void Explosion::hit(std::string objeto, int damage)
{


}

Explosion* Explosion::crear_explosion(ActorManager *actmgr,int x,int y)
{
	  ALLEGRO_BITMAP *bmp1,*bmp2,*bmp3;
	  Sprite     	*sp;
	  Explosion		*explosion_tmp;
	  ALLEGRO_PATH   *path;

	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	 path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	 al_remove_path_component(path,-1);
	 al_append_path_component(path, "resources");

	 explosion_tmp = new Explosion(actmgr);

	 al_set_path_filename(path, "Bomb1.png");

	 bmp1 = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp1 == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 // Hacemos que no se vea el color amarillo en el bitmap de oso
	 al_convert_mask_to_alpha(bmp1, al_map_rgb(255,255,0));

	 al_set_path_filename(path, "Bomb2.png");

	 bmp2 = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp2 == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 // Hacemos que no se vea el color amarillo en el bitmap de oso
	 al_convert_mask_to_alpha(bmp2, al_map_rgb(255,255,0));

	 al_set_path_filename(path, "Bomb3.png");

	 bmp3 = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp3 == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 // Hacemos que no se vea el color amarillo en el bitmap de oso
	 al_convert_mask_to_alpha(bmp3, al_map_rgb(255,255,0));

	 sp=new Sprite(explosion_tmp);
	 sp->add_frame(bmp1, al_get_bitmap_width(bmp1)/2, al_get_bitmap_height(bmp1)/2, 0);
	 sp->add_frame(bmp2, al_get_bitmap_width(bmp2)/2, al_get_bitmap_height(bmp2)/2, 0);
	 sp->add_frame(bmp3, al_get_bitmap_width(bmp3)/2, al_get_bitmap_height(bmp3)/2, 0);

	 explosion_tmp->set_actor_graphic(sp);

	 explosion_tmp->set_x(x);
	 explosion_tmp->set_y(y);

	 explosion_tmp->set_is_detected(true);
	 explosion_tmp->set_team(TEAM_EXPLOSION);
	 explosion_tmp->set_collision_method(CollisionManager::PP_COLLISION);

	 actmgr->add(explosion_tmp);

	 al_destroy_path(path);

	 return explosion_tmp;
}
void Explosion::move()
{
	movimientos = movimientos -1;
	if (movimientos < 0) am->del(this);
}

