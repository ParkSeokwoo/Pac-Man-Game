#include "Music.h"
#include <iostream>
using namespace std;
void playBGM(const std::wstring& wfile, bool loop, bool overlap) {
    if (!overlap && isMusicPlaying) {
        //cout << 2 << '\n';
        return;  // 이미 음악이 재생 중이면 새로 시작하지 않음
    }
  
    // SND_ASYNC로 비동기 재생, loop가 true이면 SND_LOOP를 추가하여 반복 재생
    DWORD flags = SND_FILENAME | SND_ASYNC;
    if (loop) {
        flags |= SND_LOOP;  // 반복 재생
    }

    // 음악 재생
    PlaySound(wfile.c_str(), NULL, flags);

    // 음악 재생 상태를 true로 설정
    isMusicPlaying = true;
}

void stopBGM() {
    PlaySound(NULL, 0, 0);  // 음악 정지
    isMusicPlaying = false;  // 음악 재생 상태를 false로 설정
}

void playBGMWithCallback(const std::wstring& wfile, int durationMs, std::function<void()> onFinished) {
    // 비동기 음악 재생
    playBGM(wfile, false, false);

    // 스레드로 재생 시간만큼 기다린 후 콜백 호출
    std::thread([durationMs, onFinished]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
        isMusicPlaying = false;  // 재생 완료로 간주
        if (onFinished) {
            onFinished();  // 콜백 호출
        }
        }).detach();  // 메인 루프와 독립적으로 실행
}