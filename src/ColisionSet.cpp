/*
 * ColisionSet.cpp
 *
 *  Created on: 31/3/2016
 *      Author: Usuario
 */

#include "colisionset.h"

ColisionSet::ColisionSet(std::string n)
{
	nombre = n;
	set = new std::set<int>;
}
ColisionSet::~ColisionSet()
{
	delete set;
}
std::string ColisionSet::get_nombre()
{
	return nombre;
}
std::set<int>* ColisionSet::get_set()
{
	return set;
}
