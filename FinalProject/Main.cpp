#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "Constants.h"
#include "Light.h"
#include "Sphere.h"
#include "Map.h"
#include "Timer.h"
#include "Texture.h"
#include "GhostRoom.h"
#include "Music.h"

#include "CollisionHandler.h"
#include <GL/freeglut.h>
#include <array>
#include <stb_image.h>

using namespace std;

const int FPS = 60;
int sTime = 0;
int eTime = 0;
int life_base = 2;
int clear_criteria = 180; // �ʱ⿡ 241��
bool hasShownInput = false;

Light light((float)BOUNDARY_X, (float)BOUNDARY_Y, (float)BOUNDARY_X / 2.0f, GL_LIGHT0);

PacMan pacman(BLOCK_SIZE / 2.0f, 20, 20);
Ghost blinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::BLINKY);
Ghost pinky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::PINKY);
Ghost inky(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::INKY);
Ghost clyde(BLOCK_SIZE / 2.0f, 20, 20, Ghost::SCATTER, Ghost::CLYDE);
vector<Ghost *> ghosts = { &blinky, &pinky, &inky, &clyde };

Map map;
GhostRoom ghostroom;
CollisionHandler colHandler;
Material pacmanMtl, blinkyMtl, pinkyMtl, inkyMtl, clydeMtl, eatenMtl, frightenedMtl;
Ghost::GHOSTSTATE currState;
Texture pacman_logo_texture, ready_text_texture, gameover_text_texture, start_text_texture, scoreboard_text_texture, newhighscore_text_texture, scoreboard_texture,  gameend_texture, showinput_texture, pacman_texture, life_texture;
wstring cutscene_wav, intro_wav, pacman_move_wav, gameclear_wav, gamefail_wav, chomp_wav;

MusicManager musicManager;

enum GAME_STATE { INIT, PLAY, GAMEEND };
GAME_STATE gs = INIT;

//score related..
ostringstream oss;
ofstream ofs;

struct Score {
	string name;
	int score;
};

vector<Score> ScoreSaved;
bool ShowingInput = false;
bool ShowScoreBoard = false;
string player_name = "";
int player_score = 0;
bool isnewhigh = false;

vector<Score> get5Scores() {
	ifstream scoretext("\SCORE.txt");
	vector<Score> scores;
	string name;
	int score;

	while (scoretext >> name >> score) {
		scores.push_back({ name, score });
	}
	scoretext.close();
	sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) {
		return a.score > b.score;
		});

	if (scores.size() > 5) scores.resize(5);
	return scores;
}

void savescore(const string& name, int score) {
	ofstream savescore("\SCORE.txt", ios::app);
	if (savescore.is_open()) {
		savescore << name << " " << score << endl;

	}
	savescore.close();
	auto scores = get5Scores();
	if (scores.size() > 1 && score > scores[1].score) {
		musicManager.playMusic("newhighscore"); // �ְ� ���� ���� ���
		isnewhigh = true;
	}
	else if (scores.size() == 1) {
		musicManager.playMusic("newhighscore");
		isnewhigh = true;
	}
	else {
		isnewhigh = false;
	}
}

void initializeMaterial(Material& mtl, const std::array<float, 4>& emission, const std::array<float, 4>& ambient, const std::array<float, 4>& diffuse, const std::array<float, 4>& specular, float shininess) {
	mtl.setEmission(emission[0], emission[1], emission[2], emission[3]);
	mtl.setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
	mtl.setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	mtl.setSpecular(specular[0], specular[1], specular[2], specular[3]);
	mtl.setShininess(shininess);
}

