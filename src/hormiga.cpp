/*
 * hormiga.cpp
 *
 *  Created on: 29/1/2016
 *      Author: Usuario
 */

#include <stdio.h>
#include <allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include "game.h"
#include "actormanager.h"
#include "hormiga.h"
#include "mapadebits.h"
#include "game.h"
#include "actormanager.h"
#include "explosion.h"

int Hormiga::num_hormigas_verdes=0;
int Hormiga::num_hormigas_rojas=0;

Hormiga::Hormiga(ActorManager * b,Hormiguero * h,int t)
{
	am = b;
	tipo = t;
	state = kForage;
	Hm = h;
	size = 20; //tamño en pixels del movimiento
	if (tipo == kRedAnt){
		num_hormigas_rojas = num_hormigas_rojas + 1;
	} else {
		num_hormigas_verdes = num_hormigas_verdes + 1;
	}
}
Hormiga::~Hormiga()
{
	if (tipo == kRedAnt){
		num_hormigas_rojas = num_hormigas_rojas - 1;
		Explosion::crear_explosion(am,get_x(),get_y());
	} else {
		num_hormigas_verdes = num_hormigas_verdes - 1;
	}

}
void Hormiga::move()
{
	switch (state)
   {
   case kNoBorn:
	   break;
   case kForage:
//	   printf("antes kForage\n");
	   Forage();
//	   printf("despues kForage\n");
	   break;
   case kGoHome:
//	   printf("antes kGoHome\n");
	   GoHome();
//	   printf("despues kGoHome\n");
	   break;
   case kThirsty:
//	   printf("antes kThirsty\n");
	   Thirsty();
//	   printf("despues kThirsty\n");
	   break;
   default:
	   break;

   }
}




void Hormiga::Forage(void)
{
 int rowMove;
 int colMove;
 int newRow;
 int newCol;


 rowMove=Game::rnd(0,2)-1;
 colMove=Game::rnd(0,2)-1;
 newRow=get_y()+rowMove*size;
 newCol=get_x()+colMove*size;

 if (newRow<0) newRow = 0;
 if (newCol<0) newCol = 0;
 if (newRow>Game::gfx_h-get_h()) newRow = Game::gfx_h-get_h();
 if (newCol>Game::gfx_w-get_w()) newCol = Game::gfx_w-get_w();

 set_y(newRow);
 set_x(newCol);

}
void Hormiga::GoHome(void)
{
 int rowMove;
 int colMove;
 int newRow;
 int newCol;

 if (get_x()<Hm->get_x())
	 colMove=1;
 else if (get_x()>Hm->get_x())
	 colMove=-1;
 else
	 colMove=0;

 if (get_y()<Hm->get_y())
	 rowMove=1;
 else if (get_y()>Hm->get_y())
	 rowMove=-1;
 else
	 rowMove=0;

 newRow=get_y()+rowMove*size;
 newCol=get_x()+colMove*size;

 if (newRow<0) newRow = 0;
 if (newCol<0) newCol = 0;
 if (newRow>Game::gfx_h-get_h()) newRow = Game::gfx_h-get_h();
 if (newCol>Game::gfx_w-get_w()) newCol = Game::gfx_w-get_w();

 set_y(newRow);
 set_x(newCol);

}




void Hormiga::Thirsty(void)
{
 int rowMove;
 int colMove;
 int newRow;
 int newCol;

 rowMove=Game::rnd(0,2)-1;
 colMove=Game::rnd(0,2)-1;
 newRow=get_y()+rowMove*size;
 newCol=get_x()+colMove*size;

 if (newRow<0) newRow = 0;
 if (newCol<0) newCol = 0;
 if (newRow>Game::gfx_h-get_h()) newRow = Game::gfx_h-get_h();
 if (newCol>Game::gfx_w-get_w()) newCol = Game::gfx_w-get_w();

 set_y(newRow);
 set_x(newCol);
}


