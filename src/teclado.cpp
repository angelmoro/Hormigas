/*
 * teclado.cpp
 *
 *  Created on: 31/1/2016
 *      Author: Usuario
 */

#include <allegro.h>
#include <string>

#include "bool.h"
#include "teclado.h"
#include "peripheral.h"
#include <stdio.h>

Teclado::Teclado()
{

    for (int i=0; i<ALLEGRO_KEY_MAX;i++) ue[i].evento_sin_consumir=FALSE;

}
Teclado::~Teclado()
{
}
Peripheral::state_t Teclado::get_state(component_t comp)
{

	if (ue[comp].evento_sin_consumir){
		ue[comp].evento_sin_consumir = FALSE;
		if (ue[comp].evento.type == ALLEGRO_EVENT_KEY_DOWN) return KEY_PRESS;
		if (ue[comp].evento.type == ALLEGRO_EVENT_KEY_UP) return KEY_RELEASE;
		return INVALID_STATE;
	}
	else
	return INVALID_STATE;
}
Peripheral::component_t Teclado::get_change()
{
	/*
	for (int i=0;i<ALLEGRO_KEY_MAX;i++)
	{
		if (key[i]!=old_state[i])
		{
			old_state[i]=key[i];
			return i;
		}
	}
	return INVALID_COMPONENT;
	*/
	/*
	 * lo hago inicialmente para mantener solo el ultimo evento, puede que haya que creear
	 * una lista para no perder eventos
	 */
//	printf(" antes if consumo evento %d\n",ultimo_evento.keyboard.keycode);
	/*
	if (evento_sin_consumir)
	{
		printf("consumo evento %d\n",ultimo_evento.keyboard.keycode);
		switch (ultimo_evento.keyboard.keycode)
		{
			case ALLEGRO_KEY_LEFT:
				return KEY_LEFT;
				break;
			case ALLEGRO_KEY_RIGHT:
				return KEY_RIGHT;
				break;
			case ALLEGRO_KEY_UP:
				return KEY_UP;
				break;
			case ALLEGRO_KEY_DOWN:
				return KEY_DOWN;
				break;
			default:
				return INVALID_COMPONENT;
		}
		evento_sin_consumir = FALSE;
	}
	*/
	return INVALID_COMPONENT;

}
void Teclado::reset()
{

	for (int i=0; i<ALLEGRO_KEY_MAX;i++) ue[i].evento_sin_consumir=FALSE;

}
string Teclado::get_component_name(component_t comp)
{
	string ret;

	switch (comp) {

		case ALLEGRO_KEY_LEFT:
			ret="LEFT";
			break;
		case ALLEGRO_KEY_RIGHT:
			ret="RIGHT";
			break;
		case ALLEGRO_KEY_UP:
			ret="UP";
			break;
		case ALLEGRO_KEY_DOWN:
			ret="DOWN";
			break;

		default:
			ret="NO COMPONENT";
			break;
		/*
		 * añadir todas las teclas del teclado
		 */
	}
	if ((comp >= ALLEGRO_KEY_A) || (comp <= ALLEGRO_KEY_Z )) {
		ret = al_keycode_to_name(comp);
	}

//	printf("retorno componente %d\n",comp);
	return ret;
}

void Teclado::evento(ALLEGRO_EVENT e)
{

	ue[e.keyboard.keycode].evento = e;
	ue[e.keyboard.keycode].evento_sin_consumir = TRUE;

}
