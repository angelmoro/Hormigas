/*
 * testframework.h
 *
 *  Created on: 30/1/2016
 *      Author: Usuario
 */

#ifndef INC_TESTFRAMEWORK_H_
#define INC_TESTFRAMEWORK_H_

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"

#define kMaxWater  			50
#define kMaxPoison 			3
#define kMaxFood   			50
#define kRedAntIniciales 	3
#define kGreenAntIniciales 	30

/*
 * Definimos el rol del ejecutable en una arquitectura stand alone o cliente-servidor
 */

#define STANDALONE	0
#define CLIENT		1
#define SERVER		2


class Game;

class TestFrameWork : public Game
{
	public:
		void main();
	private:
		ALLEGRO_SAMPLE *sonido_de_fondo;
};

#endif /* INC_TESTFRAMEWORK_H_ */
