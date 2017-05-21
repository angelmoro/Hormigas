/*
 * testframework.cpp
 *
 *  Created on: 30/1/2016
 *      Author: Usuario
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#include "testframework.h"
#include "osohormiguero.h"
#include "rana.h"
#include "scoregraphic.h"
#include "score.h"


#include "game.h"
#include "actor.h"
#include "actormanager.h"
#include "controllableactor.h"
#include "control.h"
#include "teclado.h"
#include <ctime>
#include <cstdlib>
#include "mapadebits.h"
#include "sprite.h"
#include "actorgraphic.h"
#include "pixel.h"
#include "semilla.h"
#include "bolsadinero.h"
#include "level.h"
#include "leveloso.h"
#include "hormiga.h"
#include "records.h"
#include "levelosoconf.h"
#include "tilemap.h"

#include "tinyxml2.h"

using namespace tinyxml2;

void *__gxx_personality_v0; //no se para que sirve algo del compilador de c++

void TestFrameWork::main()
{

	 OsoHormiguero 	*osohormiguero_tmp;

	 Score			*score_tmp,*score_tmp1;

	 ALLEGRO_PATH   *path;
//	 Semilla		*semilla_tmp;
//	 Pixel 			*pixel_tmp;
	 Level 			*level_tmp;
	 Records		*records_tmp;
	 int			i;
	 LevelOsoConf   configuracion;




	 /*
	 * para crear path relativos y poder distribuir el programa y ejecutarlo
	 * fuera del IDE
	 */

	 path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	 al_remove_path_component(path,-1);
	 al_append_path_component(path, "resources");


	/*
	 * Creamos 20 niveles. Los hay que crear los primeros si se va a utilizar fondo
	 * en el nivel, para que a la hora de dibujar, primero pinte el fondo del nivel o mapa y
	 * despues el resto de actores. Son los primeros en añadirse a la lista de
	 * actores del actor manager.
	 */

	LevelOso::crear_niveles(20,actor_manager,level_manager);

	 /*
	  * Creamos semillas
	  */