void Hormiga::hit(Actor *who, int damage)
{
	 ALLEGRO_PATH   *path;

	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	 path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	 al_remove_path_component(path,-1);
	 al_append_path_component(path, "resources");

	switch (who->get_team())
	{
		case TEAM_HORMIGAS_ROJAS:
			break;
		case TEAM_HORMIGAS_VERDES:
			break;
		case TEAM_OSO:
			am->del(this);
			break;
		case TEAM_COMIDA:
			if (state == kForage) state=kGoHome;
			break;
		case TEAM_AGUA:
			if (state == kThirsty)	state=kForage;
			break;
		case TEAM_VENENO:
			if (tipo == kGreenAnt) {
				//creo la hormiga en la misma posicion de la que acaba de chocar con el veneno
				Hormiga::crear_hormiga(am,kRedAnt,Hm,x,y);
				am->del(this);
			}
			break;
		case TEAM_HORMIGUERO:

			if ((who->get_y()==Hm->get_y()) && (who->get_x()==Hm->get_x()) && (state == kGoHome))
    		{

			  /*
			   * Si he llegado a mi hormiguero creo una nueva hormiga de mi tipo y me
			   * pongo a buscara agua
			   */

			 	 state=kThirsty;
				 Hormiga::crear_hormiga(am,tipo,Hm,Hm->get_x(),Hm->get_y());
    		}

			break;
		case TEAM_RANA:
			 if (tipo == kRedAnt)
			 {
				 am->del(this);
			 }
			break;
		case TEAM_SCORE:
/*
 * La envio al hormiguero
 */
//			set_x(Hm->get_x());
//			set_y(Hm->get_y());
			break;
		case TEAM_BOLSA_DINERO:
			break;
		default:
			break;
	}

	al_destroy_path(path);

}
void Hormiga::hit(std::string objeto, int damage)
{
/*
 * Los strings con los que comparamos son los valores de la property "objeto"
 * del mapa
 */
	if (objeto == "agua") {
		deshacer_posicion();
		return;
	};
	if (objeto == "piedra") {
//		deshacer_posicion();
		return;
	}


}
void Hormiga::crear_hormiga(ActorManager *actmgr,int tipo,Hormiguero *hormiguero_tmp,
							int pos_x,int pos_y)
{
	  ALLEGRO_BITMAP *bmp;
	  MapaDeBits 	 *bitm;
	  Hormiga		 *hormiga_tmp;
	  ALLEGRO_PATH   *path;

	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	 path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	 al_remove_path_component(path,-1);
	 al_append_path_component(path, "resources");

	 if (tipo == kRedAnt)
	 {
		 if (Hormiga::num_hormigas_rojas > 200) return;
		 al_set_path_filename(path, "HormigaRoja.png");
		 hormiga_tmp = new Hormiga(actmgr,hormiguero_tmp,kRedAnt);
	 }
	 else
	 {
		 if (Hormiga::num_hormigas_verdes > 200) return;
		 al_set_path_filename(path, "HormigaVerde.png");
		 hormiga_tmp = new Hormiga(actmgr,hormiguero_tmp,kGreenAnt);
	 }

	 bmp = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 // Hacemos que no se vea el color negro en el bitmap de hormiga
	 al_convert_mask_to_alpha(bmp, al_map_rgb(0,0,0));

	 bitm=new MapaDeBits(hormiga_tmp, bmp);
	 hormiga_tmp->set_actor_graphic(bitm);
	 if (pos_x == 0) {
		 hormiga_tmp->set_x(Game::rnd(50,Game::gfx_w-hormiga_tmp->get_w()));
	 } else {
		 hormiga_tmp->set_x(pos_x);
	 }
	 if (pos_y == 0) {
		 hormiga_tmp->set_y(Game::rnd(0,Game::gfx_h-hormiga_tmp->get_h()));
	 } else {
		 hormiga_tmp->set_y(pos_y);
	 }
	 hormiga_tmp->set_is_detected(true);

	 if (tipo == kRedAnt)
	 {
		 hormiga_tmp->set_team(TEAM_HORMIGAS_ROJAS);
	 }
	 else
	 {
		 hormiga_tmp->set_team(TEAM_HORMIGAS_VERDES);
	 }

	 hormiga_tmp->set_collision_method(CollisionManager::PP_COLLISION);

	 actmgr->add(hormiga_tmp);

	 al_destroy_path(path);

}

