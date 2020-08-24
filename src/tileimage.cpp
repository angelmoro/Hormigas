/*
 * tileimage.cpp
 *
 *  Created on: 23/3/2016
 *      Author: Usuario
 */

#include "math.h"
#include "tileimage.h"
#include "tiledata.h"

//#define VERBOSE 1

TileImage::TileImage(tinyxml2::XMLElement * t)
{
	/*
	 * Valores por defecto
	 */
	format = ""; // Used for embedded images, in combination with a data child element. Valid values are file extensions like png, gif, jpg, bmp, etc. (since 0.9)
	source = ""; // The reference to the tileset image file (Tiled supports most common image formats).
	trans = ""; // Defines a specific color that is treated as transparent (example value: "#FF00FF" for magenta). Up until Tiled 0.12, this value is written out without a # but this is planned to change.
	width = 0; // The image width in pixels (optional, used for tile index correction when the image changes)
	height = 0; // The image height in pixels (optional)

	bmp = NULL;

	root_tileimage = t;
#ifdef VERBOSE
	printf("creado tileimage\n");
#endif
	parse();
	cargar_imagen();
}
TileImage::~TileImage()
{

}
void TileImage::draw(int x,int y,float opacity)
{
	/*
	 * Dibuja la imagen completa en la posición x,y del display con un determinado grado de opacidad
	 */

	if (bmp != NULL) al_draw_tinted_bitmap(bmp, al_map_rgba_f(1, 1, 1, opacity), x, y, 0);

}
void TileImage::draw(int x,int y,int tx,int ty,int tw,int th)
{
	/*
	 *  Dibuja el tile en la posicion tx,ty de la imagen, con ancho tw y alto th
	 *  en la posición x,y del display
	 */
	if (bmp != NULL) {
		al_draw_bitmap_region(bmp,tx,ty,tw,th,x,y,0);
	}

}
void TileImage::parse()
{
	tinyxml2::XMLError 			eResult;
	tinyxml2::XMLElement 		* pElement_tmp;
	const char 					* szAttributeText;

	/*
	 * Se extraen todos los atributos del elemento map
	 */
	szAttributeText = NULL;
	szAttributeText = root_tileimage->Attribute("format");
	if (szAttributeText == NULL) {
#ifdef VERBOSE
		printf("format no encontrado\n");
#endif
	}else {
		format = szAttributeText;
#ifdef VERBOSE
		printf("format %s\n",format.c_str());
#endif
	}
	szAttributeText = NULL;
	szAttributeText = root_tileimage->Attribute("source");
	if (szAttributeText == NULL) {
#ifdef VERBOSE
		printf("source no encontrado\n");
#endif
	}else {
		source = szAttributeText;
#ifdef VERBOSE
		printf("source %s\n",source.c_str());
#endif
	}
	szAttributeText = NULL;
	szAttributeText = root_tileimage->Attribute("trans");
	if (szAttributeText == NULL) {
#ifdef VERBOSE
		printf("trans no encontrado\n");
#endif
	}else {
		trans = szAttributeText;
#ifdef VERBOSE
		printf("trans %s\n",trans.c_str());
#endif
	}
	eResult = root_tileimage->QueryIntAttribute("width", &width);
	if (eResult != tinyxml2::XML_SUCCESS)  {
#ifdef VERBOSE
		printf("Error cargando width: %i\n", eResult);
#endif
	}else{
#ifdef VERBOSE
		printf("width %d\n",width);
#endif
	}
	eResult = root_tileimage->QueryIntAttribute("height", &height);
	if (eResult != tinyxml2::XML_SUCCESS)  {
#ifdef VERBOSE
		printf("Error cargando height: %i\n", eResult);
#endif
	}else{
#ifdef VERBOSE
		printf("height %d\n",height);
#endif
	}
	/*
	 * Creamos el elemento data si existe
	 */
	pElement_tmp = root_tileimage->FirstChildElement("data");
	if (pElement_tmp == NULL) {
#ifdef VERBOSE
		printf("no hay ningun elemento tiledata\n");
#endif
	} else {
		data = new TileData(pElement_tmp);
	}

}
void TileImage::cargar_imagen()
{
	ALLEGRO_PATH   				*path;
	int 						R = 0;
	int 						G = 0;
	int							B = 0;
	int							i;
	std::string::iterator		it; // The string iterator.
	std::string					string_tmp;

	/*
	* para crear path relativos y poder distribuir el programa y ejecutarlo
	* fuera del IDE
	*/

	path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_remove_path_component(path,-1);
	al_append_path_component(path, "resources");

	if (source != "") {
		al_set_path_filename(path, source.c_str());

		bmp = al_load_bitmap(al_path_cstr(path, '/'));
		if(bmp == NULL)
		{
			al_show_native_message_box(al_get_current_display(), "Ventana de error",
									   "error fatal", "Error al cargar resource bitmap",
									   NULL, ALLEGRO_MESSAGEBOX_ERROR);
			exit(-1);
		}
		if (trans != "") {
			// Hacemos que no se vea el color definido en trans, formato hex ejm: #FF00FF
			string_tmp = "";
			i=0;
			for (it= trans.begin(); it != trans.end(); it++)
			{
			  if ((*it) == '#') continue;

			  string_tmp = string_tmp + (*it);
			  i++;
			  switch (i) {
				  case 2:
					  R = hex2dec(string_tmp);
					  string_tmp = "";
					  break;
				  case 4:
					  G = hex2dec(string_tmp);
					  string_tmp = "";
					  break;
				  case 6:
					  B = hex2dec(string_tmp);
					  string_tmp = "";
					  break;
				  default:
					  break;
			  }
			}
			al_convert_mask_to_alpha(bmp, al_map_rgb(R,G,B));
		}
	}
}

unsigned long TileImage::hex2dec(std::string hex)
{
    unsigned long result = 0;
    for (unsigned int i=0; i<hex.length(); i++) {
        if (hex[i]>=48 && hex[i]<=57)
        {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

