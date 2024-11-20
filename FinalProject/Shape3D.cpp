#include "Shape3D.h"

Shape3D::Shape3D() {
	setCenter(0.0f, 0.0f, 0.0f);
	setVelocity(0.0f, 0.0f, 0.0f);

	mtl.setEmission(0.0f, 0.0f, 0.0f, 0.0f);
	mtl.setAmbient(0.0f, 0.0f, 0.0f, 0.0f);
	mtl.setDiffuse(0.0f, 0.0f, 0.0f, 0.0f);
	mtl.setSpecular(0.0f, 0.0f, 0.0f, 0.0f);
	mtl.setShininess(10.0f);
}

Shape3D::Shape3D(float x, float y, float z) {
	setCenter(x, y, z);
}

void Shape3D::setCenter(float x, float y, float z) {
	center.setPos(x, y, z);
}
void Shape3D::setCenter(const Vector3f& c) {
	center = c;
}
Vector3f Shape3D::getCenter() const {
	return center;
}

void Shape3D::setVelocity(float x, float y, float z) {
	velocity.setPos(x, y, z);
}
void Shape3D::setVelocity(const Vector3f& v) {
	velocity = v;
}
Vector3f Shape3D::getVelocity() const {
	return velocity;
}

void Shape3D::setMTL(const Material& m) {
	mtl.setEmission(m.getEmission()[0], m.getEmission()[1], m.getEmission()[2], m.getEmission()[3]);
	mtl.setAmbient(m.getAmbient()[0], m.getAmbient()[1], m.getAmbient()[2], m.getAmbient()[3]);
	mtl.setDiffuse(m.getDiffuse()[0], m.getDiffuse()[1], m.getDiffuse()[2], m.getDiffuse()[3]);
	mtl.setSpecular(m.getSpecular()[0], m.getSpecular()[1], m.getSpecular()[2], m.getSpecular()[3]);
	mtl.setShininess(m.getShininess());
}
const Material& Shape3D::getMTL() const {
	return mtl;
}

void Shape3D::move() {
	center = center + velocity;
}