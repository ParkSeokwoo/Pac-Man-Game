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

	// i ��ġ�� �ִ� ghost ��������
	Ghost * getGhost(int i) const;
	// �� �� �ִ� �ε���(�̹� ������ �� �ε���)
	int getPossibleIndex(Ghost & g) const;
	// 0 �ƴ� �ð� time update
	void updatetime(int t);
	// index i�� �ð��� t�� ����
	void settime(int i, int t);
	// in_time ���� �Ѵ� index �� max ū��(���� ���� �Ȱ�) ��ȯ
	int checktime();

	void initialize();

private:
	array<Ghost*, 3>  in_ghosts;
	array<int, 3> in_ghosts_time;
	int in_time;
};