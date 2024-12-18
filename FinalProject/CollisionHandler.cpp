#include "CollisionHandler.h"
#include "CollisionDetector.h"
#include <iostream>
#include "Music.h"
#include "Timer.h"
#include <string>

using namespace std;

// MusicManager 객체를 extern으로 선언
extern MusicManager musicManager;
extern GhoststopTimer ghoststopTimer;

void CollisionHandler::operator()(PacMan& pacman, const Map& map) {
    CollisionDetector colDetector;
    if (!colDetector(pacman, map)) {
        pacman.setCurrentDirection(Sphere::DIRECTION::NONE);
        pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
    }
}

void CollisionHandler::operator()(PacMan& pacman, Ghost& ghost) {
    CollisionDetector colDetector;
    if (ghost.getState() != Ghost::GHOSTSTATE::EATEN) {
        if (colDetector(pacman, ghost)) {
            if (ghoststopTimer.getState() == Timer::NON_WORKING) {
                pacman.setCollided(true);
                ghost.setCollided(true);
                if (ghost.getState() == Ghost::FRIGHTENEND) {
                    musicManager.playMusic("eat_ghost");
                }
                else {
                    pacman.setLife(pacman.getLife() - 1);
                }
            }
        }
    }
}