/*
 * Lo meti solo para probar la clase pixel
 *
	 for (int i=0; i<100;i++)
	 {
		 semilla_tmp=new Semilla();
		 pixel_tmp=new Pixel(semilla_tmp, al_map_rgb( 255, 255, 255));
		 semilla_tmp->set_actor_graphic(pixel_tmp);

		 semilla_tmp->set_x(Game::rnd(0,Game::gfx_w-1));
		 semilla_tmp->set_y(Game::rnd(0,Game::gfx_h-1));
		 semilla_tmp->set_is_detected(false);
		 semilla_tmp->set_team(TEAM_SEMILLA);
		 semilla_tmp->set_collision_method(CollisionManager::PP_COLLISION);

		 actor_manager->add(semilla_tmp);
	 }

*/

	 /*
	  * Creamos un score para el oso hormiguero, para los puntos
	  */

	 score_tmp = Score::crear_score(actor_manager,"Puntuacion",0,0,false);

	 /*
	  * Creamos un score para el oso hormiguero para las vidas
	  */

	 score_tmp1 = Score::crear_score(actor_manager,"Vidas",520,0,true);

	 /*
	  * creamos el oso hormiguero como una secuencia de imagenes animacion (sprite)
	  */

	 osohormiguero_tmp = OsoHormiguero::crear_osohormiguero(actor_manager,
			 	 	 	 	 	 	 	 	 	 	 	 	 score_tmp,score_tmp1);

	 /*
	  * Creamos el actor para almacenar los records
	  */

	 records_tmp = Records::crear_records(actor_manager,"Records",200,400,5);


	/*
	 * Solo esta vez para inicializar valores estaticos comunes a todas las ranas
	 */

	 Rana::inicializar_sonido();


	 /*
	  * creamos un control
	  */

	 Control *control_p1=new Control;

	 /*
	  * definimos las acciones del control
	  */

	 control_p1->add_action_name(OsoHormiguero::DOWN, "Bajar");
	 control_p1->add_action_name(OsoHormiguero::UP, "Subir");
	 control_p1->add_action_name(OsoHormiguero::LEFT, "Izquierda");
	 control_p1->add_action_name(OsoHormiguero::RIGHT,"Derecha");
	 control_p1->add_action_name(OsoHormiguero::ANIQUILACION,"Aniquilacion");

	 /*
	  * creamos un periferico
	  */

	 Teclado *kboard=new Teclado;

	 /*
	  * creamos las asociaciones entre las acciones y los componentes/eventos del
	  * periferico
	  */
	 if (!configuracion.get_key_pad()) {

		 control_p1->set_actionperipheral(OsoHormiguero::DOWN, kboard, ALLEGRO_KEY_DOWN,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::UP, kboard, ALLEGRO_KEY_UP,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::LEFT, kboard, ALLEGRO_KEY_LEFT,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::RIGHT, kboard ,ALLEGRO_KEY_RIGHT,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::ANIQUILACION, kboard ,ALLEGRO_KEY_Z,
				 Peripheral::ON_PRESS);

	 } else {

		 control_p1->set_actionperipheral(OsoHormiguero::DOWN, kboard, ALLEGRO_KEY_PAD_2,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::UP, kboard, ALLEGRO_KEY_PAD_8,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::LEFT, kboard, ALLEGRO_KEY_PAD_4,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::RIGHT, kboard ,ALLEGRO_KEY_PAD_6,
		 Peripheral::ON_PRESS);
		 control_p1->set_actionperipheral(OsoHormiguero::ANIQUILACION, kboard ,ALLEGRO_KEY_PAD_5,
		 		 Peripheral::ON_PRESS);

	 }

	 /*
	  * Asociamos el control al oso hormiguero
	  */

	 control_p1->set_owner(osohormiguero_tmp);

	 /*
	  * creamos un control para el nivel y poder escribir nombres asociados a las
	  * puntuaciones
	  */

	 Control *control_p2=new Control;

	 /*
	  * definimos las acciones del control
	  *
	  */

	 for (i= ALLEGRO_KEY_A;i<=ALLEGRO_KEY_Z;i++)
		 control_p2->add_action_name(i, "Letra");

	 control_p2->add_action_name(ALLEGRO_KEY_ENTER, "Enter");
	 control_p2->add_action_name(ALLEGRO_KEY_PAD_ENTER, "Enter");
	 control_p2->add_action_name(ALLEGRO_KEY_BACKSPACE, "Backspace");

	 /*
	  * creamos las asociaciones entre las acciones y los componentes/eventos del
	  * periferico
      */

	 for (i= ALLEGRO_KEY_A;i<=ALLEGRO_KEY_Z;i++)
		 control_p2->set_actionperipheral(i, kboard ,i,
		 	 Peripheral::ON_PRESS);

	 control_p2->set_actionperipheral(ALLEGRO_KEY_ENTER, kboard ,ALLEGRO_KEY_ENTER,
	 		 	 Peripheral::ON_PRESS);
	 control_p2->set_actionperipheral(ALLEGRO_KEY_PAD_ENTER, kboard ,ALLEGRO_KEY_PAD_ENTER,
	 	 		 	 Peripheral::ON_PRESS);
	 control_p2->set_actionperipheral(ALLEGRO_KEY_BACKSPACE, kboard ,ALLEGRO_KEY_BACKSPACE,
	 	 	 		 	 Peripheral::ON_PRESS);

	 /*
	  * Asociamos el control al tablero de records
	  */

		 control_p2->set_owner(records_tmp);


	 /*
	  * Añadimos al control manager el control y el periferico creados
	  */

	 control_manager->add_control(control_p1);
	 control_manager->add_control(control_p2);
	 control_manager->add_peripheral(kboard);


	 /*
	  * sonido de fondo
	  */
	 al_set_path_filename(path, "sonido-ants.wav");

	 sonido_de_fondo = al_load_sample(al_path_cstr(path, '/'));
	 if(sonido_de_fondo == NULL)
	 {
		al_show_native_message_box(al_get_current_display(), "Ventana de error",
								   "error fatal", "Error al cargar resource sonido",
								   NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	 }
	 al_play_sample(sonido_de_fondo, 2.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

	 /*
	  * Arrancamos un timer para tener eventos periodicos en el bucle principal
	  */

	al_start_timer(timer);

	/*
	 * Ojo liberamos la estrucutura path, eliminar esto si la usamos en el while (TRUE)
	 */

	al_destroy_path(path);


	/*
	 * Activamos el primer nivel
	 */
	level_manager->rewind();
	level_tmp = level_manager->current();
	level_tmp->set_activo(TRUE);

	 /*
	  * Entramos al bucle principal de eventos
	  */

	while (TRUE)
	{
		al_wait_for_event(event_queue, &event);

		switch (event.type) {

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				return;
				break;

			case ALLEGRO_EVENT_TIMER:

				update();
				level_manager->tick();
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
			case ALLEGRO_EVENT_KEY_UP:
// enviamos el evento al teclado, sera consumido en el siguiente tick del timer
				kboard->evento(event);
				break;
			default:
				break;
		}

	}

}
int main()
{
	TestFrameWork game;
	game.set_name("El oso hormiguero se va de fiesta");
	game.set_ancho(640);
	game.set_alto(640);
	game.init();
}


