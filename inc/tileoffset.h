/*
 * tileoffset.h
 *
 *  Created on: 25/3/2016
 *      Author: Usuario
 */

#ifndef INC_TILEOFFSET_H_
#define INC_TILEOFFSET_H_

#include <string>
#include "tinyxml2.h"


class TileOffset
{
public:
	TileOffset(tinyxml2::XMLElement * t);
	~TileOffset();
	void parse();
private:

	int x; // Horizontal offset in pixels
	int y; // Vertical offset in pixels (positive is down)


	tinyxml2::XMLElement * root_tileoffset;
};

#endif /* INC_TILEOFFSET_H_ */
