#pragma once
#include <iostream>
#include <GL/freeglut.h>
#include <FreeImage.h>
using namespace std;

class Texture
{
public:
	Texture();

	FIBITMAP* createBitMap(char const* filename);
	void generateTexture();
	void initializeTexture(char const* filename);
	GLuint getTextureId() const;
	float getAspectRatio() const;

private:
	GLuint textureID;
	GLubyte* textureData;
	int imageWidth, imageHeight;
	char filename;
	float aspectRatio = 0.0f;
};

