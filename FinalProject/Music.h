#pragma once

#include <windows.h>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include "Music.h"

static std::atomic<bool> isMusicPlaying(false);  // 음악 상태 추적
static bool intro_done = false;  // 인트로 음악이 끝났는지 확인하는 변수

void playBGM(const std::wstring& wfile, bool loop = false);

void stopBGM();