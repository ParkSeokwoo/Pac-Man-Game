#pragma once
#include "Sphere.h"
#include <iostream>
#include <array>
using namespace std;


class GhostRoom {
public:
	GhostRoom();
	void moveGhostToRoom(Ghost & ghost, Ghost::GHOSTSTATE currState);
	void moveGhostToMap(Ghost& ghost, Ghost::GHOSTSTATE currState);

	// i 위치에 있는 ghost 가져오기
	Ghost * getGhost(int i) const;
	// 들어갈 수 있는 인덱스(이미 있으면 그 인덱스)
	int getPossibleIndex(Ghost & g) const;
	// 0 아닌 시간 time update
	void updatetime(int t);
	// index i의 시간을 t로 설정
	void settime(int i, int t);
	// in_time 기준 넘는 index 중 max 큰거(지연 많이 된거) 반환
	int checktime();

	void initialize();

private:
	array<Ghost*, 3>  in_ghosts;
	array<int, 3> in_ghosts_time;
	int in_time;
};