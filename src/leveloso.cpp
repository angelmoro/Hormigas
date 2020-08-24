/*
 * leveloso.cpp
 *
 *  Created on: 27/2/2016
 *      Author: Usuario
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>

#include "bool.h"
#include "level.h"
#include "leveloso.h"
#include "hormiga.h"
#include "comida.h"
#include "veneno.h"
#include "hormiguero.h"
#include "agua.h"
#include "levelgraphic.h"
#include "rana.h"
#include "bolsadinero.h"
#include "records.h"
#include "levelosoconf.h"
#include "tilemap.h"
#include "marca.h"

LevelOso::LevelOso(ActorManager * b,LevelManager * l,
		           int n, int c, int a, int v, int hv,
				   int hr, int rt, int bt, int rtd, int btd, TileMap *m):Level(b,l,n)
{
	ALLEGRO_PATH   *path;

	comida = c;
	agua = a;
	veneno = v;
	hormigas_verdes = hv;
	hormigas_rojas = hr;
	rana_ticks = rt;
	rana_ticks_fijados = rt;
	bolsa_dinero_ticks = bt;
	bolsa_dinero_ticks_fijados = bt;
	rana_mov_to_dead = rtd;
	bolsa_dinero_mov_to_dead = btd;
	mapa = m;



	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_remove_path_component(path,-1);
	al_append_path_component(path, "resources");

	al_set_path_filename(path, "robinhood.wav");

	sonido_fin_nivel = al_load_sample(al_path_cstr(path, '/'));
	if(sonido_fin_nivel == NULL)
	{
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource sonido fin nivel",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}

	al_set_path_filename(path, "landlucky__game-over.wav");


	sonido_game_over = al_load_sample(al_path_cstr(path, '/'));
	if(sonido_game_over == NULL)
	{
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource sonido game over",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}


}
LevelOso::~LevelOso()
{

}
void LevelOso::init()
{

}
void LevelOso::iniciar()
{

/*
 * establecemos la fase 1
 */
	set_estado(INICIALIZADO);
//	printf("nivel %d inicializado\n",num_nivel);
/*
 * Ticks de esta fase
 */
	ticks_inicializado = 250;  // 250/70 = 3,57 segundos
/*
 * añado al actor manager el nivel que esta activo, este nivel es el que estoy
 * activando
 */
	 am->add(this);
}
void LevelOso::create()
{
	 int 			i;
	 Hormiguero		*hormiguero_tmp;
	 /*
	  * Creamos la comida
	  */

//	 limite = Game::rnd(0,10);
	// limite = 1;
	 for(i=0; i<comida; i++){
		 Comida::crear_comida(am);
	 }

	 /*
	  * Creamos el agua
	  */

//	 limite = Game::rnd(0,5);
//	 limite = 1;
	 for(i=0; i<agua; i++){
		 Agua::crear_agua(am);
	 }

	 /*
	  * Creamos el veneno
	  */

//	 limite = Game::rnd(0,1);
//	 limite = 1;
	 for(i=0; i<veneno; i++){
		 Veneno::crear_veneno(am);
	}

	 /*
	 * Creamos el hormiguero
	 */

	 hormiguero_tmp = Hormiguero::crear_hormiguero(am);

	 /*
	 * Creamos hormigas rojas iniciales
	 */

//	 limite = Game::rnd(0,2);
//	 limite = 1;
	 for(i=0; i<hormigas_rojas; i++){
		Hormiga::crear_hormiga(am,kRedAnt,hormiguero_tmp);
	 }

	 /*
	  * Creamos las hormigas verdes iniciales
	  */

	// limite = Game::rnd(0,10);
//	 limite = 1;
	 for(i=0; i<hormigas_verdes; i++){
		Hormiga::crear_hormiga(am,kGreenAnt,hormiguero_tmp);
	 }



 /*
  * establecemos estado a creado
  */
	 set_estado(CREADO);
//	 printf("nivel %d creado\n",num_nivel);

}

