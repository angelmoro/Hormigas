/*
 * tileframe.cpp
 *
 *  Created on: 23/3/2016
 *      Author: Usuario
 */


#include "tileframe.h"

//#define VERBOSE 1

TileFrame::TileFrame(tinyxml2::XMLElement * t)
{
	/*
	 * Valores por defecto
	 */
	tileid = -1; // The local ID of a tile within the parent tileset.
	duration = 0; // How long (in milliseconds) this frame should be displayed before advancing to the next frame.

	root_tileframe = t;
#ifdef VERBOSE
	printf("creado tileframe\n");
#endif
	parse();
}
TileFrame::~TileFrame()
{

}

void TileFrame::parse()
{
	tinyxml2::XMLError 		eResult;

	/*
	 * Se extraen todos los atributos del elemento tileframe
	 */
	eResult = root_tileframe->QueryIntAttribute("tileid", &tileid);
	if (eResult != tinyxml2::XML_SUCCESS)  {
#ifdef VERBOSE
		printf("Error cargando tileid: %i\n", eResult);
#endif
	}else{
#ifdef VERBOSE
		printf("tileid %d\n",tileid);
#endif
	}
	eResult = root_tileframe->QueryIntAttribute("duration", &duration);
	if (eResult != tinyxml2::XML_SUCCESS)  {
#ifdef VERBOSE
		printf("Error cargando duration: %i\n", eResult);
#endif
	}else{
#ifdef VERBOSE
		printf("duration %d\n",duration);
#endif
	}
}
