/*
 * tileoffset.cpp
 *
 *  Created on: 25/3/2016
 *      Author: Usuario
 */

#include "tileoffset.h"

//#define VERBOSE 1

TileOffset::TileOffset(tinyxml2::XMLElement * t)
{
	/*
	 * Valores por defecto
	 */
	x = 0; // Horizontal offset in pixels
	y = 0; // Vertical offset in pixels (positive is down)

	root_tileoffset = t;
	printf("creado tileoffset\n");
	parse();
}
TileOffset::~TileOffset()
{

}

void TileOffset::parse()
{
	tinyxml2::XMLError 			eResult;

	/*
	 * Se extraen todos los atributos del elemento tileoffset
	 */
	eResult = root_tileoffset->QueryIntAttribute("x", &x);
	if (eResult != tinyxml2::XML_SUCCESS)  {
#ifdef VERBOSE
		printf("Error cargando x: %i\n", eResult);
#endif
	}else{
#ifdef VERBOSE
		printf("x %d\n",x);
#endif
	}
	eResult = root_tileoffset->QueryIntAttribute("y", &y);
	if (eResult != tinyxml2::XML_SUCCESS)  {
#ifdef VERBOSE
		printf("Error cargando y: %i\n", eResult);
#endif
	}else{
#ifdef VERBOSE
		printf("y %d\n",y);
#endif
	}
}