void initializeGhost(Ghost& ghost, int x, int y, Material& mtl, int i = -1) {
    ghost.setIndexPosition(x, y);
    ghost.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    ghost.setMTL(mtl);

	if (i == 0) {
		ghost.setState(Ghost::GHOSTROOM);
		ghost.setCenter(LEFT_BOUNDARY + 13.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
		ghost.setisInGhostroom(true);
		ghostroom.setGhostinIndex(ghost, 0);
	}
	else if (i == 1) {
		ghost.setState(Ghost::GHOSTROOM);
		ghost.setCenter(LEFT_BOUNDARY + 11.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
		ghost.setisInGhostroom(true);
		ghostroom.setGhostinIndex(ghost, 1);
	}
	else if (i == 2) {
		ghost.setState(Ghost::GHOSTROOM);
		ghost.setCenter(LEFT_BOUNDARY + 15.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
		ghost.setisInGhostroom(true);
		ghostroom.setGhostinIndex(ghost, 2);
	}
}

void init_pacman_ghost() {
	// PacMan Initialization
	pacman.setIndexPosition(21, 5);
	pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	pacman.setMTL(pacmanMtl);
	pacman.setAlpha(1.0f);
	pacman.setCurrentDirection(Sphere::NONE);
	pacman.setNextDirection(Sphere::NONE);
	pacman.setLife(life_base);

	// Ghosts Initialization
	initializeGhost(blinky, 15, 5, blinkyMtl);
	initializeGhost(pinky, 13, 5, pinkyMtl);
	initializeGhost(inky, 11, 5, inkyMtl);
	initializeGhost(clyde, 9, 5, clydeMtl);
	for (auto* ghost : ghosts) {
		ghost->setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
		ghost->setAlpha(1.0f);
		ghost->setCurrentDirection(Sphere::NONE);
		ghost->setNextDirection(Sphere::NONE);
		ghost->setIndexPositionUpdated(true);
		ghost->setState(Ghost::CHASE);
	}
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

    // Map Initialization
    map.createMap();

	// Texture Initialization
	pacman_logo_texture.initializeTexture("pacman_logo.png");
	start_text_texture.initializeTexture("start_text.png");
	gameover_text_texture.initializeTexture("gameover_text.png");
	ready_text_texture.initializeTexture("ready_text.png");
	scoreboard_text_texture.initializeTexture("scoreboard_text.png");
	newhighscore_text_texture.initializeTexture("newhighscore_text.png");

	pacman_texture.initializeTexture("pacman.png");
	life_texture.initializeTexture("life.png");
	scoreboard_texture.initializeTexture("scoreboard.png");
	gameend_texture.initializeTexture("GameEnd.png");
	showinput_texture.initializeTexture("ShowInput.png");
	init_pacman_ghost();

	musicManager.initialize();
	musicManager.loadMusic("chomp", "chomp.wav");
	musicManager.loadMusic("cutscene", "cutscene.wav");
	musicManager.loadMusic("dead", "dead.wav");
	musicManager.loadMusic("gamefail", "gamefail.wav");
	musicManager.loadMusic("gameclear", "gameclear.wav");
	musicManager.loadMusic("intro", "intro.wav", true);
	musicManager.loadMusic("eat_ghost", "pacman_eatghost.wav");
	musicManager.loadMusic("pacman_move", "pacman_move.wav", true);
	musicManager.loadMusic("powerup", "powerup.wav", true);
	musicManager.loadMusic("gotoghostroom", "gotoghostroom.wav", true);
	musicManager.loadMusic("newhighscore", "newhighscore.wav", true);
}

void updateDirectionOfPacMan(int targetX = 0, int targetY = 0) {
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

	Block nextblocks[4] = { tBlock, lBlock, bBlock, rBlock };

	int indices[4][2] = { { tIdx[0], tIdx[1] }, { lIdx[0], lIdx[1] }, { bIdx[0], bIdx[1] },	{ rIdx[0], rIdx[1] } };
	Sphere::DIRECTION directions[4] = { Sphere::DIRECTION::UP, Sphere::DIRECTION::LEFT, Sphere::DIRECTION::DOWN, Sphere::DIRECTION::RIGHT };
	Sphere::DIRECTION newDir = Sphere::DIRECTION::NONE;
	Sphere::DIRECTION reverseDir = Sphere::DIRECTION::NONE;
	switch (pacman.getCurrentDirection()) {
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
	if (gs == INIT) {

		float minIdxDist = (float)INT_MAX;
		for (int i = 0; i < 4; i++) {
			if (directions[i] != reverseDir && nextblocks[i].isPassable()) {
				float dist_squre = (indices[i][0] - targetX) * (indices[i][0] - targetX) + (indices[i][1] - targetY) * (indices[i][1] - targetY);
				if (dist_squre < minIdxDist) {
					minIdxDist = dist_squre;
					newDir = directions[i];
				}
			}
		}
		pacman.setNextDirection(newDir);
		pacman.updateDirection();
		switch (pacman.getCurrentDirection()) {
		case Sphere::DIRECTION::LEFT: {
			pacman.setVelocity(-MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::RIGHT: {
			pacman.setVelocity(MOVE_SPEED, 0.0f, 0.0f);
			break;
		}
		case Sphere::DIRECTION::DOWN: {
			pacman.setVelocity(0.0f, -MOVE_SPEED, 0.0f);
			break;
		}
		case Sphere::DIRECTION::UP: {
			pacman.setVelocity(0.0f, MOVE_SPEED, 0.0f);
			break;
		}
		}
		return;
	}

	// update direction - passable�ϸ� ������ �ٲ���
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
	// update velocity - �� ������
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

void updateDirectionOfGhost(Ghost& ghost, float targetX, float targetY, bool shortest = true) {
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

	// state�� ���� ������ reverDir�� �ʿ�����Ƿ� NONE���� ����
	// �װ� �ƴϸ� �ݴ����
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
	// scatter�� chase, eaten�� ��� shortest�� true -> �ּҹ��� ã��
	if (shortest) {
		float minIdxDist = (float)INT_MAX;
		for (int i = 0; i < 4; i++) {
			if (directions[i] != reverseDir && nextblocks[i].isPassable()) {
				float dist_squre = (indices[i][0] - targetX) * (indices[i][0] - targetX) + (indices[i][1] - targetY) * (indices[i][1] - targetY);
				if (dist_squre < minIdxDist) {
					minIdxDist = dist_squre;
					newDir = directions[i];
				}
			}
		}
	}
	// frightened�� ��� shortest�� false -> �ִ���� ã��
	else {
		float maxIdxDist = 0;
		for (int i = 0; i < 4; i++) {
			if (directions[i] != reverseDir && nextblocks[i].isPassable()) {
				float dist_squre = (indices[i][0] - targetX) * (indices[i][0] - targetX) + (indices[i][1] - targetY) * (indices[i][1] - targetY);
				if (dist_squre > maxIdxDist) {
					maxIdxDist = dist_squre;
					newDir = directions[i];
				}
			}
		}
	}

	ghost.setNextDirection(newDir);
	ghost.updateDirection();
	if (gs == INIT) {
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
	// ���� ���¿� �°� �ӵ� ���� -> frightened ������, eaten ������
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
		if (gs == INIT) {
			updateDirectionOfPacMan(14, 14);
			pacman.move();
			return;
		}
		updateDirectionOfPacMan();
		colHandler(pacman, map);
	}
	pacman.move();
}

void updateGhost() {
	// �� ghost ���� STATE ���� direction update �� move
	int targetx = pacman.getXIndex(), targety = pacman.getYIndex();
	if (gs == INIT) {
		for (auto* ghost : ghosts) {
			bool IdxPosUpdated = ghost->isIndexPositionUpdated();
			if (IdxPosUpdated) {
				updateDirectionOfGhost(*ghost, targetx, targety);
			}
			ghost->move();
		}
		return;
	}

	for (auto* ghost : ghosts) {
		if (ghost->getState() == Ghost::GHOSTROOM) {
			if (ghost->getisInGhostroom() == false) {
				ghostroom.moveGhostToRoom(*ghost, currState);
			}
			else if (ghost->getisInGhostroom() == true && (ghost->getVelocity()[0] != 0.0f || ghost->getVelocity()[1] != 0.0f)){
				ghostroom.moveGhostToMap(*ghost, currState);
			}
		}
		else {
			switch (ghost->getGhostname()) {
			case Ghost::BLINKY: {
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
					case Ghost::GHOSTSTATE::EATEN:
						if (blinky.getXIndex() == 11 && (blinky.getYIndex() == 13 || blinky.getYIndex() == 14)) {
							blinky.setState(Ghost::GHOSTROOM);
						}
						updateDirectionOfGhost(blinky, 11, 13.5f);
						break;
					case Ghost::GHOSTSTATE::FRIGHTENEND:
						updateDirectionOfGhost(blinky, targetx, targety, false);
						break;
					default:
						break;
					}
				}
			} break;
			case Ghost::PINKY: {
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
					case Ghost::GHOSTSTATE::EATEN:
						if (pinky.getXIndex() == 11 && (pinky.getYIndex() == 13 || pinky.getYIndex() == 14)) {
							pinky.setState(Ghost::GHOSTROOM);
						}
						updateDirectionOfGhost(pinky, 11, 13.5f);
						break;
					case Ghost::GHOSTSTATE::FRIGHTENEND:
						updateDirectionOfGhost(pinky, targetx, targety, false);
						break;
					default:
						break;
					}
				}
			} break;
			case Ghost::INKY: {
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
						updateDirectionOfGhost(inky, 2 * inkytargetx - blinky.getXIndex(), 2 * inkytargety - blinky.getYIndex());
						break;
					case Ghost::GHOSTSTATE::SCATTER:
						updateDirectionOfGhost(inky, 30, 27);
						break;
					case Ghost::GHOSTSTATE::EATEN:
						if (inky.getXIndex() == 11 && (inky.getYIndex() == 13 || inky.getYIndex() == 14)) {
							inky.setState(Ghost::GHOSTROOM);
						}
						updateDirectionOfGhost(inky, 11, 13.5f);
						break;
						break;
					case Ghost::GHOSTSTATE::FRIGHTENEND:
						updateDirectionOfGhost(inky, targetx, targety, false);
						break;
					default:
						break;
					}
				}
			} break;
			case Ghost::CLYDE: {
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
					case Ghost::GHOSTSTATE::EATEN:
						if (clyde.getXIndex() == 11 && (clyde.getYIndex() == 13 || clyde.getYIndex() == 14)) {
							clyde.setState(Ghost::GHOSTROOM);
						}
						updateDirectionOfGhost(clyde, 14, 13.5f);
						break;
					case Ghost::GHOSTSTATE::FRIGHTENEND:
						updateDirectionOfGhost(clyde, targetx, targety, false);
						break;
					default:
						break;
					}
				}

			} break;
			default:
				break;
			}
			ghost->move();
		}
			
	}


}

