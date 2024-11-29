#include "CollisionHandler.h"
#include "CollisionDetector.h"
#include <iostream>
#include "Music.h"
using namespace std;
extern wstring dead_wav, pacman_eatghost_wav;

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