#include "CollisionDetector.h"
#include "Sphere.h"

bool CollisionDetector::operator()(const PacMan& pacman, const Map& map) {
	int XIndex = pacman.getXIndex();
	int YIndex = pacman.getYIndex();

	switch (pacman.getCurrentDirection()) {
	case Sphere::DIRECTION::UP:
		XIndex -= 1;
		break;
	case Sphere::DIRECTION::DOWN:
		XIndex += 1;
		break;
	case Sphere::DIRECTION::RIGHT:
		if (XIndex == 14 && YIndex == 27)
			YIndex = 0;
		else
			YIndex += 1;
		break;
	case Sphere::DIRECTION::LEFT:
		if (XIndex == 14 && YIndex == 0)
			YIndex = 27;
		else
			YIndex -= 1;
		break;
	default:
		break;
	}

	return map.getBlock(XIndex, YIndex).isPassable();
}

bool CollisionDetector::operator()(const PacMan& pacman, const Ghost& ghost) {
	int XIndex = pacman.getXIndex();
	int YIndex = pacman.getYIndex();

	if (XIndex == ghost.getXIndex() && YIndex == ghost.getYIndex())
		return true;
	else
		return false;
}