void LevelOso::destroy()
{

	list<Actor*>::iterator tmp_iter;



	/*
	 * Eliminamos del actor manager todos los actores que no son validos cuando se
	 * termina un nivel
	 */
	for (tmp_iter=am->get_begin_iterator();
			 tmp_iter!=am->get_end_iterator();
			 tmp_iter++)
	{
		switch ((*tmp_iter)->get_team()) {
			case TEAM_HORMIGAS_ROJAS:
			case TEAM_HORMIGAS_VERDES:
			case TEAM_COMIDA:
			case TEAM_AGUA:
			case TEAM_VENENO:
			case TEAM_HORMIGUERO:
//			case TEAM_LEVEL:    No se deben destruir los actores nivel.
				am->del(*tmp_iter);
				break;
			default:
				break;
		}
	}

/*
 * establecemos estado a destruido
 */
	set_estado(DESTRUIDO);
//	printf("nivel %d destruido\n",num_nivel);
/*
 * Ticks de esta fase
 */
	ticks_destruido = 250;   // 250/70 = 3,57 segundos

}

LevelOso* LevelOso::crear_level(ActorManager *actmgr,LevelManager *levmgr, string nombre,int nivel,
		                     int c,int a,int v,int hv,int hr, int pos_x,int pos_y,
							 int rt, int bt, int rtd, int btd)
{
//	  ALLEGRO_BITMAP 	*bmp;
	  LevelOso		 	*level_oso_tmp;
	  LevelGraphic   	*lg;
	  ALLEGRO_PATH   	*path;
	  ALLEGRO_FONT 	 	*font;
	  TileMap			*m;
	  char 				buffer [256];

	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	 path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	 al_remove_path_component(path,-1);
	 al_append_path_component(path, "resources");


	 /*
	  * Creamos el mapa del nivel
	  */

	 sprintf (buffer, "mapa%d.tmx",nivel);
	 al_set_path_filename(path, buffer);

	 m = new TileMap(al_path_cstr(path, '/'));

	 /*
	  * creamos el nivel
	  */
	 level_oso_tmp = new LevelOso(actmgr,levmgr,nivel,c,a,v,hv,hr,rt,bt,rtd,btd,m);
/*
	 bmp = al_load_bitmap(al_path_cstr(path, '/'));
	 if(bmp == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource bitmap",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
*/


	 lg=new LevelGraphic(level_oso_tmp, m);

	 al_set_path_filename(path, "comic.ttf");
	 font = NULL;
	 font = al_load_ttf_font(al_path_cstr(path, '/'),20,0);

	 if(font == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar el font",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 lg->set_font(font);

	 al_set_path_filename(path, "comic.ttf");
	 font = NULL;
	 font = al_load_ttf_font(al_path_cstr(path, '/'),40,0);

	 if(font == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar el font",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 lg->set_font_transito(font);


	 lg->set_title(nombre);
	 level_oso_tmp->set_actor_graphic(lg);

	 level_oso_tmp->set_x(pos_x);
	 level_oso_tmp->set_y(pos_y);
	 level_oso_tmp->set_is_detected(false); //TDB revisar si el nivel necesita colisionar el bmp que se utilizaba para calcular la colision se ha substituido por el mapa
	 level_oso_tmp->set_team(TEAM_LEVEL);
	 level_oso_tmp->set_collision_method(CollisionManager::PP_COLLISION);

	 al_destroy_path(path);
	 return level_oso_tmp;

}




void LevelOso::crear_niveles(int niveles,ActorManager *actmgr,LevelManager *levmgr)
{
	int i;
	LevelOso *level_oso_tmp;
	int pos_x,pos_y;
	int r_ticks, b_ticks;
	int	r_mov_to_dead;
	int b_mov_to_dead;
	LevelOsoConf configuracion;

	pos_x = 250;
	pos_y = 0;
//	float peso_comida,peso_agua,peso_veneno,peso_hormigas_verdes,peso_hormigas_rojas;
//	float peso_rana_ticks, peso_bolsa_ticks, peso_rana_to_dead, peso_bolsa_to_dead;
	int c,a,v,hr,hv;



/*
 * Si se elimna la lectura del fichero de configuracion, se pueden
 * establecer los parametros harcoded aqui
 *
	configuracion.set_peso_comida(1.0);
	configuracion.set_peso_agua(1.0);
	configuracion.set_peso_veneno(0.3);
	configuracion.set_peso_hormigas_verdes(1.5);
	configuracion.set_peso_hormigas_rojas(0.5);
	configuracion.set_peso_rana_ticks(100.0);
	configuracion.set_peso_bolsa_ticks(100.0);
	configuracion.set_peso_rana_to_dead(0.5);
	configuracion.set_peso_bolsa_to_dead(0.3);
	configuracion.set_r_ticks(1000);
	configuracion.set_b_ticks(2000);
	configuracion.set_r_mov_to_dead(20);
	configuracion.set_b_mov_to_dead(10);
	configuracion.set_key_pad(false);
*/


	/*
	 * fijamos los ticks base para la rana y la bolsa de dinero, con esto se fija
	 * el tiempo cada cuanto aparecen 70 ticks es un segundo. El ritmo de ticks podría
	 * variar en game.
	 */
	r_ticks = configuracion.get_r_ticks();
	b_ticks = configuracion.get_b_ticks();
	/*
	 * Fijamos los saltos antes de desaparecer, esto depende de el nunero de veces que
	 * se actualiza el estado de los actores. Este ritmo tambien esta en game
	 */
	r_mov_to_dead = configuracion.get_r_mov_to_dead();
	b_mov_to_dead = configuracion.get_b_mov_to_dead();

/*
 * Creamos 20 niveles con el numero de comida,agua,veneno,hormiga verde y hormiga roja
 * ajustado a su nivel y las apariciones y desapariciones de la rana y la bolsa de dinero
 */
	 for(i=1; i<=niveles; i++){
		 c = (int)floor(configuracion.get_peso_comida() * i);
		 a = (int)floor(configuracion.get_peso_agua() * i);
		 v = (int)floor(configuracion.get_peso_veneno() * i);
		 hv = (int)floor(configuracion.get_peso_hormigas_verdes() * i);
		 hr = (int)floor(configuracion.get_peso_hormigas_rojas() * i);

		 r_ticks = r_ticks + (int)floor(configuracion.get_peso_rana_ticks() * i);
		 b_ticks = b_ticks + (int)floor(configuracion.get_peso_bolsa_ticks() * i);

		 r_mov_to_dead = r_mov_to_dead + (int)floor(configuracion.get_peso_rana_to_dead() * i);

		 b_mov_to_dead = b_mov_to_dead + (int)floor(configuracion.get_peso_bolsa_to_dead() * i);

//		 printf("nivel %d, c %d, a %d, v %d, hv %d, hr %d rt %d bt %d rtd %d btd %d\n",
//		         i,c,a,v,hv,hr,r_ticks,b_ticks,r_mov_to_dead,b_mov_to_dead);


		level_oso_tmp = LevelOso::crear_level(actmgr,levmgr,"nivel",i,
				                              c,a,v,hv,hr,pos_x,pos_y,r_ticks,b_ticks,
											  r_mov_to_dead,b_mov_to_dead);

		if (i == niveles) {
			level_oso_tmp->set_last_level(TRUE);
		} else {
			level_oso_tmp->set_last_level(FALSE);
		}
		levmgr->add(level_oso_tmp);
	 }
}

void LevelOso::tick()
{
	Level * level_tmp;
	Level * level_marca_tmp;
	Actor actor_tmp;
	list<Actor*>::iterator actors_iter_tmp;
	Marca *marca_tmp;
	int puntuacion = 0;

	switch (estado) {

		case INICIALIZADO:
			ticks_inicializado--;
			if (ticks_inicializado == 0) create();
			break;
		case CREADO:

	/*
	 * Creamos una rana cada 14,28 segundos (1000 ticks/ (70 ticks/seg))
	 */
			rana_ticks = rana_ticks -1;
			if (rana_ticks < 0) {
				 rana_ticks = rana_ticks_fijados;
				 Rana::crear_rana(am, rana_mov_to_dead);
			}

	/*
	 * Creamos una bolsa de dinero cada 42,85 segundos (1000 ticks/ (70 ticks/seg))
	*/
			bolsa_dinero_ticks = bolsa_dinero_ticks -1;
			if (bolsa_dinero_ticks < 0) {
				 bolsa_dinero_ticks = bolsa_dinero_ticks_fijados;
				 BolsaDinero::crear_bolsa_dinero(am,bolsa_dinero_mov_to_dead);
			}

	/*
	 * Si todas las hormigas verdes muertas, cambio de nivel
	 */

			if (Hormiga::num_hormigas_verdes == 0){
				level_tmp = le->current();
				level_tmp->destroy();
				al_play_sample(sonido_fin_nivel, 3.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
			}
			break;
		case DESTRUIDO:
			ticks_destruido--;
			if (ticks_destruido == 0) {
				/*
				 * Desactivamos el nivel y avanzamos al siguiente
				 * si no es el ultimo
				 */
				level_tmp = le->next();
				level_marca_tmp = level_tmp;
				if (!level_tmp->last_level()) {
					level_tmp->set_activo(FALSE);
					level_tmp = le->current();
					if (level_tmp != NULL ) level_tmp->set_activo(TRUE);
				} else {
					// fin del juego, pedir nombre para almacenar record y mostrar records
					al_play_sample(sonido_game_over, 9.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

	/*
	 * Buscamos el osohormiguero para obetner la puntuacion
	 */
					for (actors_iter_tmp=am->get_begin_iterator();
							 actors_iter_tmp!=am->get_end_iterator();
							 actors_iter_tmp++)
						{
							if((*actors_iter_tmp)->get_team()== TEAM_OSO) {
								puntuacion = (*actors_iter_tmp)->get_puntuacion();
							}
						}

	/*
	 * Buscamos los records para añadir una nueva marca
	 */
					for (actors_iter_tmp=am->get_begin_iterator();
						 actors_iter_tmp!=am->get_end_iterator();
						 actors_iter_tmp++)
					{
						if((*actors_iter_tmp)->get_team()== TEAM_RECORDS) {
							(*actors_iter_tmp)->set_estado(CAPTURANDO);

							/*
							 * Creamos una nueva marca y la añadimos a la lista de records.
							 */
								marca_tmp = new Marca();
								marca_tmp->set_nombre(""); //todavia no se el nombre
								marca_tmp->set_puntuacion(puntuacion);
								marca_tmp->set_level(level_marca_tmp->get_level());
								(*actors_iter_tmp)->add(marca_tmp);

						}
					}
				}
			}
			break;
		default:
			break;
	}
}
void LevelOso::set_activo(bool a)
{
	activo = a;

	if (activo == TRUE)
	{
		/*
		 * Si se activa el nivel notificamos al colision manager
		 * el colision set de mapa del nivel
		 */
		// TBD "meta_tiles","colisionable","meta tiles" y "objeto" deberian de ser cadenas parametrizables.

		/*
		 * Registramos el mapa
		 */

		le->get_game()->collision_manager->registrar_mapa(mapa,
														"meta_tiles",
														"colisionable",
														"objeto",
														"meta tiles");
/*
 * Creamos un colision set para los objetos "piedra"
 */
		le->get_game()->collision_manager->add_colision_set("piedra");
/*
 * cramos un clision set para los objetos "agua"
 */

		le->get_game()->collision_manager->add_colision_set("agua");
	} else
	{
		/*
		 * Desregistramos el mapa
		 */
		le->get_game()->collision_manager->desregistrar_mapa();
	}
}
