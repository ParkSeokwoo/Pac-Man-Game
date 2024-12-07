#pragma once

#include "Shape3d.h"
#include "Material.h"

/* Sphere class */
class Sphere : public Shape3D
{
public:
	enum DIRECTION { NONE, LEFT, UP, RIGHT, DOWN, WAIT};

	Sphere();
	Sphere(float r, int sl, int st, bool bCol);

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
	void setAlpha(float newAlpha);
	float getAlpha() const;
	void updateDirection();

	void move();
	virtual void draw() const;
	void setCollided(bool bCol);
	bool getCollided();

protected:
	float radius;
	int slice;
	int stack;
	float alpha;
	int idxPos[2];
	bool bInxPosUpdated;
	bool bCollided;

	DIRECTION currDirection;
	DIRECTION nextDirection;
};

/* Pacman class */
class PacMan : public Sphere
{
public:
	PacMan(float r, int sl, int st);
	void setLife(int a);
	int getLife();

	virtual void draw() const;

private:
	int life;
};

/* Ghost class */
class Ghost : public Sphere
{
public:
	enum GHOSTSTATE { CHASE, SCATTER, EATEN, FRIGHTENEND, GHOSTROOM };
	enum GHOSTNAME { NONE, BLINKY, PINKY, INKY, CLYDE };
	Ghost();
	Ghost(float r, int sl, int st, GHOSTSTATE s, GHOSTNAME n);

	// 22¹ø ¼öÁ¤
	void setState(GHOSTSTATE s);
	void setChange_state(bool v);
	void setisInGhostroom(bool v);

	GHOSTSTATE getState() const;
	GHOSTNAME getGhostname() const;
	bool getChange_state() const;
	bool getisInGhostroom() const;
	//

private:
	GHOSTSTATE state;
	GHOSTNAME name;
	bool isChange_state;
	bool isInGhostroom;
};