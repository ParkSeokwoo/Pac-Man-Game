#pragma once

#include <windows.h>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include "Music.h"

static std::atomic<bool> isMusicPlaying(false);  // ���� ���� ����
static bool intro_done = false;  // ��Ʈ�� ������ �������� Ȯ���ϴ� ����

void playBGM(const std::wstring& wfile, bool loop = false);

void stopBGM();