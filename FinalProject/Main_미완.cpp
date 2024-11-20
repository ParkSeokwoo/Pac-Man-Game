#include <iostream>
#include <vector>

#include "Constants.h"

#include "Light.h"
#include "Sphere.h"
#include "Map.h"

#include "CollisionHandler.h"
#include <GL/freeglut.h>

using namespace std;

const int FPS = 60;
int sTime = 0;
int eTime = 0;

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);

PacMan pacman(BLOCK_SIZE / 2.0f, 20, 20, false);

Ghost blinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE, Ghost::BLINKY);
Ghost pinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE, Ghost::PINKY);
Ghost inky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE, Ghost::INKY);
Ghost clyde(BLOCK_SIZE / 2.0f, 20, 20, Ghost::CHASE, Ghost::CLYDE);
vector<Ghost*> ghosts = { &blinky, &pinky, &inky, &clyde };

// PacMan pacman(BLOCK_SIZE,  20, 20, false);
// Ghost blinky(BLOCK_SIZE, 20, 20, Ghost::CHASE);

Map map;
CollisionHandler colHandler;

void initialize() {
	// Light
	light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
	light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

	// PacMan
	Material mtl, mtl1, mtl2, mtl3, mtl4;	// basic material
	mtl.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtl.setAmbient(0.6f, 0.6f, 0.0f, 1.0f);
	mtl.setDiffuse(0.8f, 0.8f, 0.0f, 1.0f);
	mtl.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl.setShininess(30.0f);

	pacman.setIndexPosition(17,13);
	pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	pacman.setMTL(mtl);

	// Ghost
	mtl1.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtl1.setAmbient(0.6f, 0.0f, 0.0f, 1.0f);
	mtl1.setDiffuse(0.8f, 0.0f, 0.0f, 1.0f);
	mtl1.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl1.setShininess(30.0f);

	mtl2.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtl2.setAmbient(0.0f, 0.6f, 0.6f, 1.0f);
	mtl2.setDiffuse(0.0f, 0.8f, 0.8f, 1.0f);
	mtl2.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl2.setShininess(30.0f);

	mtl3.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtl3.setAmbient(0.6f, 0.4f, 0.6f, 1.0f);
	mtl3.setDiffuse(0.8f, 0.4f, 0.8f, 1.0f);
	mtl3.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl3.setShininess(30.0f);

	mtl4.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtl4.setAmbient(0.6f, 0.4f, 0.0f, 1.0f);
	mtl4.setDiffuse(0.8f, 0.5f, 0.0f, 1.0f);
	mtl4.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtl4.setShininess(30.0f);

	blinky.setIndexPosition(1, 26);
	pinky.setIndexPosition(1, 1);
	inky.setIndexPosition(29, 1);
	clyde.setIndexPosition(29, 26);


	blinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	blinky.setMTL(mtl1);
	pinky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	pinky.setMTL(mtl2);
	inky.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	inky.setMTL(mtl3);
	clyde.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	clyde.setMTL(mtl4);


	// Map
	map.createMap();
}

void updateDirectionOfPacMan() {
	int idx[2] = { pacman.getXIndex(), pacman.getYIndex() };

	int lIdx[2] = { idx[0], idx[1] - 1 };// left
	int tIdx[2] = { idx[0] - 1, idx[1] };// top
	int rIdx[2] = { idx[0], idx[1] + 1 };// right
	int bIdx[2] = { idx[0] + 1, idx[1] };// bottom
	if (idx[0] == 14 && idx[1] == 0) {
		lIdx[1] = 27;// left
	}
	else if (idx[0] == 14 && idx[1] == 27) {
		rIdx[1] = 0;// right
	}
	
	const Block& lBlock = map.getBlock(lIdx[0], lIdx[1]);// left
	const Block& tBlock = map.getBlock(tIdx[0], tIdx[1]);// top
	const Block& rBlock = map.getBlock(rIdx[0], rIdx[1]);// right
	const Block& bBlock = map.getBlock(bIdx[0], bIdx[1]);// bottom

	// update direction
	Sphere::DIRECTION nextDir = pacman.getNextDirection();
	switch (nextDir)
	{
	case Sphere::NONE:
		break;
	case Sphere::LEFT:
		if (lBlock.isPassable())
			pacman.updateDirection();
		break;
	case Sphere::UP:
		if (tBlock.isPassable())
			pacman.updateDirection();
		break;
	case Sphere::RIGHT:
		if (rBlock.isPassable())
			pacman.updateDirection();
		break;
	case Sphere::DOWN:
		if (bBlock.isPassable())
			pacman.updateDirection();
		break;
	default:
		break;
	}
}

