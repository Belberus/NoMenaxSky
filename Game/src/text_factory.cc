#include "text_factory.h"

#include <EasyBMP.h>
#include <EasyBMP_Font.h>
#include <string>
#include <glm/glm.hpp>

using namespace std;


string textToImage(string text, glm::vec2 size, string name){
	BMP image;
	image.SetBitDepth(8);
	image.SetSize(2*size.x,2*size.y);
	RGBApixel FontColor;
	FontColor.Red = 0;
	FontColor.Green = 0;
	FontColor.Blue = 0;
	char* TextString;
	TextString = new char [2050];
	strcpy(TextString, text.c_str());
	int linea = 75;
	int posX = 6;
	int posY = 6;
	int i = 0;
	char* def;
	def = new char [linea];
	//Slicing the text in 50char parts for clarity
	while(TextString[i] != '\0'){
		int j = 0;
		memset(def, 0, linea);
		while(j < linea && TextString[i] != '\n' && TextString[i] != '\0'){
			def[j] = TextString[i];
			j++;
			i++;
		}
		if(TextString[i] == '\n'){
			i++;
		}
		PrintString(image, def, posX,posY,16, FontColor);
		posY += 24;
	}
	//char* ruta = "assets/generatedText/";
	string ruta = "assets/generatedText/"+name+".bmp";
	char* c;
	c = new char [2050];
	strcpy(c, ruta.c_str());
	image.WriteToFile(c);
	return ruta;
}
