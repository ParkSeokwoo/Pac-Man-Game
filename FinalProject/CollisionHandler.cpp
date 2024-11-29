#include "CollisionHandler.h"
#include "CollisionDetector.h"
#include <iostream>
#include "Music.h"
#include <string>
using namespace std;

extern wstring pacman_move_wav;

std::string file3 = "dead.wav";
std::string file5 = "pacman_eatghost.wav";

wstring dead_wav = std::wstring(file3.begin(), file3.end());
wstring pacman_eatghost_wav = std::wstring(file5.begin(), file5.end());


//true는 통과가능해
void CollisionHandler::operator()(PacMan& pacman, const Map& map) {
	CollisionDetector colDetector;
	if (!colDetector(pacman, map)) {
		pacman.setCurrentDirection(Sphere::DIRECTION::NONE);
		pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	}
}

// true는 충돌이야
void CollisionHandler::operator()(PacMan& pacman, Ghost& ghost) {
	CollisionDetector colDetector;
	if (ghost.getState() != Ghost::GHOSTSTATE::EATEN) {
		if (colDetector(pacman, ghost)) {
			if (ghost.getState() != Ghost::GHOSTSTATE::FRIGHTENEND) {
				stopBGM();
				pacman.setLife(pacman.getLife() - 1);
				playBGM(dead_wav, false);
			}
			else if (ghost.getState() == Ghost::FRIGHTENEND) {
				stopBGM();
				playBGM(pacman_eatghost_wav, false);
			}
			pacman.setCollided(true);
		}
	}
}