/*
 * ColisionSet.h
 *
 *  Created on: 31/3/2016
 *      Author: Usuario
 */

#ifndef INC_COLISIONSET_H_
#define INC_COLISIONSET_H_

#include <set>
#include <string>

class ColisionSet
{
	public:
		ColisionSet(std::string n);
		~ColisionSet();
		std::string get_nombre();
		std::set<int>* get_set();


	private:
		std::string 			nombre;
		std::set<int>			*set;
};



#endif /* INC_COLISIONSET_H_ */
