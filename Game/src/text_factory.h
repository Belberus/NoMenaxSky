#ifndef TEXT_FACTORY_H_
#define TEXT_FACTORY_H_

#include <EasyBMP.h>
#include <EasyBMP_Font.h>

#include <string>
#include <glm/glm.hpp>

using namespace std;

string textToImage(string text, glm::vec2 size, string route);

#endif // TEXT_FACTORY_H