void updateGhostRoom(int dt) {
	ghostroom.updatetime(dt);

	int i = ghostroom.checktime();

	if (i == 1 && abs(ghostroom.getGhost(i)->getCenter()[1] - 10) <= 0.24)
		ghostroom.getGhost(i)->setVelocity(+0.6f * MOVE_SPEED, 0.0f, 0.0f);
	else if (i == 2 && abs(ghostroom.getGhost(i)->getCenter()[1] - 10) <= 0.24)
		ghostroom.getGhost(i)->setVelocity(-0.6f * MOVE_SPEED, 0.0f, 0.0f);
	else if (i == 0 && abs(ghostroom.getGhost(i)->getCenter()[1] - 10) <= 0.24)
		ghostroom.getGhost(i)->setVelocity(0.0f, +0.6f * MOVE_SPEED, 0.0f);

	ghostroom.updatelocate();

	int j = 0;
	for (auto ghost : ghosts) {
		if (ghost->getState() == Ghost::EATEN)
			j++;
	}
	if (j == 0) {
		musicManager.stopMusic("gotoghostroom");
	}
}

GameTimer gameTimer;
FrightenedTimer frightenedTimer;
BlackshownTimer blackshownTimer;
GhoststopTimer ghoststopTimer;
int blinkingIntreval = frightenedTimer.getblinkingInterval();
int blackshownTime = blackshownTimer.getblackshownTime();
int ghoststopTime = ghoststopTimer.getghoststopTime();