void updateDirectionOfGhost(Ghost& ghost, int targetX, int targetY) {
	int idx[2] = { ghost.getXIndex(), ghost.getYIndex() };

	int lIdx[2] = { idx[0], idx[1] - 1 };// left
	int tIdx[2] = { idx[0] - 1, idx[1] };// top
	int rIdx[2] = { idx[0], idx[1] + 1 };// right
	int bIdx[2] = { idx[0] + 1, idx[1] };// bottom
	if (idx[0] == 14 && idx[1] == 0) {
		lIdx[1] = 27;// left
	}
	else if (idx[0] == 14 && idx[1] == 27) {
		rIdx[1] = 0;// right
	}

	const Block& lBlock = map.getBlock(lIdx[0], lIdx[1]);// left
	const Block& tBlock = map.getBlock(tIdx[0], tIdx[1]);// top
	const Block& rBlock = map.getBlock(rIdx[0], rIdx[1]);// right
	const Block& bBlock = map.getBlock(bIdx[0], bIdx[1]);// bottom

	Block nextblocks[4] = { tBlock, lBlock, bBlock, rBlock };
	int indices[4][2] = { { tIdx[0], tIdx[1] }, { lIdx[0], lIdx[1] }, { bIdx[0], bIdx[1] },	{ rIdx[0], rIdx[1] } };
	Sphere::DIRECTION directions[4] = { Sphere::DIRECTION::UP, Sphere::DIRECTION::LEFT, Sphere::DIRECTION::DOWN, Sphere::DIRECTION::RIGHT };

	Sphere::DIRECTION currDir = ghost.getCurrentDirection();
	Sphere::DIRECTION newDir = Sphere::DIRECTION::NONE;
	Sphere::DIRECTION reverseDir = Sphere::DIRECTION::NONE;
	int minIdxDist = INT_MAX;

	switch (currDir) {
	case Sphere::DIRECTION::UP:
		reverseDir = Sphere::DIRECTION::DOWN;
		break;
	case Sphere::DIRECTION::LEFT:
		reverseDir = Sphere::DIRECTION::RIGHT;
		break;
	case Sphere::DIRECTION::DOWN:
		reverseDir = Sphere::DIRECTION::UP;
		break;
	case Sphere::DIRECTION::RIGHT:
		reverseDir = Sphere::DIRECTION::LEFT;
		break;
	default: break;
	}

	for (int i = 0; i < 4; i++) {
		if (directions[i] != reverseDir && nextblocks[i].isPassable()) {
			int dist_squre = (indices[i][0] - targetX) * (indices[i][0] - targetX) + (indices[i][1] - targetY) * (indices[i][1] - targetY);
			if (dist_squre < minIdxDist) {
				minIdxDist = dist_squre;
				newDir = directions[i];
			}
		}
	}
	ghost.setNextDirection(newDir);
	ghost.updateDirection();
}

void updatePacMan() {
	// update PacMan
	bool bNoDir = pacman.getCurrentDirection() == Sphere::DIRECTION::NONE;
	bool bIdxPosUpdated = pacman.isIndexPositionUpdated();
	if (bNoDir || bIdxPosUpdated) {
		updateDirectionOfPacMan();
		colHandler(pacman, map);
	}
	pacman.move();
}

