#pragma once

#include "Sphere.h"
#include "Constants.h"

#include <array>

/* Block class */
class Block : public Shape3D
{
public:
	Block();
	enum BOX_TYPE { NONE, ST1, ST2, ST3, ST4, ST5, ST6, ST7 };
	//16번 수정
	enum POINT_TYPE { NOPT, SMALL, BIG };
	//
	Block(float x, float y, float z, float w, float h);

	void setWidth(float w);
	void setHeight(float h);
	void setPassable(bool v);
	void setRotate(float r);
	void setBoxType(BOX_TYPE bt);
	// 18번 수정
	void setPoint(POINT_TYPE pt);
	void setColor(float r, float g, float b);
	//

	float getWidth() const;
	float getHeight() const;
	bool isPassable() const;
	POINT_TYPE getPoint_type() const;

	virtual void draw() const;
	
private:
	float width, height;
	bool bPassable;
	float rotate;
	BOX_TYPE type;
	// 19번 수정
	POINT_TYPE bPoint;
	Vector3f box_color;
	//
};

/* Map class */
class Map
{
public:
	Map();

	void createMap();

	const Block& getBlock(int r, int c) const;

	void draw() const;
	// 20번 수정
	void setPoint_type(int x, int y, Block::POINT_TYPE pt);
	Block::POINT_TYPE getPoint_type(int x, int y);
	void setBox_color(int x, int y, float r, float g, float b);
	//
private:
	std::array<std::array<Block, NUM_COL>, NUM_ROW>  blocks;
};