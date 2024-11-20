#include <iostream>
#include <vector>

#include "Constants.h"

#include "Light.h"
#include "Sphere.h"
#include "Map.h"
#include "Timer.h"

#include "CollisionHandler.h"
#include <GL/freeglut.h>
#include <array>

using namespace std;

const int FPS = 60;
int sTime = 0;
int eTime = 0;

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);

PacMan pacman(BLOCK_SIZE / 2.0f, 20, 20, false);

Ghost blinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::BLINKY);
Ghost pinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::PINKY);
Ghost inky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::INKY);
Ghost clyde(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::CLYDE);
vector<Ghost *> ghosts = { &blinky, &pinky, &inky, &clyde };

Map map;
CollisionHandler colHandler;
Material pacmanMtl, blinkyMtl, pinkyMtl, inkyMtl, clydeMtl, eatenMtl, frightenedMtl;
Ghost::GHOSTSTATE currState;


void initializeMaterial(Material& mtl, const std::array<float, 4>& emission, const std::array<float, 4>& ambient, const std::array<float, 4>& diffuse, const std::array<float, 4>& specular, float shininess) {
	mtl.setEmission(emission[0], emission[1], emission[2], emission[3]);
	mtl.setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
	mtl.setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	mtl.setSpecular(specular[0], specular[1], specular[2], specular[3]);
	mtl.setShininess(shininess);
}

void initializeGhost(Ghost& ghost, int x, int y, Material& mtl) {
    ghost.setIndexPosition(x, y);
    ghost.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    ghost.setMTL(mtl);
}