int th1 = blinkingIntreval;
int th2 = 1000;
int th3 = 1200;
int th4 = blackshownTime;
int th5 = 1000;
int th6 = 1200;
int th7 = 1000;
int th8 = 1200;
int th9 = 200;
int catchghost = 0;

int responseTime = gameTimer.getresponseTime();
int GTelapsedTime, FTelapsedTime, BTelapsedTime;

bool isMusicPlaying = false;
bool intro_done = false;  // ��Ʈ�� ������ �������� Ȯ���ϴ� ����
bool frightened_start = false; //frightened ���� �� �������� Ȯ���ϴ� ����
bool gameclear_start = false;
bool gameend_start = false;
bool gamefail_start = false;
bool response_music_start = false;

void resetGame() {
	pacman.setCollided(false);
	pacman.setIndexPosition(23, 13);
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
	th5 = 500;
	th6 = 1200;
	th7 = 1000;
	th8 = 1200;
	th9 = 200;
	catchghost = 0;

	ghostroom.initialize();
	initializeGhost(blinky, 11, 13, blinkyMtl);
	initializeGhost(pinky, 11, 13, pinkyMtl, 0);
	initializeGhost(inky, 11, 13, inkyMtl, 1);
	initializeGhost(clyde, 11, 13, clydeMtl, 2);
	glutPostRedisplay();

	currState = Ghost::GHOSTSTATE::SCATTER;

	isMusicPlaying = false;  // ���� ���� ����
	intro_done = false;  // ��Ʈ�� ������ �������� Ȯ���ϴ� ����
	frightened_start = false; //frightened ���� �� �������� Ȯ���ϴ� ����
	gameclear_start = false;
	gameend_start = false;
	gamefail_start = false;
}

void renderBitmapString(float x, float y, void* font, const std::string& str) {
	glRasterPos2f(x, y);
	for (char c : str) {
		glutBitmapCharacter(font, c);
	}
}

