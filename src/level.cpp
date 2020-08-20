/*
 * level.cpp
 *
 *  Created on: 26/2/2016
 *      Author: Usuario
 */

#include <stdio.h>
#include <iostream>
#include <string>

#include "bool.h"
#include "level.h"
#include "actormanager.h"
#include "game.h"
#include "actor.h"

Level::Level(ActorManager *a,LevelManager *l,int n)
{
	am = a;
	le = l;
	num_nivel = n;
	activo = FALSE;
	ultimo_nivel = FALSE;
	set_estado(NO_ESTADO);
}
Level::~Level()
{

}
int Level::get_level()
{
	return num_nivel;
}
void Level::move()
{

}
void Level::hit(Actor *who, int damage)
{

	switch (who->get_team())
	{
	    case TEAM_OSO:
	    case TEAM_COMIDA:
	    case TEAM_AGUA:
	    case TEAM_VENENO:
	    case TEAM_HORMIGUERO:
		case TEAM_HORMIGAS_ROJAS:
		case TEAM_HORMIGAS_VERDES:
		case TEAM_RANA:
		case TEAM_SCORE:
		case TEAM_BOLSA_DINERO:
		case TEAM_LEVEL:
			break;
		default:
			break;
	}

}

void Level::init()
{

}
void Level::iniciar()
{

}
void Level::create()
{

}
void Level::destroy()
{

}
bool Level::nivel_activo()
{
	return activo;
}
void Level::set_activo(bool a)
{
	activo = a;
}
void Level::tick()
{

}
void Level::set_estado(int e)
{
	estado = e;
	// TBD ver si es el mejor sitio para registrar y desregistrar el mapa en el
	// collision manager
}
int Level::get_estado()
{
	return estado;
}
bool Level::last_level()
{
	return ultimo_nivel;
}
void Level::set_last_level(bool b)
{
	ultimo_nivel = b;
}
