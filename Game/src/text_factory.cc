#include "text_factory.h"

#include <EasyBMP.h>
#include <EasyBMP_Font.h>
#include <string>
#include <glm/glm.hpp>

using namespace std;


string textToImage(string text, glm::vec2 size, string name){
	BMP image;
	image.SetBitDepth(8);
	image.SetSize(size.x,size.y);
	RGBApixel FontColor;
	FontColor.Red = 0;
	FontColor.Green = 0;
	FontColor.Blue = 0;
	char* TextString;
	TextString = new char [2050];
	strcpy(TextString, text.c_str());
	int posX = 3;
	int posY = 3;
	int i = 0;
	char* def;
	def = new char [50];
	//Slicing the text in 50char parts for clarity
	while(TextString[i] != '\0'){
		int j = 0;
		memset(def, 0, 50);
		while(j < 50 && TextString[i] != '\n' && TextString[i] != '\0'){
			def[j] = TextString[i];
			j++;
			i++;
		}
		if(TextString[i] == '\n'){
			i++;
		}
		PrintString(image, def, posX,posY,6, FontColor);
		posY += 8;
	}
	//char* ruta = "assets/generatedText/";
	string ruta = "assets/generatedText/"+name+".bmp";
	char* c;
	c = new char [2050];
	strcpy(c, ruta.c_str());
	image.WriteToFile(c);
	return ruta;
}
