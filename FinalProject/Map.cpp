#include "Map.h"
#include "GL/freeglut.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

// 2번 수정
Block::Block() {
	width = 0.0f;
	height = 0.0f;
	bPassable = true;
	rotate = 0.0f;
	type = BOX_TYPE::NONE;
	bPoint = POINT_TYPE::SMALL;
	box_color = Vector3f(0.0f, 0.0f, 1.0f);
	object_color = Vector3f(1.0f, 0.8f, 0.6f);
}
Block::Block(float x, float y, float z, float w, float h) {
	Shape3D::setCenter(x, y, z);
	width = w;
	height = h;
	bPassable = true;
	rotate = 0.0f;
	type = BOX_TYPE::NONE;
	bPoint = POINT_TYPE::SMALL;
	box_color = Vector3f(0.0f, 0.0f, 1.0f);
	object_color = Vector3f(1.0f, 0.8f, 0.6f);
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
void Block::setBoxColor(float r, float g, float b) {
	box_color[0] = r; box_color[1] = g; box_color[2] = b;
}
void Block::setObjectColor(float r, float g, float b) {
	object_color[0] = r; object_color[1] = g; object_color[2] = b;
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
Vector3f Block::getBoxColor() const {
	return box_color;
}
Vector3f Block::getObjectColor() const {
	return object_color;
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
		glColor3f(box_color[0], box_color[1], box_color[2]);

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
		case BOX_TYPE::ST8: {
			glPushMatrix();
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.8f, 0.9f);
			glVertex3f(-width / 2, -height / 2, 0.0f); 
			glVertex3f(-width / 2, height / 2, 0.0f);  
			glVertex3f(-width / 8, height / 2, 0.0f);
			glVertex3f(-width / 8, -height / 2, 0.0f);
			glEnd();
			glPopMatrix();
			break;
		}
		case BOX_TYPE::ST9: {
			glBegin(GL_LINES);
			glVertex3f(-width / 2, -height / 2, 0.0f);
			glVertex3f(-width / 2, height / 2, 0.0f);

			glVertex3f(0.0f, -height / 2, 0.0f);
			glVertex3f(0.0f, height / 2, 0.0f);

			glVertex3f(-width / 2, -height / 2, 0.0f);
			glVertex3f(0, -height / 2, 0.0f);
			glEnd();
			break;
		}
		case BOX_TYPE::ST10: {
			glBegin(GL_LINES);
			glVertex3f(-width / 2, -height / 2, 0.0f);
			glVertex3f(-width / 2, height / 2, 0.0f);

			glVertex3f(0.0f, -height / 2, 0.0f);
			glVertex3f(0.0f, height / 2, 0.0f);

			glVertex3f(-width / 2, height / 2, 0.0f);
			glVertex3f(0, height / 2, 0.0f);
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
			radius = width / 2.5f;
			numSegments = 50;
		}
	//
		glPushMatrix();

		glTranslatef(center[0], center[1], center[2]);
		glColor3f(object_color[0], object_color[1], object_color[2]);

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
	state = MAP_STATE::INIT;
}
// 4번 수정 
void Map::createMap() {
	if (state == MAP_STATE::INIT) {
		resetBlocks(false, false);
		std::ifstream file("initial_boxtype.txt");
		while (file) {
			int x, y, t;
			float r;
			file >> x >> y >> r >> t;
			blocks[x][y].setPassable(false);
			blocks[x][y].setRotate(r);
			blocks[x][y].setBoxType(static_cast<Block::BOX_TYPE>(t));
			blocks[x][y].setPoint(Block::POINT_TYPE::NOPT);
		}
		int xNopt[] = {
			9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
			9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
			21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
		};
		int yNopt[] = {
			5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,            
			5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 
			5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
			22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22             
		};

		for (int i = 0; i < sizeof(xNopt) / sizeof(xNopt[0]); ++i) {
			int x = xNopt[i];
			int y = yNopt[i];
			blocks[x][y].setPassable(true);
		}

		float x = LEFT_BOUNDARY;
		float y = TOP_BOUNDARY;
		for (int i = 0; i < blocks.size(); i++) {
			for (int j = 0; j < blocks[i].size(); j++) {
				blocks[i][j].setCenter(x + j * BLOCK_SIZE, y - i * BLOCK_SIZE, 0.0f);
				blocks[i][j].setWidth(BLOCK_SIZE);
				blocks[i][j].setHeight(BLOCK_SIZE);
			}
		}
		file.close();
	}
	else if (state == MAP_STATE::ST1) {
		resetBlocks(true);
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
		int xNopt[] = { 
			14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
			11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
			17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
			9, 10, 9, 10,
			12, 13, 15, 16, 18, 19,
			12, 13, 15, 16, 18, 19,
		};
		int yNopt[] = {
			1, 2, 3, 4, 5, 7, 8, 9, 18, 19, 20, 22, 23, 24, 25, 26,
			9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
			9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
			12, 12, 15, 15,
			9, 9, 9, 9, 9, 9, 
			18, 18, 18, 18, 18, 18
		
		};
		for (int i = 0; i < sizeof(xNopt) / sizeof(xNopt[0]); ++i) {
			int x = xNopt[i];
			int y = yNopt[i];
			blocks[x][y].setPoint(Block::POINT_TYPE::NOPT);
		}
		blocks[3][1].setPoint(Block::POINT_TYPE::BIG);
		blocks[3][26].setPoint(Block::POINT_TYPE::BIG);
		blocks[23][1].setPoint(Block::POINT_TYPE::BIG);
		blocks[23][26].setPoint(Block::POINT_TYPE::BIG);
		
		blocks[12][13].setBoxType(Block::BOX_TYPE::ST8);
		blocks[12][14].setBoxType(Block::BOX_TYPE::ST8);
		blocks[12][12].setBoxType(Block::BOX_TYPE::ST9);
		blocks[12][15].setBoxType(Block::BOX_TYPE::ST10);
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
	else if (state == MAP_STATE::GAMEEND) {
		resetBlocks(false);
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

void Map::setObject_color(int x, int y, float r, float g, float b) {
	blocks[x][y].setObjectColor(r, g, b);
}
void Map::setBox_color(int x, int y, float r, float g, float b) {
	blocks[x][y].setBoxColor(r, g, b);
}

Map::MAP_STATE Map::getState() const {
	return state;
}
void Map::setState(Map::MAP_STATE s) {
	state = s;
}
void Map::resetBlocks(bool bpt, bool bPassible) {
	// 모든 Block 객체를 기본값으로 초기화
	for (auto& row : blocks) {
		for (auto& block : row) {
			block = Block(); // Block의 기본 생성자 호출
			if (!bpt) {
				block.setPoint(Block::POINT_TYPE::NOPT);
			}
			if (!bPassible) {
				block.setPassable(false);
			}
		}
	}
}
//score 계산
int Map::getScore() {
	int count = 0;
	for (const auto& row : blocks) {
		count += std::count_if(row.begin(), row.end(), [](const Block& block) {
			return block.getPoint_type() == Block::POINT_TYPE::SMALL;
			});
	}
	return count * 10;
}
bool Map::isGameClear(int v) {
	int count = 0;
	for (const auto& row : blocks) {
		count += std::count_if(row.begin(), row.end(), [](const Block& block) {
			return block.getPoint_type() == Block::POINT_TYPE::SMALL;
			});
	}
	// cout << count << '\n';
	return count < v ? true : false;
}
Vector3f Map::getBox_Color(int x, int y) const {
	return blocks[x][y].getBoxColor();
}
Vector3f Map::getObject_Color(int x, int y) const {
	return blocks[x][y].getObjectColor();
}

//