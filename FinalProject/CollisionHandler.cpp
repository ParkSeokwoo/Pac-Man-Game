#include "CollisionHandler.h"
#include "CollisionDetector.h"
#include <iostream>
using namespace std;


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
	// 수정 1번
	if (ghost.getState() != Ghost::GHOSTSTATE::EATEN) {
	//
		if (colDetector(pacman, ghost)) {
			pacman.setCollided(true);
		}
	}
}