void initialize() {
    // Light Initialization
    light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
    light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

    // Materials Initialization
	std::array<float, 4> emission = { 0.2f, 0.2f, 0.2f, 1.0f };
    float shininess = 30.0f;

	initializeMaterial(pacmanMtl, emission,
		{ 0.6f, 0.6f, 0.0f, 1.0f },
		{ 0.8f, 0.8f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		shininess);

	initializeMaterial(blinkyMtl, emission,
		{ 0.6f, 0.0f, 0.0f, 1.0f },
		{ 0.8f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		shininess);

	initializeMaterial(pinkyMtl, emission,
		{ 0.6f, 0.4f, 0.6f, 1.0f },
		{ 0.8f, 0.4f, 0.8f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		shininess);

	initializeMaterial(inkyMtl, emission,
		{ 0.0f, 0.6f, 0.6f, 1.0f },
		{ 0.0f, 0.8f, 0.8f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		shininess);

	initializeMaterial(clydeMtl, emission,
		{ 0.6f, 0.4f, 0.0f, 1.0f },
		{ 0.8f, 0.5f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		shininess);

	// Eaten Material (White)
	initializeMaterial(eatenMtl, emission,
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }, 
		shininess);

	// Frightened Material (Blue)
	initializeMaterial(frightenedMtl, emission,
		{ 0.0f, 0.0f, 0.6f, 1.0f }, 
		{ 0.0f, 0.0f, 0.8f, 1.0f }, 
		{ 0.0f, 0.0f, 1.0f, 1.0f }, 
		shininess);

    // PacMan Initialization
    pacman.setIndexPosition(17, 13);
    pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    pacman.setMTL(pacmanMtl);

    // Ghosts Initialization
    initializeGhost(blinky, 1, 26, blinkyMtl);
    initializeGhost(pinky, 1, 1, pinkyMtl);
    initializeGhost(inky, 29, 26, inkyMtl);
    initializeGhost(clyde, 29, 1, clydeMtl);

    // Map Initialization
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

	// update direction - passable하면 방향을 바꿔줌
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
	// update velocity - 좀 빠르게
	switch (pacman.getCurrentDirection()) {
	case Sphere::DIRECTION::LEFT: {
		pacman.setVelocity(-1.3f * MOVE_SPEED, 0.0f, 0.0f);
		break;
	}
	case Sphere::DIRECTION::RIGHT: {
		pacman.setVelocity(1.3f * MOVE_SPEED, 0.0f, 0.0f);
		break;
	}
	case Sphere::DIRECTION::DOWN: {
		pacman.setVelocity(0.0f, -1.3f * MOVE_SPEED, 0.0f);
		break;
	}
	case Sphere::DIRECTION::UP: {
		pacman.setVelocity(0.0f, 1.3f * MOVE_SPEED, 0.0f);
		break;
	}
	}
}

void updateDirectionOfGhost(Ghost& ghost, int targetX, int targetY, bool shortest = true) {
	int idx[2] = { ghost.getXIndex(), ghost.getYIndex() };
	int lIdx[2] = { idx[0], idx[1] - 1 };// left
	int tIdx[2] = { idx[0] - 1, idx[1] };// top
	int rIdx[2] = { idx[0], idx[1] + 1 };// right
	int bIdx[2] = { idx[0] + 1, idx[1] };// bottom
	if (idx[0] == 14 && idx[1] == 0) lIdx[1] = 27; // wrap-around left
	if (idx[0] == 14 && idx[1] == 27) rIdx[1] = 0; // wrap-around right

	const Block& lBlock = map.getBlock(lIdx[0], lIdx[1]);// left
	const Block& tBlock = map.getBlock(tIdx[0], tIdx[1]);// top
	const Block& rBlock = map.getBlock(rIdx[0], rIdx[1]);// right
	const Block& bBlock = map.getBlock(bIdx[0], bIdx[1]);// bottom

	Block nextblocks[4] = { tBlock, lBlock, bBlock, rBlock };

	int indices[4][2] = { { tIdx[0], tIdx[1] }, { lIdx[0], lIdx[1] }, { bIdx[0], bIdx[1] },	{ rIdx[0], rIdx[1] } };
	Sphere::DIRECTION directions[4] = { Sphere::DIRECTION::UP, Sphere::DIRECTION::LEFT, Sphere::DIRECTION::DOWN, Sphere::DIRECTION::RIGHT };
	Sphere::DIRECTION newDir = Sphere::DIRECTION::NONE;

	// state가 변할 때에는 reverDir이 필요없으므로 NONE으로 만듦
	// 그게 아니면 반대방향
	Sphere::DIRECTION reverseDir = Sphere::DIRECTION::NONE;
	if (ghost.getChange_state() == false) {
		switch (ghost.getCurrentDirection()) {
		case Sphere::UP:
			reverseDir = Sphere::DOWN;
			break;
		case Sphere::LEFT:
			reverseDir = Sphere::RIGHT;
			break;
		case Sphere::DOWN:
			reverseDir = Sphere::UP;
			break;
		case Sphere::RIGHT:
			reverseDir = Sphere::LEFT;
			break;
		default:
			reverseDir = Sphere::NONE;
			break;
		}
	}
	else {
		ghost.setChange_state(false);
	}
	// scatter나 chase, eaten의 경우 shortest가 true -> 최소방향 찾기
	if (shortest) {
		int minIdxDist = INT_MAX;
		for (int i = 0; i < 4; i++) {
			if (directions[i] != reverseDir && nextblocks[i].isPassable()) {
				int dist_squre = (indices[i][0] - targetX) * (indices[i][0] - targetX) + (indices[i][1] - targetY) * (indices[i][1] - targetY);
				if (dist_squre < minIdxDist) {
					minIdxDist = dist_squre;
					newDir = directions[i];
				}
			}
		}
	}
	// frightened이 경우 shortest가 false -> 최대방향 찾기
	else {
		int maxIdxDist = 0;
		for (int i = 0; i < 4; i++) {
			if (directions[i] != reverseDir && nextblocks[i].isPassable()) {
				int dist_squre = (indices[i][0] - targetX) * (indices[i][0] - targetX) + (indices[i][1] - targetY) * (indices[i][1] - targetY);
				if (dist_squre > maxIdxDist) {
					maxIdxDist = dist_squre;
					newDir = directions[i];
				}
			}
		}
	}
	
	ghost.setNextDirection(newDir);
	ghost.updateDirection();
	// 현재 상태에 맞게 속도 조절 -> frightened 느리고, eaten 빠르게
	if (ghost.getState() == Ghost::GHOSTSTATE::CHASE || ghost.getState() == Ghost::GHOSTSTATE::SCATTER) {
		switch (ghost.getCurrentDirection()) {
		case Sphere::DIRECTION::LEFT: {
			ghost.setVelocity(-MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::RIGHT: {
			ghost.setVelocity(MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::DOWN: {
			ghost.setVelocity(0.0f, -MOVE_SPEED, 0.0f);
			break;
		}
		case Sphere::DIRECTION::UP: {
			ghost.setVelocity(0.0f, MOVE_SPEED, 0.0f);
			break;
		}
		}
	}
	else if (ghost.getState() == Ghost::GHOSTSTATE::FRIGHTENEND) {
		switch (ghost.getCurrentDirection()) {
		case Sphere::DIRECTION::LEFT: {
			ghost.setVelocity(-0.6f * MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::RIGHT: {
			ghost.setVelocity(0.6f * MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::DOWN: {
			ghost.setVelocity(0.0f, -0.6f * MOVE_SPEED, 0.0f);
			break;
		}
		case Sphere::DIRECTION::UP: {
			ghost.setVelocity(0.0f, 0.6f * MOVE_SPEED, 0.0f);
			break;
		}
		}
	}
	else if (ghost.getState() == Ghost::GHOSTSTATE::EATEN) {
		switch (ghost.getCurrentDirection()) {
		case Sphere::DIRECTION::LEFT: {
			ghost.setVelocity(-4 * MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::RIGHT: {
			ghost.setVelocity(4 * MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::DOWN: {
			ghost.setVelocity(0.0f, -4 * MOVE_SPEED, 0.0f);
			break;
		}
		case Sphere::DIRECTION::UP: {
			ghost.setVelocity(0.0f, 4 * MOVE_SPEED, 0.0f);
			break;
		}
		}
	}
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
	// 각 ghost 마다 STATE 별로 direction update 후 move
	int targetx = pacman.getXIndex(), targety = pacman.getYIndex();

	//blinky
	bool bIdxPosUpdated = blinky.isIndexPositionUpdated();
	if (bIdxPosUpdated) {
		switch (blinky.getState()) {
		case Ghost::GHOSTSTATE::CHASE:
			updateDirectionOfGhost(blinky, targetx, targety);
			break;
		case Ghost::GHOSTSTATE::SCATTER:
			updateDirectionOfGhost(blinky, 0, 27);
			break;
		// EATEN의 경우 목표 위치 도달시 currState로 바꿔줌
		case Ghost::GHOSTSTATE::EATEN: 
			if (blinky.getXIndex() == 1 && blinky.getYIndex() == 26) {
				blinky.setMTL(blinkyMtl);
				blinky.setState(currState);
				blinky.setChange_state(true);
			}
			updateDirectionOfGhost(blinky, 0, 27);
			break;
		case Ghost::GHOSTSTATE::FRIGHTENEND:
			updateDirectionOfGhost(blinky, targetx, targety, false);
			break;
		default:
			break;
		}
	}
	blinky.move();

	// pinky
	bool pIdxPosUpdated = pinky.isIndexPositionUpdated();
	if (pIdxPosUpdated) {
		switch (pinky.getState()) {
		case Ghost::GHOSTSTATE::CHASE:
			switch (pacman.getCurrentDirection()) {
			case Sphere::UP: updateDirectionOfGhost(pinky, targetx - 4, targety); break;
			case Sphere::RIGHT: updateDirectionOfGhost(pinky, targetx, targety + 4); break;
			case Sphere::DOWN: updateDirectionOfGhost(pinky, targetx + 4, targety); break;
			case Sphere::LEFT: updateDirectionOfGhost(pinky, targetx, targety - 4); break;
			default: updateDirectionOfGhost(pinky, targetx, targety); break;
			}
			break;
		case Ghost::GHOSTSTATE::SCATTER:
			updateDirectionOfGhost(pinky, 0, 0);
			break;
		// EATEN의 경우 목표 위치 도달시 currState로 바꿔줌
		case Ghost::GHOSTSTATE::EATEN:
			if (pinky.getXIndex() == 1 && pinky.getYIndex() == 1) {
				pinky.setMTL(pinkyMtl);
				pinky.setState(currState);
				pinky.setChange_state(true);
			}
			updateDirectionOfGhost(pinky, 0, 0);
			break;
		case Ghost::GHOSTSTATE::FRIGHTENEND:
			updateDirectionOfGhost(pinky, targetx, targety, false); 
			break;
		default:
			break;
		}
	}
	pinky.move();

	// inky
	bool iIdxPosUpdated = inky.isIndexPositionUpdated();
	int inkytargetx = targetx, inkytargety = targety;
	if (iIdxPosUpdated) {
		switch (inky.getState()) {
		case Ghost::GHOSTSTATE::CHASE:
			switch (pacman.getCurrentDirection()) {
			case Sphere::UP: inkytargetx -= 2; break;
			case Sphere::RIGHT: inkytargety += 2; break;
			case Sphere::DOWN: inkytargetx += 2; break;
			case Sphere::LEFT: inkytargety -= 2; break;
			default: break;
			}
			targetx = 2 * inkytargetx - blinky.getXIndex();
			targety = 2 * inkytargety - blinky.getYIndex();
			updateDirectionOfGhost(inky, targetx, targety);
			break;
		case Ghost::GHOSTSTATE::SCATTER:
			updateDirectionOfGhost(inky, 30, 27);
			break;
		// EATEN의 경우 목표 위치 도달시 currState로 바꿔줌
		case Ghost::GHOSTSTATE::EATEN:
			if (inky.getXIndex() == 29 && inky.getYIndex() == 26) {
				inky.setMTL(inkyMtl);
				inky.setState(currState);
				inky.setChange_state(true);
			}
			updateDirectionOfGhost(inky, 30, 27);
			break;
		case Ghost::GHOSTSTATE::FRIGHTENEND:
			updateDirectionOfGhost(inky, targetx, targety, false);
			break;
		default:
			break;
		}
	}
	inky.move();

	// clyde
	bool cIdxPosUpdated = clyde.isIndexPositionUpdated();
	if (cIdxPosUpdated) {
		int distance = abs(clyde.getXIndex() - targetx) + abs(clyde.getYIndex() - targety);
		switch (clyde.getState()) {
		case Ghost::GHOSTSTATE::CHASE:
			if (distance > 8) updateDirectionOfGhost(clyde, 30, 0);
			else updateDirectionOfGhost(clyde, targetx, targety);
			break;
		case Ghost::GHOSTSTATE::SCATTER:
			updateDirectionOfGhost(clyde, 30, 0);
			break;
		// EATEN의 경우 목표 위치 도달시 currState로 바꿔줌.
		case Ghost::GHOSTSTATE::EATEN:
			if (clyde.getXIndex() == 29 && clyde.getYIndex() == 1) {
				clyde.setMTL(clydeMtl);
				clyde.setState(currState);
				clyde.setChange_state(true);
			}
			updateDirectionOfGhost(clyde, 30, 0);
			break;
		case Ghost::GHOSTSTATE::FRIGHTENEND:
			updateDirectionOfGhost(clyde, targetx, targety, false);
			break;
		default:
			break;
		}
	}
	clyde.move();
}

GameTimer gameTimer;
FrightenedTimer frightenedTimer;
BlackshownTimer blackshownTimer;
int blinkingIntreval = frightenedTimer.getblinkingInterval();
int blackshownTime = blackshownTimer.getblackshownTime();

int th1 = blinkingIntreval;
int th2 = 1000;
int th3 = 1200;
int th4 = blackshownTime;

int responseTime = gameTimer.getresponseTime();
int GTelapsedTime, FTelapsedTime, BTelapsedTime;

void resetGame() {
	pacman.setCollided(false);
	pacman.setIndexPosition(17, 13);
	pacman.setCurrentDirection(Sphere::NONE);
	pacman.setNextDirection(Sphere::NONE);
	pacman.setVelocity(0.0f, 0.0f, 0.0f);
	pacman.setAlpha(1.0f);

	for (auto* ghost : ghosts) {
		ghost->setCurrentDirection(Sphere::NONE);
		ghost->setNextDirection(Sphere::NONE);
		ghost->setVelocity(0.0f, 0.0f, 0.0f);
		ghost->setIndexPositionUpdated(true);
		ghost->setState(Ghost::GHOSTSTATE::SCATTER);
		ghost->setAlpha(1.0f);
	}

	map.setBox_color(3, 1, 1.0f, 0.8f, 0.6f);
	map.setBox_color(3, 26, 1.0f, 0.8f, 0.6f);
	map.setBox_color(23, 1, 1.0f, 0.8f, 0.6f);
	map.setBox_color(23, 26, 1.0f, 0.8f, 0.6f);

	th1 = blinkingIntreval;
	th2 = 1000;
	th3 = 1200;
	th4 = blackshownTime;

	initializeGhost(blinky, 1, 26, blinkyMtl);
	initializeGhost(pinky, 1, 1, pinkyMtl);
	initializeGhost(inky, 29, 26, inkyMtl);
	initializeGhost(clyde, 29, 1, clydeMtl);
	glutPostRedisplay();
}


void idle() {
	float spf = 1000.0f / FPS;
	eTime = glutGet(GLUT_ELAPSED_TIME);

	if (eTime - sTime > spf) {
		int deltaTime = eTime - sTime;
		sTime = eTime;

		//-------------------------------(gameTimer)-------------------------------
		gameTimer.update(deltaTime);

		// READY: 처음시작과 RESPONSE 이후 실행
		if (gameTimer.getState() == Timer::STATE::READY) {
			bool isplay = gameTimer.checkchange(Timer::SCATTER, gameTimer.getreadyTime());
			if (!gameTimer.getReadyInitialized()) {
				gameTimer.setReadyInitialized(true);
				resetGame();
			}
			return;
		}
		// RESPONSE: 부딪힌 채로 일정 시간 지나면 READY로
		else if (gameTimer.getState() == Timer::STATE::RESPONSE) {
			bool isready = gameTimer.checkchange(Timer::READY, gameTimer.getresponseTime());
			// SMOOTH하게 돌아가기
			GTelapsedTime = gameTimer.getelapsedTime();
			if (th2 < GTelapsedTime && GTelapsedTime < th3) {
				for (auto* g : ghosts) {
					g->setAlpha(0.0f);
				}
			}
			else if (GTelapsedTime > th3) {
				pacman.setAlpha(1.0f - 1.5 * (float)((GTelapsedTime - th3) * (GTelapsedTime - th3)) / (float)((responseTime - th3) * (responseTime - th3)));
			}
			if (isready) {
				gameTimer.setReadyInitialized(false);
				blackshownTimer.initialize(Timer::STATE::READY, 0);
				return;
			}
			// ----------------------------(blackshownTimer)----------------------------
			blackshownTimer.update(deltaTime);
			BTelapsedTime = blackshownTimer.getelapsedTime();
			if (BTelapsedTime > th4) {
				th4 += blackshownTime;
				bool isBlackShown = blackshownTimer.getState() == Timer::STATE::BLACKSHOWN;
				blackshownTimer.setState(isBlackShown ? Timer::STATE::NON_WORKING : Timer::STATE::BLACKSHOWN);
				float r = isBlackShown ? 1.0f : 0.0f; float g = isBlackShown ? 0.8f : 0.0f;	float b = isBlackShown ? 0.6f : 0.0f;
				map.setBox_color(3, 1, r, g, b);
				map.setBox_color(3, 26, r, g, b);
				map.setBox_color(23, 1, r, g, b);
				map.setBox_color(23, 26, r, g, b);
			}
			// ----------------------------(blackshownTimer)----------------------------
			glutPostRedisplay();
			return;
		}
		// PLAYING
		// 상태 변경 check (Scatter <-> Chase)
		else if (gameTimer.getState() == Timer::STATE::CHASE) {
			// CHASE에서 SCATTER로 넘어갔는가?
			if (gameTimer.checkchange(Timer::SCATTER, gameTimer.getchaseTime())) {
				currState = Ghost::GHOSTSTATE::SCATTER;
				// 고스트 상태 변경(START) (Scatter -> Chase)
				for (auto* ghost : ghosts) {
					if (ghost->getState() == Ghost::GHOSTSTATE::CHASE)
						ghost->setState(currState);
				}
			}
			else
				currState = Ghost::GHOSTSTATE::CHASE;

			if (frightenedTimer.getState() != Timer::STATE::NON_WORKING)
				frightenedTimer.update(deltaTime);
		}
		else if (gameTimer.getState() == Timer::STATE::SCATTER) {
			// SCATTER에서 CHASE로 넘어갔는가?
			if (gameTimer.checkchange(Timer::CHASE, gameTimer.getscatterTime())) {
				currState = Ghost::GHOSTSTATE::CHASE;
				// 고스트 상태 변경(START) (Scatter <- Chase)
				for (auto* ghost : ghosts) {
					if (ghost->getState() == Ghost::GHOSTSTATE::SCATTER)
						ghost->setState(currState);
				}
			}
			else
				currState = Ghost::GHOSTSTATE::SCATTER;

			if (frightenedTimer.getState() != Timer::STATE::NON_WORKING)
				frightenedTimer.update(deltaTime);
		}

		// -------------------------------(gameTimer)-------------------------------

		// ----------------------------(frightenedTimer)----------------------------
		switch (frightenedTimer.getState()) {
		case Timer::STATE::NON_WORKING:
			break;
		case Timer::STATE::FRIGHTENED:
			if (frightenedTimer.checkchange(Timer::TRANSITION, frightenedTimer.getfrightenedTime())) {}
			break;
		case Timer::STATE::TRANSITION:
			if (frightenedTimer.checkchange(Timer::NON_WORKING, frightenedTimer.gettransitionTime())) {
				// 고스트 상태 변경(START) (Frightened -> currState)
				for (auto* ghost : ghosts) {
					if (ghost->getState() == Ghost::GHOSTSTATE::FRIGHTENEND) {
						ghost->setState(currState);
						ghost->setAlpha(1.0f);
					}	
					blinky.setMTL(blinkyMtl);
					pinky.setMTL(pinkyMtl);
					inky.setMTL(inkyMtl);
					clyde.setMTL(clydeMtl);
				}
			}
			else {
				FTelapsedTime = frightenedTimer.getelapsedTime();
				if (FTelapsedTime > th1) {
					th1 += blinkingIntreval;
					for (auto* ghost : ghosts) {
						if (ghost->getState() == Ghost::GHOSTSTATE::FRIGHTENEND) {
							ghost->setAlpha(ghost->getAlpha() == 1.0f ? 0.4f : 1.0f);
						}
					}
				}
			}
			break;
		default:
			break;
		}
		// ----------------------------(frightenedTimer)----------------------------
		
		// ----------------------------(blackshownTimer)----------------------------
		blackshownTimer.update(deltaTime);
		BTelapsedTime = blackshownTimer.getelapsedTime();
		if (BTelapsedTime > th4) {
			th4 += blackshownTime;
			bool isBlackShown = blackshownTimer.getState() == Timer::STATE::BLACKSHOWN;
			blackshownTimer.setState(isBlackShown ? Timer::STATE::NON_WORKING : Timer::STATE::BLACKSHOWN);
			float r = isBlackShown ? 1.0f : 0.0f; float g = isBlackShown ? 0.8f : 0.0f;	float b = isBlackShown ? 0.6f : 0.0f;
			map.setBox_color(3, 1, r, g, b);
			map.setBox_color(3, 26, r, g, b);
			map.setBox_color(23, 1, r, g, b);
			map.setBox_color(23, 26, r, g, b);
		}
		// ----------------------------(blackshownTimer)----------------------------

		// 큰 dot 먹었을 때(START) - chase와 scatter인 ghost만 모두 frightenend
		int targetx = pacman.getXIndex();
		int targety = pacman.getYIndex();
		if (map.getPoint_type(targetx, targety) == Block::POINT_TYPE::BIG) {
			frightenedTimer.initialize(Timer::STATE::FRIGHTENED, 0);
			th1 = blinkingIntreval;
			for (auto* ghost : ghosts) {
				if (ghost->getState() == Ghost::GHOSTSTATE::CHASE || ghost->getState() == Ghost::GHOSTSTATE::SCATTER) {
					ghost->setMTL(frightenedMtl);
					ghost->setState(Ghost::GHOSTSTATE::FRIGHTENEND);
					ghost->setChange_state(true);
				}
			}
		}
		map.setPoint_type(targetx, targety, Block::POINT_TYPE::NOPT);
		// 큰 dot 먹었을 때(END)


		updatePacMan();
		updateGhost();


		// 충돌 검사(START) - 한번에 여러명 충돌할 우연의 우연의 우연의... 까지 고려하려면 for문 써야할 듯
		for (auto* ghost : ghosts) {
			colHandler(pacman, *ghost);
			if (!pacman.getCollided()) {
				continue;
			}
			pacman.setCollided(false); // 충돌 초기화
			switch (ghost->getState()) {
			case Ghost::GHOSTSTATE::SCATTER:
			case Ghost::GHOSTSTATE::CHASE:
				// RESPONSE 상태로 진입
				gameTimer.initialize(Timer::STATE::RESPONSE, 0);
				break;

			case Ghost::GHOSTSTATE::FRIGHTENEND:
				// ghost를 EATEN 상태로 변경
				ghost->setState(Ghost::GHOSTSTATE::EATEN);
				ghost->setChange_state(true);
				ghost->setMTL(eatenMtl);
				ghost->setAlpha(1.0f);
				break;

			default:
				break;
			}
		}
		//충돌검사(END);

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

	// Enable blending for transparent objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw pacman 
	pacman.draw();
	// Draw ghosts
	for (auto* ghost : ghosts) {
		if(ghost->getAlpha()>0)
		ghost->draw();
	}

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(light.getID());

	glutSwapBuffers();
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
	glutSpecialFunc(specialKeyDown);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
