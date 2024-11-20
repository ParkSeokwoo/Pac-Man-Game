#pragma once

#include "Shape3d.h"
#include "Material.h"

/* Sphere class */
class Sphere : public Shape3D
{
public:
	enum DIRECTION { NONE, LEFT, UP, RIGHT, DOWN };

	Sphere();
	Sphere(float r, int sl, int st);

	void setRadius(float r);
	float getRadius() const;
	void setSlice(int sl);
	void setStack(int st);

	void setIndexPosition(int x, int y);
	int getXIndex() const;
	int getYIndex() const;
	bool isIndexPositionUpdated() const;
	void setIndexPositionUpdated(bool b);

	void setCurrentDirection(DIRECTION d);
	void setNextDirection(DIRECTION d);
	DIRECTION getCurrentDirection() const;
	DIRECTION getNextDirection() const;

	//21번 수정
	void setAlpha(float newAlpha);
	float getAlpha() const;
	//
	void updateDirection();

	void move();
	virtual void draw() const;

protected:
	float radius;
	int slice;
	int stack;
	float alpha;
	int idxPos[2];
	bool bInxPosUpdated;

	DIRECTION currDirection;
	DIRECTION nextDirection;
};

/* Pacman class */
class PacMan : public Sphere
{
public:
	PacMan(float r, int sl, int st, bool bCol);

	void setCollided(bool bCol);
	bool getCollided();

	virtual void draw() const;

private:
	bool bCollided;
};

/* Ghost class */
class Ghost : public Sphere
{
public:
	enum GHOSTSTATE { CHASE, SCATTER, EATEN, FRIGHTENEND };
	enum GHOSTNAME { BLINKY, PINKY, INKY, CLYDE };

	Ghost(float r, int sl, int st, GHOSTSTATE s, GHOSTNAME n);

	// 22번 수정
	void setState(GHOSTSTATE s);
	void setChange_state(bool v);

	GHOSTSTATE getState() const;
	GHOSTNAME getGhostname() const;
	bool getChange_state() const;
	//

private:
	GHOSTSTATE state;
	GHOSTNAME name;
	bool isChange_state;
};