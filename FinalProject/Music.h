#pragma once

#include <windows.h>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include "Music.h"
#include <functional>

static std::atomic<bool> isMusicPlaying(false);  // ���� ���� ����
static bool intro_done = false;  // ��Ʈ�� ������ �������� Ȯ���ϴ� ����
static bool frightened_start = false; //frightened ���� �� �������� Ȯ���ϴ� ����
static bool gameclear_start = false;
static bool gameend_start = false;
static bool gamefail_start = false;

void playBGM(const std::wstring& wfile, bool loop = false, bool overlap = false);

void stopBGM();

void playBGMWithCallback(const std::wstring& wfile, int durationMs, std::function<void()> onFinished);