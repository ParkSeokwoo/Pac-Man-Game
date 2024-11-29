#include "Music.h"
#include <iostream>
using namespace std;
void playBGM(const std::wstring& wfile, bool loop) {
    if (isMusicPlaying) {
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
