#include "GhostRoom.h"
#include "Constants.h"
#include "Material.h"

extern Material pacmanMtl, blinkyMtl, pinkyMtl, inkyMtl, clydeMtl, eatenMtl, frightenedMtl;

GhostRoom::GhostRoom() {
	in_ghosts = { nullptr, nullptr, nullptr };
	in_ghosts_time = {-1, -1, -1};
	in_time = 3000;
	for (auto& ghost : in_ghosts) {
		ghost = new Ghost(); 
	}
}

void GhostRoom::moveGhostToRoom(Ghost& ghost, Ghost::GHOSTSTATE currState) {
	
	int i = getPossibleIndex(ghost);
	//cout << i << '\n';
	if (i == 3) {
		ghost.setState(currState);
		ghost.setChange_state(true);
		switch (ghost.getGhostname()) {
		case Ghost::BLINKY: ghost.setMTL(blinkyMtl); break;
		case Ghost::PINKY: ghost.setMTL(pinkyMtl); break;
		case Ghost::INKY: ghost.setMTL(inkyMtl); break;
		case Ghost::CLYDE: ghost.setMTL(clydeMtl); break;
		}
	}
	else {
		if (in_ghosts[i]->getGhostname() != ghost.getGhostname()) {
			in_ghosts[i] = & ghost;
		}
	}
	if (i == 0) {
		if (ghost.getYIndex() == 13){
			if (ghost.getCenter()[0] < LEFT_BOUNDARY + 13.5 * BLOCK_SIZE) {
				ghost.setVelocity(4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}	
			else if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE) {
				ghost.setVelocity(0.0f, -4 * MOVE_SPEED, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());

			}
			else {
				ghost.setisInGhostroom(true);
				ghost.setVelocity(0.0f, 0.0f, 0.0f);
				ghost.setCenter(LEFT_BOUNDARY + 13.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
				in_ghosts_time[i] = 0;
				switch (ghost.getGhostname()) {
				case Ghost::BLINKY: ghost.setMTL(blinkyMtl); break;
				case Ghost::PINKY: ghost.setMTL(pinkyMtl); break;
				case Ghost::INKY: ghost.setMTL(inkyMtl); break;
				case Ghost::CLYDE: ghost.setMTL(clydeMtl); break;
				}
			}
		}
		else if (ghost.getYIndex() == 14) {
			if (ghost.getCenter()[0] >= LEFT_BOUNDARY + 13.5 * BLOCK_SIZE) {
				ghost.setVelocity(-4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[1] >= TOP_BOUNDARY - 14 * BLOCK_SIZE) {
				ghost.setVelocity(0.0f, -4 * MOVE_SPEED, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else {
				ghost.setisInGhostroom(true);
				ghost.setVelocity(0.0f, 0.0f, 0.0f);
				ghost.setCenter(LEFT_BOUNDARY + 13.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
				in_ghosts_time[i] = 0;
				switch (ghost.getGhostname()) {
				case Ghost::BLINKY: ghost.setMTL(blinkyMtl); break;
				case Ghost::PINKY: ghost.setMTL(pinkyMtl); break;
				case Ghost::INKY: ghost.setMTL(inkyMtl); break;
				case Ghost::CLYDE: ghost.setMTL(clydeMtl); break;
				}
			}
		}
	}
	else if (i == 1) {
		if (ghost.getYIndex() == 13) {
			if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE && ghost.getCenter()[0] < LEFT_BOUNDARY + 13.5 * BLOCK_SIZE) {
				ghost.setVelocity(4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE) {
				ghost.setVelocity(0.0f, -4 * MOVE_SPEED, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[0] > LEFT_BOUNDARY + 11.5 * BLOCK_SIZE) {
				ghost.setVelocity(-4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else {
				ghost.setisInGhostroom(true);
				ghost.setVelocity(0.0f, 0.0f, 0.0f);
				ghost.setCenter(LEFT_BOUNDARY + 11.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
				in_ghosts_time[i] = 0;
				switch (ghost.getGhostname()) {
				case Ghost::BLINKY: ghost.setMTL(blinkyMtl); break;
				case Ghost::PINKY: ghost.setMTL(pinkyMtl); break;
				case Ghost::INKY: ghost.setMTL(inkyMtl); break;
				case Ghost::CLYDE: ghost.setMTL(clydeMtl); break;
				}
			}
		}
		else if (ghost.getYIndex() == 14) {
			if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE && ghost.getCenter()[0] > LEFT_BOUNDARY + 13.5 * BLOCK_SIZE) {
				ghost.setVelocity(-4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE) {
				ghost.setVelocity(0.0f, -4 * MOVE_SPEED, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[0] > LEFT_BOUNDARY + 11.5 * BLOCK_SIZE) {
				ghost.setVelocity(-4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else {
				ghost.setisInGhostroom(true);
				ghost.setVelocity(0.0f, 0.0f, 0.0f);
				ghost.setCenter(LEFT_BOUNDARY + 11.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
				in_ghosts_time[i] = 0;
				switch (ghost.getGhostname()) {
				case Ghost::BLINKY: ghost.setMTL(blinkyMtl); break;
				case Ghost::PINKY: ghost.setMTL(pinkyMtl); break;
				case Ghost::INKY: ghost.setMTL(inkyMtl); break;
				case Ghost::CLYDE: ghost.setMTL(clydeMtl); break;
				}
			}
		}
	}
	else if (i == 2) {
		if (ghost.getYIndex() == 13) {
			if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE && ghost.getCenter()[0] < LEFT_BOUNDARY + 13.5 * BLOCK_SIZE) {
				ghost.setVelocity(4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE) {
				ghost.setVelocity(0.0f, -4 * MOVE_SPEED, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[0] < LEFT_BOUNDARY + 15.5 * BLOCK_SIZE) {
				ghost.setVelocity(4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else {
				ghost.setisInGhostroom(true);
				ghost.setVelocity(0.0f, 0.0f, 0.0f);
				ghost.setCenter(LEFT_BOUNDARY + 15.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
				in_ghosts_time[i] = 0;
				switch (ghost.getGhostname()) {
				case Ghost::BLINKY: ghost.setMTL(blinkyMtl); break;
				case Ghost::PINKY: ghost.setMTL(pinkyMtl); break;
				case Ghost::INKY: ghost.setMTL(inkyMtl); break;
				case Ghost::CLYDE: ghost.setMTL(clydeMtl); break;
				}
			}
		}
		else if (ghost.getYIndex() == 14) {
			if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE && ghost.getCenter()[0] > LEFT_BOUNDARY + 13.5 * BLOCK_SIZE) {
				ghost.setVelocity(-4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[1] > TOP_BOUNDARY - 14 * BLOCK_SIZE) {
				ghost.setVelocity(0.0f, -4 * MOVE_SPEED, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else if (ghost.getCenter()[0] < LEFT_BOUNDARY + 15.5 * BLOCK_SIZE) {
				ghost.setVelocity(4 * MOVE_SPEED, 0.0f, 0.0f);
				ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
			}
			else {
				ghost.setisInGhostroom(true);
				ghost.setVelocity(0.0f, 0.0f, 0.0f);
				ghost.setCenter(LEFT_BOUNDARY + 15.5 * BLOCK_SIZE, TOP_BOUNDARY - 14 * BLOCK_SIZE, 0.0f);
				in_ghosts_time[i] = 0;
				switch (ghost.getGhostname()) {
				case Ghost::BLINKY: ghost.setMTL(blinkyMtl); break;
				case Ghost::PINKY: ghost.setMTL(pinkyMtl); break;
				case Ghost::INKY: ghost.setMTL(inkyMtl); break;
				case Ghost::CLYDE: ghost.setMTL(clydeMtl); break;
				}
			}
		}
	}
}

void GhostRoom::moveGhostToMap(Ghost& ghost, Ghost::GHOSTSTATE currState) {
	int i = getPossibleIndex(ghost);
	
	if (i == 0) {
		if (ghost.getCenter()[1] < TOP_BOUNDARY - 11 * BLOCK_SIZE) {
			ghost.setVelocity(0.0f, 0.6f * MOVE_SPEED, 0.0f);
			ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
		}
		else {
			if (ghost.getYIndex() == 13) {
				if (ghost.getCenter()[0] > LEFT_BOUNDARY + 13 * BLOCK_SIZE) {
					ghost.setVelocity(-0.6f * MOVE_SPEED, 0.0f, 0.0f);
					ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
				}
				else {
					ghost.setIndexPosition(11, 13);
					ghost.setIndexPositionUpdated(true);
					// cout << "府悸" << endl;
					ghost.setCurrentDirection(Sphere::NONE);
					ghost.setNextDirection(Sphere::NONE);
					ghost.setVelocity(0.0f, 0.0f, 0.0f);
					ghost.setState(currState);
					ghost.setChange_state(true);
					ghost.setisInGhostroom(false);
					in_ghosts[i] = new Ghost;
					in_ghosts_time[i] = -1;
				}
			}
			else if (ghost.getYIndex() == 14) {
				if (ghost.getCenter()[0] < LEFT_BOUNDARY + 14 * BLOCK_SIZE) {
					ghost.setVelocity(+0.6f * MOVE_SPEED, 0.0f, 0.0f);
					ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
				}
				else {
					ghost.setIndexPosition(11, 14);
					ghost.setIndexPositionUpdated(true);
					// cout << "府悸" << endl;
					ghost.setCurrentDirection(Sphere::NONE);
					ghost.setNextDirection(Sphere::NONE);
					ghost.setVelocity(0.0f, 0.0f, 0.0f);
					ghost.setState(currState);
					ghost.setChange_state(true);
					ghost.setisInGhostroom(false);
					in_ghosts[i] = new Ghost;
					in_ghosts_time[i] = -1;
				}
			}
		}
	}
	else if (i == 1) {
		if (ghost.getCenter()[0] < LEFT_BOUNDARY + 13.5 * BLOCK_SIZE && ghost.getCenter()[1] < TOP_BOUNDARY - 11 * BLOCK_SIZE) {
			ghost.setVelocity(0.6f * MOVE_SPEED, 0.0f, 0.0f);
			ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
		}
		else if (ghost.getCenter()[1] < TOP_BOUNDARY - 11 * BLOCK_SIZE) {
			ghost.setVelocity(0.0f, 0.6f * MOVE_SPEED, 0.0f);
			ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
		}
		else {
			if (ghost.getYIndex() == 13) {
				if (ghost.getCenter()[0] > LEFT_BOUNDARY + 13 * BLOCK_SIZE) {
					ghost.setVelocity(-0.6f * MOVE_SPEED, 0.0f, 0.0f);
					ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
				}
				else {
					ghost.setIndexPosition(11, 13);
					ghost.setIndexPositionUpdated(true);
					// cout << "府悸" << endl;
					ghost.setCurrentDirection(Sphere::NONE);
					ghost.setNextDirection(Sphere::NONE);
					ghost.setVelocity(0.0f, 0.0f, 0.0f);
					ghost.setState(currState);
					ghost.setChange_state(true);
					ghost.setisInGhostroom(false);
					in_ghosts[i] = new Ghost;
					in_ghosts_time[i] = -1;
				}
			}
			else if (ghost.getYIndex() == 14) {
				if (ghost.getCenter()[0] < LEFT_BOUNDARY + 14 * BLOCK_SIZE) {
					ghost.setVelocity(+0.6f * MOVE_SPEED, 0.0f, 0.0f);
					ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
				}
				else {
					ghost.setIndexPosition(11, 14);
					ghost.setIndexPositionUpdated(true);
					// cout << "府悸" << endl;
					ghost.setCurrentDirection(Sphere::NONE);
					ghost.setNextDirection(Sphere::NONE);
					ghost.setVelocity(0.0f, 0.0f, 0.0f);
					ghost.setState(currState);
					ghost.setChange_state(true);
					ghost.setisInGhostroom(false);
					in_ghosts[i] = new Ghost;
					in_ghosts_time[i] = -1;
				}
			}
		}
	}
	else if (i == 2) {
		if (ghost.getCenter()[0] > LEFT_BOUNDARY + 13.5 * BLOCK_SIZE && ghost.getCenter()[1] < TOP_BOUNDARY - 11 * BLOCK_SIZE)  {
			ghost.setVelocity(-0.6f * MOVE_SPEED, 0.0f, 0.0f);
			ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
		}
		else if (ghost.getCenter()[1] < TOP_BOUNDARY - 11 * BLOCK_SIZE) {
			ghost.setVelocity(0.0f, 0.6f * MOVE_SPEED, 0.0f);
			ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
		}
		else {
			if (ghost.getYIndex() == 13) {
				if (ghost.getCenter()[0] > LEFT_BOUNDARY + 13 * BLOCK_SIZE) {
					ghost.setVelocity(-0.6f * MOVE_SPEED, 0.0f, 0.0f);
					ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
				}
				else {
					ghost.setIndexPosition(11, 13);
					ghost.setIndexPositionUpdated(true);
					// cout << "府悸" << endl;
					ghost.setCurrentDirection(Sphere::NONE);
					ghost.setNextDirection(Sphere::NONE);
					ghost.setVelocity(0.0f, 0.0f, 0.0f);
					ghost.setState(currState);
					ghost.setChange_state(true);
					ghost.setisInGhostroom(false);
					in_ghosts[i] = new Ghost;
					in_ghosts_time[i] = -1;
				}
			}
			else if (ghost.getYIndex() == 14) {
				if (ghost.getCenter()[0] < LEFT_BOUNDARY + 14 * BLOCK_SIZE) {
					ghost.setVelocity(+0.6f * MOVE_SPEED, 0.0f, 0.0f);
					ghost.setCenter(ghost.getCenter() + ghost.getVelocity());
				}
				else {
					ghost.setIndexPosition(11, 14);
					ghost.setIndexPositionUpdated(true);
					// cout << "府悸" << endl;
					ghost.setCurrentDirection(Sphere::NONE);
					ghost.setNextDirection(Sphere::NONE);
					ghost.setVelocity(0.0f, 0.0f, 0.0f);
					ghost.setState(currState);
					ghost.setChange_state(true);
					ghost.setisInGhostroom(false);
					in_ghosts[i] = new Ghost;
					in_ghosts_time[i] = -1;
				}
			}
		}
	}
}

Ghost* GhostRoom::getGhost(int i) const {
	return in_ghosts[i];
}

int GhostRoom::getPossibleIndex(Ghost& g) const {
	for (int i = 0; i < 3; i++) {
		if (in_ghosts[i]->getGhostname() == g.getGhostname()) {
			return i;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (in_ghosts[i]->getGhostname() == Ghost::NONE) {
			return i;
		}
	}
	return 3;
}

void GhostRoom::updatetime(int t) {
	for (auto& i : in_ghosts_time) {
		if (i != -1) {
			i += t;
			// cout << i << ' ';
		}	
	}
	//cout << '\n';
}

void GhostRoom::settime(int i, int t) {
	in_ghosts_time[i] = t;
}

int GhostRoom::checktime() {
	int max_t = 0;
	int idx = -1;
	for (int i = 0; i < 3; i++) {
		if (in_ghosts_time[i] > in_time && in_ghosts_time[i] > max_t) {
			max_t = in_ghosts_time[i];
			idx = i;
		}
	}
	return idx;
}

void GhostRoom::initialize() {
	for (auto& ghost : in_ghosts) {
		ghost = new Ghost(); 
	}
	in_ghosts_time.fill(-1);
}