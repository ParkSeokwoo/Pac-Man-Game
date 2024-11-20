#include "CollisionHandler.h"
#include "CollisionDetector.h"
#include <iostream>
using namespace std;


//true�� ���������
void CollisionHandler::operator()(PacMan& pacman, const Map& map) {
	CollisionDetector colDetector;
	if (!colDetector(pacman, map)) {
		pacman.setCurrentDirection(Sphere::DIRECTION::NONE);
		pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	}
}

// true�� �浹�̾�
void CollisionHandler::operator()(PacMan& pacman, Ghost& ghost) {
	CollisionDetector colDetector;
	// ���� 1��
	if (ghost.getState() != Ghost::GHOSTSTATE::EATEN) {
	//
		if (colDetector(pacman, ghost)) {
			pacman.setCollided(true);
		}
	}
}