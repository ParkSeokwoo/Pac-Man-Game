#include "Light.h"
#include "GL/freeglut.h"

Light::Light(float x, float y, float z, int L_ID) {
	pos = Vector3f(x, y, z);
	lightID = L_ID;
}

void Light::setAmbient(float r, float g, float b, float a) {
	ambient.setPos(r, g, b, a);
}
void Light::setDiffuse(float r, float g, float b, float a) {
	diffuse.setPos(r, g, b, a);
}
void Light::setSpecular(float r, float g, float b, float a) {
	specular.setPos(r, g, b, a);
}

Vector4f Light::getAmbient() const {
	return ambient;
}
Vector4f Light::getDiffuse() const {
	return diffuse;
}
Vector4f Light::getSpecular() const {
	return specular;
}
int Light::getID() const {
	return lightID;
}

void Light::draw() const {
	glPushMatrix();
	glEnable(lightID);

	float ambientArray[] = { ambient[0], ambient[1], ambient[2], ambient[3] };
	float diffuseArray[] = { diffuse[0], diffuse[1], diffuse[2], diffuse[3] };
	float specularArray[] = { specular[0], specular[1], specular[2], specular[3] };
	float positionArray[] = { pos[0], pos[1], pos[2] , 1.0f };

	glLightfv(lightID, GL_AMBIENT, ambientArray);
	glLightfv(lightID, GL_DIFFUSE, diffuseArray);
	glLightfv(lightID, GL_SPECULAR, specularArray);
	glLightfv(lightID, GL_POSITION, positionArray);

	glPopMatrix();
}