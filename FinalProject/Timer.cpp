#include "Timer.h"

Timer::Timer() : elapsedTime(0), state(Timer::STATE::READY) {}

void Timer::update(int deltaTime) {
    elapsedTime += deltaTime;
}

void Timer::setState(STATE s) {
    state = s;
}

bool Timer::checkchange(Timer::STATE a, int t) {
    if (elapsedTime > t) {
        elapsedTime = 0;
        state = a;
        return true;
    }
    else {
        return false;
    }
    //}
}

int Timer::getelapsedTime() const {
    return elapsedTime;
}

Timer::STATE Timer::getState() const {
    return state;
}


void Timer::initialize(STATE s, int t) {
    state = s;
    elapsedTime = t;
}


GameTimer::GameTimer() : readyTime(1000), chaseTime(20000), scatterTime(7000), responseTime(3000), ReadyInitialized(false) {}

void GameTimer::setReadyInitialized(bool s) {
    ReadyInitialized = s;
}
int GameTimer::getreadyTime() const {
    return readyTime;
}
int GameTimer::getchaseTime() const {
    return chaseTime;
}
int GameTimer::getscatterTime() const {
    return scatterTime;
}
int GameTimer::getresponseTime() const {
    return responseTime;
}
bool GameTimer::getReadyInitialized() const {
    return ReadyInitialized;
}

FrightenedTimer::FrightenedTimer() : frightenedTime(5000), transitionTime(3000), blinkingInterval(200) {}

int FrightenedTimer::getfrightenedTime() const {
    return frightenedTime;
}
int FrightenedTimer::gettransitionTime() const {
    return transitionTime;
}
int FrightenedTimer::getblinkingInterval() const {
    return blinkingInterval;
}

BlackshownTimer::BlackshownTimer() : blackshownTime(200) {}

int BlackshownTimer::getblackshownTime() const {
    return blackshownTime;
}