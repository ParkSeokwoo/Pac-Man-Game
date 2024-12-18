#include "Sphere.h"
#include "GL/freeglut.h"
#include "Constants.h"

#include <iostream>
#include <valarray>
using namespace std;

//6번 수정
Sphere::Sphere() : radius(0.0f), slice(0), stack(0), alpha(1.0f), idxPos{ 0, 0 }, bInxPosUpdated(true),
currDirection(NONE), nextDirection(NONE), bCollided(false) {}
Sphere::Sphere(float r, int sl, int st, bool bCol)
	: radius(r), slice(sl), stack(st), alpha(1.0f), idxPos{ 0, 0 }, bInxPosUpdated(true),
	currDirection(NONE), nextDirection(NONE), bCollided(false) {}
//
void Sphere::setRadius(float r) {
	radius = r;
}
float Sphere::getRadius() const {
	return radius;
}
void Sphere::setSlice(int sl) {
	slice = sl;
}
void Sphere::setStack(int st) {
	stack = st;
}

void Sphere::setIndexPosition(int x, int y) {
	idxPos[0] = x;
	idxPos[1] = y;
	//7번 수정
	setCenter((float)(LEFT_BOUNDARY + idxPos[1] * BLOCK_SIZE), (float)(TOP_BOUNDARY - idxPos[0] * BLOCK_SIZE), 0.0f);
	//
}

int Sphere::getXIndex() const {
	return idxPos[0];
}
int Sphere::getYIndex() const {
	return idxPos[1];
}
bool Sphere::isIndexPositionUpdated() const {
	return bInxPosUpdated;
}

void Sphere::setIndexPositionUpdated(bool b) {
	bInxPosUpdated = b;
}

void Sphere::setCurrentDirection(DIRECTION d) {
	currDirection = d;
}
void Sphere::setNextDirection(DIRECTION d) {
	nextDirection = d;
}
Sphere::DIRECTION Sphere::getCurrentDirection() const {
	return currDirection;
}
Sphere::DIRECTION Sphere::getNextDirection() const {
	return nextDirection;
}
void Sphere::setAlpha(float newAlpha) {
	alpha = std::max(0.0f, std::min(1.0f, newAlpha));
}

float Sphere::getAlpha() const {
	return alpha;
}
// 8번 수정
void Sphere::updateDirection() {
	currDirection = nextDirection;
}
//
void Sphere::move() {
	Shape3D::move();

	int xFromIdx = LEFT_BOUNDARY + idxPos[1] * BLOCK_SIZE;
	int yFromIdx = TOP_BOUNDARY - idxPos[0] * BLOCK_SIZE;

	if (velocity[0] > 0.0f) {
		if (center[0] - radius * 2.0f >= xFromIdx) {
			if (idxPos[0] == 14 && idxPos[1] == 27) {
				setIndexPosition(idxPos[0], 0);
			}
			else {
				setIndexPosition(idxPos[0], idxPos[1] + 1);
			}
			bInxPosUpdated = true;
		}
		else {
			bInxPosUpdated = false;
		}
	}
	else if (velocity[0] < 0.0f) {
		if (center[0] + radius * 2.0f <= xFromIdx) {
			if (idxPos[0] == 14 && idxPos[1] == 0) {
				setIndexPosition(idxPos[0], 27);
			}
			else {
				setIndexPosition(idxPos[0], idxPos[1] - 1);
			}
			bInxPosUpdated = true;
		}
		else {
			bInxPosUpdated = false;
		}
	}
	else if (velocity[1] > 0.0f) {
		if (center[1] - radius * 2.0f >= yFromIdx) {
			setIndexPosition(idxPos[0] - 1, idxPos[1]);
			bInxPosUpdated = true;
		}
		else {
			bInxPosUpdated = false;
		}
	}
	else if (velocity[1] < 0.0f) {

		if (center[1] + radius * 2.0f <= yFromIdx) {
			setIndexPosition(idxPos[0] + 1, idxPos[1]);
			bInxPosUpdated = true;
		}
		else {
			bInxPosUpdated = false;
		}
	}
}

//9번 수정
void Sphere::draw() const {
	glPushMatrix();

	float emissionArray[] = { mtl.getEmission()[0], mtl.getEmission()[1], mtl.getEmission()[2], alpha };
	float ambientArray[] = { mtl.getAmbient()[0], mtl.getAmbient()[1], mtl.getAmbient()[2], alpha };
	float diffuseArray[] = { mtl.getDiffuse()[0], mtl.getDiffuse()[1], mtl.getDiffuse()[2], alpha };
	float specularArray[] = { mtl.getSpecular()[0], mtl.getSpecular()[1], mtl.getSpecular()[2], alpha };
	float shininessArray[] = { mtl.getShininess() };

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininessArray);

	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(R_RATIO * radius, slice, stack);

	glPopMatrix();
}

void Sphere::setCollided(bool bCol) {
	bCollided = bCol;
}

bool Sphere::getCollided() {
	return bCollided;
}
//

//10번 수정
PacMan::PacMan(float r, int sl, int st)
	: Sphere(r, sl, st, false), life(1) {}
//

void PacMan::draw() const {
	glPushMatrix();

	float emissionArray[] = { mtl.getEmission()[0], mtl.getEmission()[1], mtl.getEmission()[2], alpha };
	float ambientArray[] = { mtl.getAmbient()[0], mtl.getAmbient()[1], mtl.getAmbient()[2], alpha };
	float diffuseArray[] = { mtl.getDiffuse()[0], mtl.getDiffuse()[1], mtl.getDiffuse()[2], alpha };
	float specularArray[] = { mtl.getSpecular()[0], mtl.getSpecular()[1], mtl.getSpecular()[2], alpha };
	float shininessArray[] = { mtl.getShininess() };

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininessArray);

	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(R_RATIO * radius, slice, stack);

	glPopMatrix();
}

void PacMan::setLife(int a) {
	life = a;
}
int PacMan::getLife() {
	return life;
}

//11번 수정
Ghost::Ghost(): state(Ghost::GHOSTSTATE::SCATTER), name(Ghost::GHOSTNAME::NONE), isChange_state(false), isInGhostroom(false) {}

Ghost::Ghost(float r, int sl, int st, Ghost::GHOSTSTATE s, Ghost::GHOSTNAME n)
	: Sphere(r, sl, st, false), state(s), name(n), isChange_state(false), isInGhostroom(false) {}
//

void Ghost::setState(Ghost::GHOSTSTATE s) {
	state = s;
}

//12번 수정
void Ghost::setChange_state(bool v) {
	isChange_state = v;
}

void Ghost::setisInGhostroom(bool v) {
	isInGhostroom = v;
}
//

Ghost::GHOSTSTATE Ghost::getState() const {
	return state;
}

Ghost::GHOSTNAME Ghost::getGhostname() const {
	return name;
}

//13번 수정
bool Ghost::getChange_state() const{
	return isChange_state;
}

bool Ghost::getisInGhostroom() const {
	return isInGhostroom;
}
//
