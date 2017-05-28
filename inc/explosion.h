/*
 * explosion.h
 *
 *  Created on: 3/4/2016
 *      Author: Usuario
 */

#ifndef INC_EXPLOSION_H_
#define INC_EXPLOSION_H_

#include <allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "actor.h"

class Explosion : public Actor
{
public:
	Explosion(ActorManager * b);
	~Explosion(void);

	void hit(Actor *a, int damage);
	void hit(std::string objeto, int damage);
	static Explosion * crear_explosion(ActorManager *actmgr,int x,int y);
	void move();

protected:
	ActorManager	* am;
	ALLEGRO_SAMPLE 	* sonido_explosion;
	int				movimientos;

private:

};


#endif /* INC_EXPLOSION_H_ */