void idle() {
	float spf = 1000.0f / FPS;
	eTime = glutGet(GLUT_ELAPSED_TIME);

	if (eTime - sTime > spf) {
		int deltaTime = eTime - sTime;
		sTime = eTime;

		if (gs == INIT) {
			updatePacMan();
			updateGhost();
			glutPostRedisplay();
			hasShownInput = false;

			if (!isMusicPlaying) {
				musicManager.playMusic("intro");  // ��Ʈ������ �ݺ����
				isMusicPlaying = true;
			}
			return;
		}
		else if (gs == PLAY) {
			if (!intro_done) {
				musicManager.stopMusic("intro"); // ��Ʈ�� ���� ����
				intro_done = true;  // intro_done�� true�� ����
			}
			if (frightenedTimer.getState() == Timer::STATE::FRIGHTENED) {
				if (!frightened_start) {
					frightened_start = true;
					musicManager.playMusic("powerup");
				} 
			}
			else if (frightenedTimer.getState() == Timer::STATE::NON_WORKING) {
				if (frightened_start) {
					frightened_start = false;
					musicManager.stopMusic("powerup");
					musicManager.playMusic("pacman_move");
				}
			}
			//-------------------------------(gameTimer)-------------------------------
			if (ghoststopTimer.getState() != Timer::STATE::STOPPING) {
				if (frightenedTimer.getState() == Timer::STATE::NON_WORKING) {
					gameTimer.update(deltaTime);
				}
				else {
					if (gameTimer.getState() == Timer::STATE::GAMECLEAR || gameTimer.getState() == Timer::STATE::GAMEOVER)
						gameTimer.update(deltaTime);
				}
			}
			else {
				ghoststopTimer.update(deltaTime);
				bool isnotstop = ghoststopTimer.checkchange(Timer::CHANGING, ghoststopTime);
				return;
			}

			// READY: ó�����۰� RESPONSE ���� ����
			if (gameTimer.getState() == Timer::STATE::READY) {

				//musicPlayer.stopBGM();
				bool isplay = gameTimer.checkchange(Timer::SCATTER, gameTimer.getreadyTime());
				if (!gameTimer.getReadyInitialized()) {
					gameTimer.setReadyInitialized(true);
					resetGame();
				}
				return;
			}
			// RESPONSE: �ε��� ä�� ���� �ð� ������ READY��
			else if (gameTimer.getState() == Timer::STATE::RESPONSE) {
				bool isready = gameTimer.checkchange(Timer::READY, gameTimer.getresponseTime());
				// SMOOTH�ϰ� ���ư���
				GTelapsedTime = gameTimer.getelapsedTime();
				if (th2 < GTelapsedTime && GTelapsedTime < th3) {
					for (auto* g : ghosts) {
						g->setAlpha(0.0f);
					}
				}
				else if (GTelapsedTime >= th3) {
					if (response_music_start == false && (GTelapsedTime - th3)<=deltaTime) {
						musicManager.playMusic("dead");
						response_music_start == true;
					}
					pacman.setAlpha(1.0f - 1.5f * (float)((GTelapsedTime - th3) * (GTelapsedTime - th3)) / (float)((responseTime - th3) * (responseTime - th3)));
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
					map.setObject_color(3, 1, r, g, b);
					map.setObject_color(3, 26, r, g, b);
					map.setObject_color(23, 1, r, g, b);
					map.setObject_color(23, 26, r, g, b);
				}
				// ----------------------------(blackshownTimer)----------------------------
				glutPostRedisplay();
				return;
			}
			// GAMEOVER
			else if (gameTimer.getState() == Timer::STATE::GAMEOVER) {
				if (!gamefail_start) {
					gamefail_start = true;
					musicManager.playMusic("gamefail");
				}
				// SMOOTH�ϰ� ���ư���
				GTelapsedTime = gameTimer.getelapsedTime();
				if (th5 < GTelapsedTime && GTelapsedTime < th6) {
					for (auto* g : ghosts) {
						g->setAlpha(0.0f);
					}
				}
				else if (GTelapsedTime > th6) {
					pacman.setAlpha(1.0f - 1.5f * (float)((GTelapsedTime - th6) * (GTelapsedTime - th6)) / (float)((responseTime - th6) * (responseTime - th6)));
				}
				bool isover = gameTimer.checkchange(Timer::NON_WORKING, gameTimer.getgameoverTime());

				if (isover) {
					gs = GAMEEND;
					map.setState(Map::MAP_STATE::GAMEEND);
					map.createMap();
					return;
				}
				glutPostRedisplay();
				return;
			}
			// GAMECLEAR
			else if (gameTimer.getState() == Timer::STATE::GAMECLEAR) {
				if (!gameclear_start) {
					gameclear_start = true;
					musicManager.playMusic("gameclear");
				}
				// SMOOTH�ϰ� ���ư���
				GTelapsedTime = gameTimer.getelapsedTime();
				if (th7 < GTelapsedTime && GTelapsedTime < th8) {
					for (auto* g : ghosts) {
						g->setAlpha(0.0f);
					}
				}
				else if (GTelapsedTime > th6) {
					if (GTelapsedTime - th6 > th9) {
						th9 += 200;
						float r = (map.getBox_Color(0, 0)[0] == 0.0f) ? 1.0f : 0.0f; float g = (map.getBox_Color(0, 0)[0] == 0.0f) ? 1.0f : 0.0f;	float b = (map.getBox_Color(0, 0)[0] == 0.0f) ? 1.0f : 1.0f;
						for (int i = 0; i < NUM_ROW; ++i) { 
							for (int j = 0; j < NUM_COL; ++j) {
								map.setPoint_type(i, j, Block::POINT_TYPE::NOPT);
								if (map.getBlock(i, j).isPassable() == false) {
									map.setBox_color(i, j, r, g, b);
								}
							}
						}
					}
				}
				bool isend = gameTimer.checkchange(Timer::NON_WORKING, gameTimer.getgameclearTime());

				if (isend) {
					gs = GAMEEND;
					map.setState(Map::MAP_STATE::GAMEEND);
					map.createMap();
					return;
				}
				glutPostRedisplay();
				return;
			}
			// PLAYING
			// ���� ���� check (Scatter <-> Chase)
			if (gameTimer.getState() == Timer::STATE::CHASE) {
				// musicManager.playMusic("pacman_move");
				// CHASE���� SCATTER�� �Ѿ�°�?
				if (gameTimer.checkchange(Timer::SCATTER, gameTimer.getchaseTime())) {
					currState = Ghost::GHOSTSTATE::SCATTER;
					// ��Ʈ ���� ����(START) (Scatter -> Chase)
					for (auto* ghost : ghosts) {
						if (ghost->getState() == Ghost::GHOSTSTATE::CHASE)
							ghost->setState(currState);
					}
				}
				if (frightenedTimer.getState() != Timer::STATE::NON_WORKING)
					frightenedTimer.update(deltaTime);
			}
			else if (gameTimer.getState() == Timer::STATE::SCATTER) {
				musicManager.playMusic("pacman_move");
				if (gameTimer.checkchange(Timer::CHASE, gameTimer.getscatterTime())) {
					currState = Ghost::GHOSTSTATE::CHASE;
					for (auto* ghost : ghosts) {
						if (ghost->getState() == Ghost::GHOSTSTATE::SCATTER)
							ghost->setState(currState);
					}
				}
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
					// ��Ʈ ���� ����(START) (Frightened -> currState)
					for (auto* ghost : ghosts) {
						if (ghost->getState() == Ghost::GHOSTSTATE::FRIGHTENEND) {
							ghost->setState(currState);
							ghost->setAlpha(1.0f);
							switch (ghost->getGhostname()) {
							case Ghost::BLINKY: blinky.setMTL(blinkyMtl); break;
							case Ghost::PINKY: pinky.setMTL(pinkyMtl); break;
							case Ghost::INKY: inky.setMTL(inkyMtl); break;
							case Ghost::CLYDE: clyde.setMTL(clydeMtl); break;
							}
						}
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
				map.setObject_color(3, 1, r, g, b);
				map.setObject_color(3, 26, r, g, b);
				map.setObject_color(23, 1, r, g, b);
				map.setObject_color(23, 26, r, g, b);
			}
			// ----------------------------(blackshownTimer)----------------------------

			// ū dot �Ծ��� ��(START) - chase�� scatter�� ghost�� ��� frightenend
			int targetx = pacman.getXIndex();
			int targety = pacman.getYIndex();
			if (map.getPoint_type(targetx, targety) == Block::POINT_TYPE::BIG) {
				frightened_start = false;
				frightenedTimer.initialize(Timer::STATE::FRIGHTENED, 0);
				th1 = blinkingIntreval;
				for (auto* ghost : ghosts) {
					if (ghost->getState() == Ghost::GHOSTSTATE::CHASE || ghost->getState() == Ghost::GHOSTSTATE::SCATTER) {
						ghost->setMTL(frightenedMtl);
						ghost->setState(Ghost::GHOSTSTATE::FRIGHTENEND);
						ghost->setChange_state(true);
					}
					else if (ghost->getState() == Ghost::GHOSTSTATE::FRIGHTENEND) {
						ghost->setAlpha(1.0f);
					}
					else if (ghost->getState() == Ghost::GHOSTSTATE::GHOSTROOM && ghost->getisInGhostroom() == true) {
						ghost->setMTL(frightenedMtl);
					}
				}
			}
			map.setPoint_type(targetx, targety, Block::POINT_TYPE::NOPT);
			// ū dot �Ծ��� ��(END)

			if (ghoststopTimer.getState() != Timer::STATE::CHANGING) {
				updatePacMan();
				updateGhost();
				updateGhostRoom(deltaTime);
			}


			// �浹 �˻�(START)
			for (auto* ghost : ghosts) {
				colHandler(pacman, *ghost);
				if (!(pacman.getCollided() && ghost->getCollided())) {
					continue;
				}
				// ---- �浹 �߻� ----
				if (ghoststopTimer.getState() == Timer::STATE::NON_WORKING) {
					musicManager.stopMusic("pacman_move");
					if (ghost->getState() != Ghost::GHOSTSTATE::FRIGHTENEND) {
						musicManager.stopAllMusic();
					}
					ghoststopTimer.initialize(Timer::STATE::STOPPING, 0);
					return;
				}
				else if (ghoststopTimer.getState() == Timer::STATE::CHANGING) {
					//cout << "Now Changing" << ghost->getState() << endl;
					// musicManager.stopMusic("pacman_move");
					// musicManager.update();
					pacman.setCollided(false); // �浹 �ʱ�ȭ
					ghost->setCollided(false);
					switch (ghost->getState()) {
					case Ghost::GHOSTSTATE::GHOSTROOM:
					case Ghost::GHOSTSTATE::SCATTER:
					case Ghost::GHOSTSTATE::CHASE: {
						// GAMEOVER or RESPONSE ���·� ����
						if (pacman.getLife() == 0) {
							musicManager.stopAllMusic();
							gameTimer.initialize(Timer::STATE::GAMEOVER, 0);;
						}
						else {
							musicManager.stopAllMusic();
							response_music_start = false;
							gameTimer.initialize(Timer::STATE::RESPONSE, 0);
						}
						break;
					}
					case Ghost::GHOSTSTATE::FRIGHTENEND: {
						// ghost�� EATEN ���·� ����
						ghost->setState(Ghost::GHOSTSTATE::EATEN);
						ghost->setChange_state(true);
						ghost->setMTL(eatenMtl);
						ghost->setAlpha(1.0f);
						catchghost++;
						musicManager.playMusic("gotoghostroom");
					}
						break;
					default:
						break;
					}
					ghoststopTimer.initialize(Timer::NON_WORKING, 0);
				}
			}
			//�浹�˻�(END);
			if (map.isGameClear(clear_criteria)) {
				musicManager.stopAllMusic();
				musicManager.playMusic("gameclear");
				gameTimer.initialize(Timer::STATE::GAMECLEAR, 0);
			}
		}
		else if (gs == GAMEEND) {
			if (!gameend_start) {
				gameend_start = true;
				intro_done = false;
			}
			if (!hasShownInput) {
				ShowingInput = true;
				hasShownInput = true; //ó�� �Է¸� �޵���
			}
			if (ShowingInput == false && ShowScoreBoard == false) {
				musicManager.stopMusic("newhighscore");
				musicManager.playMusic("cutscene");
			}
		}
		musicManager.update();
		glutPostRedisplay();
	}
}

void drawTexture(const Texture& texture, float x, float y, float width, float r) {
	float height = width * r;
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(x - width / 2, y - height /2);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(x - width / 2, y + height / 2);     
	glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width / 2, y + height / 2); 
	glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width / 2, y - height / 2);    
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw texture
	if (gs == INIT) {
		drawTexture(pacman_logo_texture, 0, +BOUNDARY_Y / 1.5f, BOUNDARY_X * 1.6f, 0.9f *pacman_logo_texture.getAspectRatio());
		drawTexture(start_text_texture, 0, -BOUNDARY_Y / 1.7f, BOUNDARY_X * 1.5f, start_text_texture.getAspectRatio());
		drawTexture(scoreboard_text_texture, 0, -BOUNDARY_Y / 1.24f, BOUNDARY_X * 0.4f, scoreboard_text_texture.getAspectRatio());
	}
	if (ShowScoreBoard) {
		drawTexture(scoreboard_texture, 0, 0, BOUNDARY_X * 1.8f, scoreboard_texture.getAspectRatio());
		// ���� 5�� ���� ���
		vector<Score> scores = get5Scores();
		float y = BOUNDARY_Y - 164;
		for (const auto& score : scores) {
			glColor3f(1.0f, 1.0f, 1.0f);
			renderBitmapString(-BOUNDARY_X + 185, y, GLUT_BITMAP_HELVETICA_18, score.name);
			renderBitmapString(-BOUNDARY_X + 256, y, GLUT_BITMAP_HELVETICA_18, to_string(score.score));
			y -= 25;
		}
		if (isnewhigh) {
			drawTexture(newhighscore_text_texture, 0, +BOUNDARY_Y / 1.4f, BOUNDARY_X / 1.5f, newhighscore_text_texture.getAspectRatio());
		}
	}
	if (gs == GAMEEND && ShowingInput) {
		drawTexture(showinput_texture, 0, 0, BOUNDARY_X * 2.0f, showinput_texture.getAspectRatio());
		glColor3f(1.0f, 1.0f, 1.0f);
		renderBitmapString(30, -35, GLUT_BITMAP_TIMES_ROMAN_24, to_string(player_score));
		renderBitmapString(40, -62, GLUT_BITMAP_TIMES_ROMAN_24, player_name);

	}
	// Draw 2D
	if (!(gs == INIT && ShowScoreBoard)) {
		map.draw();
	}
	// life and score
	if (gs == PLAY) {
		//draw life
		drawTexture(life_texture, -BOUNDARY_X + 40, -BOUNDARY_Y + 21, BOUNDARY_X / 4.0f, life_texture.getAspectRatio());
		for (int i = 0; i < pacman.getLife() - 1; ++i) {
			drawTexture(pacman_texture, -BOUNDARY_X + 80 + i * 20, -BOUNDARY_Y + 20, BOUNDARY_X / 10.0f, pacman_texture.getAspectRatio());
		}
		//display score
		int prev_score = player_score;
		if (gameTimer.getState() != Timer::STATE::GAMECLEAR) {
			player_score = -map.getScore() + 2410 + 50 * (catchghost) * (catchghost + 1)/2;
			if (prev_score < player_score) {
				if (player_score != 0) {
					musicManager.playMusic("chomp", true);
				}
			}
		}		
		if (player_score >= 0) {
			glColor3f(1.0f, 1.0f, 1.0f);
			renderBitmapString(-40, BOUNDARY_Y - 20, GLUT_BITMAP_TIMES_ROMAN_24, "Score: " + to_string(player_score));
		}
	}
	// Draw 3D
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	// Enable blending for transparent objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (gs == INIT || gs == PLAY) {
		if (!(gs == INIT && ShowScoreBoard)) {
			// Draw pacman 
			pacman.draw();
			// Draw ghosts
			for (auto* ghost : ghosts) {
				if (ghost->getAlpha() > 0)
					ghost->draw();
			}
		}
	}
	if (gs == PLAY) {
		if (gameTimer.getState() == Timer::STATE::READY) {
			drawTexture(ready_text_texture, -2, -20, 45, ready_text_texture.getAspectRatio());
		}
		else if (gameTimer.getState() == Timer::STATE::GAMEOVER && gameTimer.getelapsedTime() >= responseTime) {
			drawTexture(gameover_text_texture, -5, -20, 50, gameover_text_texture.getAspectRatio());
		}
	}
	if (gs == GAMEEND && ShowingInput == false && ShowScoreBoard == false) {
		drawTexture(gameend_texture, 0, 0, BOUNDARY_X * 2.0f, gameend_texture.getAspectRatio());
	}
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(light.getID());

	glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y) {
	if (tolower(key) == ' ' && gs == INIT && ShowScoreBoard == false) {
		gs = PLAY;
		map.setState(Map::MAP_STATE::ST1);
		map.createMap();
		init_pacman_ghost();

		//Timer Initialization
		gameTimer.initialize(Timer::STATE::READY, 0);
		frightenedTimer.initialize(Timer::STATE::NON_WORKING, 0);
		blackshownTimer.initialize(Timer::STATE::NON_WORKING, 0);
		gameTimer.setReadyInitialized(false);

	}
	else if (tolower(key) == 'r' && gs == GAMEEND && ShowingInput == false && ShowScoreBoard == false) {
		musicManager.stopAllMusic();
		gs = INIT;
		map.setState(Map::MAP_STATE::INIT);
		map.createMap();
		init_pacman_ghost();
		isnewhigh = false;
	}
	else if (key == 27) { // ESC
		musicManager.cleanup();
		exit(0);
	}
	if (gs == INIT || gs == GAMEEND) {
		if (ShowingInput == false) {
			if (ShowScoreBoard == false && tolower(key) == 's') { // Show Scoreboad 
				ShowScoreBoard = true;
			}
			else if (ShowScoreBoard == true && tolower(key) == 's') { // Hide Scoreboard
				ShowScoreBoard = false;
			}
		}
	}
	//saving score..
	if (gs == GAMEEND && ShowingInput) {
		if (key == '\r') { // Enter to save
			savescore(player_name, player_score);
			player_name = "";
			player_score = 0;
			ShowingInput = false;
			ShowScoreBoard = true;
		}
		else if (key == '\b') { // backspace
			if (!player_name.empty()) player_name.pop_back();
		}
		else {
			player_name += key;
		}
	}
}

void specialKeyDown(int key, int x, int y) {
	if (gs == PLAY) {
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
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutCreateWindow("Pac-Man");

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
