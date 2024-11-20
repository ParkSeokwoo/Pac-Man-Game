#include "Map.h"
#include "GL/freeglut.h"
#include <iostream>
#include <fstream>
#include <string>
// 2번 수정
Block::Block() {
	width = 0.0f;
	height = 0.0f;
	bPassable = true;
	rotate = 0.0f;
	type = BOX_TYPE::NONE;
	bPoint = POINT_TYPE::SMALL;
	box_color = Vector3f(1.0f, 0.8f, 0.6f);
}
Block::Block(float x, float y, float z, float w, float h) {
	Shape3D::setCenter(x, y, z);
	width = w;
	height = h;
	bPassable = true;
	rotate = 0.0f;
	type = BOX_TYPE::NONE;
	bPoint = POINT_TYPE::SMALL;
	box_color = Vector3f(1.0f, 0.8f, 0.6f);
}
//
void Block::setWidth(float w) {
	width = w;
}
void Block::setHeight(float h) {
	height = h;
}
void Block::setPassable(bool v) {
	bPassable = v;
}
void Block::setRotate(float r) {
	rotate = r;
}
void Block::setBoxType(BOX_TYPE bt) {
	type = bt;
}
//17번 수정
void Block::setPoint(POINT_TYPE pt) {
	bPoint = pt;
}
void Block::setColor(float r, float g, float b) {
	box_color[0] = r; box_color[1] = g; box_color[2] = b;
}
//


float Block::getWidth() const {
	return width;
}
float Block::getHeight() const {
	return height;
}
bool Block::isPassable() const {
	return bPassable;
}
Block::POINT_TYPE Block::getPoint_type() const {
	return bPoint;
}

void drawQuarterCircle(float width, float height, int segments) {
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= segments; ++i) {
			float theta = (M_PI / 2.0f) * float(i) / float(segments);
			float x = width * cos(theta);
			float y = height * sin(theta);
			glVertex3f(x, y, 0.0);
		}
	glEnd();
}

void Block::draw() const {
	if (!bPassable) {
		glPushMatrix();

		glTranslatef(center[0], center[1], center[2]);
		glRotatef(rotate, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);

		switch (type) {
		case BOX_TYPE::ST1: {
			glPushMatrix();

			glTranslatef(-width / 2, -height / 2, 0.0f);

			drawQuarterCircle(width, width, 20);
			drawQuarterCircle(width/2, width/2, 10);

			glPopMatrix();
			break;
		}
		case BOX_TYPE::ST2: {
			glBegin(GL_LINES);
				glVertex3f(-width / 2, -height / 2, 0.0f); 
				glVertex3f(-width / 2, height / 2, 0.0f); 

				glVertex3f(0.0f, -height / 2, 0.0f);
				glVertex3f(0.0f, height / 2, 0.0f);
			glEnd();
			break;
		}
		case BOX_TYPE::ST3: {
			glPushMatrix();

			glTranslatef(-width / 2, -height / 2, 0.0f);

			drawQuarterCircle(width / 2, width / 2, 10);

			glPopMatrix();
			break;
		}
		case BOX_TYPE::ST4: {
			glPushMatrix();

			glTranslatef(-width / 2, -height / 2, 0.0f);

			drawQuarterCircle(width / 2, width / 2, 10);

			glPopMatrix();

			glBegin(GL_LINES);
				glVertex3f(width / 2, -height / 2, 0.0f);
				glVertex3f(width / 2, height / 2, 0.0f);
			glEnd();
			break;
		}
		case BOX_TYPE::ST5: {
			glPushMatrix();

			glTranslatef(-width / 2, -height / 2, 0.0f);

			drawQuarterCircle(width / 2, width / 2, 10);

			glPopMatrix();

			glBegin(GL_LINES);
				glVertex3f(-width / 2, height / 2, 0.0f);
				glVertex3f(width / 2, height / 2, 0.0f);
			glEnd();
			break;
			break;
		}
		case BOX_TYPE::ST6: {
			glBegin(GL_LINES);
				glVertex3f(0.0f, -height / 2, 0.0f);
				glVertex3f(0.0f, height / 2, 0.0f);
			glEnd();
			break;
		}
		case BOX_TYPE::ST7: {
			glBegin(GL_LINES);
				glVertex3f(0.0f, -height / 2, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);

				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(-width / 2, 0.0f, 0.0f);
			glEnd();
			break;
		}
		}
		glPopMatrix();
	}
	// 3번 수정
	else {
		float radius = 0;
		int numSegments = 0;
		if (bPoint == POINT_TYPE::NOPT) {
			return;
		}
		if (bPoint == POINT_TYPE::SMALL) {
			radius = width / 8;
			numSegments = 20;
		}			
		else if (bPoint == POINT_TYPE::BIG) {
			radius = width / 2.5;
			numSegments = 50;
		}
	//
		glPushMatrix();

		glTranslatef(center[0], center[1], center[2]);
		glColor3f(box_color[0], box_color[1], box_color[2]);

		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i <= numSegments; ++i) {
			float theta = 2.0f * M_PI * float(i) / float(numSegments);
			float x = radius * cos(theta);
			float y = radius * sin(theta);
			glVertex2f(x, y);
		}
		glEnd();
		glPopMatrix();
	}
}


Map::Map() {
	blocks = {};
}
// 4번 수정 
void Map::createMap() {
	std::ifstream file("boxtype.txt");
	while (file) {
		int x, y, t;
		float r;
		file >> x >> y >> r >> t;
		blocks[x][y].setPassable(false);
		blocks[x][y].setRotate(r);
		blocks[x][y].setBoxType(static_cast<Block::BOX_TYPE>(t));
		blocks[x][y].setPoint(Block::POINT_TYPE::NOPT);
	}

	blocks[14][0].setPassable(true);
	blocks[14][27].setPassable(true);
	blocks[14][0].setPoint(Block::POINT_TYPE::SMALL);
	blocks[14][27].setPoint(Block::POINT_TYPE::SMALL);

	blocks[3][1].setPoint(Block::POINT_TYPE::BIG);
	blocks[3][26].setPoint(Block::POINT_TYPE::BIG);
	blocks[23][1].setPoint(Block::POINT_TYPE::BIG);
	blocks[23][26].setPoint(Block::POINT_TYPE::BIG);

	file.close();
	float x = LEFT_BOUNDARY;
	float y = TOP_BOUNDARY;
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			blocks[i][j].setCenter(x + j * BLOCK_SIZE, y - i * BLOCK_SIZE, 0.0f);
			blocks[i][j].setWidth(BLOCK_SIZE);
			blocks[i][j].setHeight(BLOCK_SIZE);
		}
	}
}
//
const Block& Map::getBlock(int r, int c) const {
	return blocks[r][c];
}

void Map::draw() const {
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			blocks[i][j].draw();
		}
	}
}

void Map::setPoint_type(int x, int y, Block::POINT_TYPE pt) {
	blocks[x][y].setPoint(pt);
}
// 5번 수정
Block::POINT_TYPE Map::getPoint_type(int x, int y) {
	return blocks[x][y].getPoint_type();
}

void Map::setBox_color(int x, int y, float r, float g, float b) {
	blocks[x][y].setColor(r, g, b);
}
//