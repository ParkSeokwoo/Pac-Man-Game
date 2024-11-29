#pragma once

#include <windows.h>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include "Music.h"
#include <functional>

static std::atomic<bool> isMusicPlaying(false);  // 음악 상태 추적
static bool intro_done = false;  // 인트로 음악이 끝났는지 확인하는 변수
static bool frightened_start = false; //frightened 음악 이 끝났는지 확인하는 변수
static bool gameclear_start = false;
static bool gameend_start = false;
static bool gamefail_start = false;

void playBGM(const std::wstring& wfile, bool loop = false, bool overlap = false);

void stopBGM();

void playBGMWithCallback(const std::wstring& wfile, int durationMs, std::function<void()> onFinished);