void updateGhost() {
	for (auto* ghost : ghosts) {
		if (!ghost->isIndexPositionUpdated()) continue;

		int targetX = pacman.getXIndex();
		int targetY = pacman.getYIndex();

		if (ghost->getState() == Ghost::STATE::CHASE) {
			int distance = 0;
			switch (ghost->getGhostname()) {
			case Ghost::BLINKY:
				updateDirectionOfGhost(*ghost, targetX, targetY);
				break;

			case Ghost::PINKY:
				switch (pacman.getCurrentDirection()) {
				case Sphere::UP:    targetX -= 4; targetY -= 3; break;
				case Sphere::RIGHT: targetY += 4; break;
				case Sphere::DOWN:  targetX += 4; break;
				case Sphere::LEFT:  targetY -= 4; break;
				default: break;
				}
				updateDirectionOfGhost(*ghost, targetX, targetY);
				break;

			case Ghost::INKY:
				switch (pacman.getCurrentDirection()) {
				case Sphere::UP:    targetX -= 2; break;
				case Sphere::RIGHT: targetY += 2; break;
				case Sphere::DOWN:  targetX += 2; break;
				case Sphere::LEFT:  targetY -= 2; break;
				default: break;
				}
				targetX = 2 * targetX - blinky.getXIndex();
				targetY = 2 * targetY - blinky.getYIndex();
				updateDirectionOfGhost(*ghost, targetX, targetY);
				break;

			case Ghost::CLYDE:
				distance = abs(ghost->getXIndex() - pacman.getXIndex()) + abs(ghost->getYIndex() - pacman.getYIndex());
				if (distance > 8) {
					ghost->setState(Ghost::STATE::SCATTER);
				}
				else {
					updateDirectionOfGhost(*ghost, targetX, targetY);
				}
				break;

			default:
				break;
			}
		}
		else { // SCATTER mode
			switch (ghost->getGhostname()) {
			case Ghost::BLINKY:
			case Ghost::PINKY:
				updateDirectionOfGhost(*ghost, 1, 1);
				break;
			case Ghost::INKY:
				updateDirectionOfGhost(*ghost, 30, 27);
				break;
			case Ghost::CLYDE:
				updateDirectionOfGhost(*ghost, 29, 1);
				break;
			default:
				break;
			}
		}

		// Move ghost after updating direction
		ghost->move();
	}

}



void idle() {
	float spf = 1000.0f / FPS;
	eTime = glutGet(GLUT_ELAPSED_TIME);

	if (eTime - sTime > spf) {
		map.setPoint(pacman.getXIndex(), pacman.getYIndex());
		updatePacMan();
		updateGhost();
		colHandler(pacman, blinky);
		colHandler(pacman, pinky);
		colHandler(pacman, inky);
		colHandler(pacman, clyde);

		sTime = eTime;
		glutPostRedisplay();
	}
}

//void displayCharacters(void* font, string str, float x, float y) {
//	glColor3f(1.0f, 1.0f, 1.0f);
//	glRasterPos2f(x, y);
//	for (int i = 0; i < str.size(); i++)
//		glutBitmapCharacter(font, str[i]);
//}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
	// gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw 2D
	map.draw();

	// Draw 3D
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	pacman.draw();
	for (auto* ghost : ghosts) {
		ghost->draw();
	}


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(light.getID());

	glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y) {
	if (key == '1') {
		for (auto* ghost : ghosts) {
			if (ghost->getState() == Ghost::STATE::CHASE) {
				ghost->setState(Ghost::STATE::SCATTER);
			}
			else {
				ghost->setState(Ghost::STATE::CHASE);
			}
		}
	}
}


void specialKeyDown(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		pacman.setNextDirection(Sphere::DIRECTION::LEFT);
		break;
	case GLUT_KEY_UP:
		pacman.setNextDirection(Sphere::DIRECTION::UP);
		break;
	case GLUT_KEY_RIGHT:
		pacman.setNextDirection(Sphere::DIRECTION::RIGHT);
		break;
	case GLUT_KEY_DOWN:
		pacman.setNextDirection(Sphere::DIRECTION::DOWN);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutCreateWindow("Pac-Man Map");

	initialize();

	// register callbacks
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(specialKeyDown);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
