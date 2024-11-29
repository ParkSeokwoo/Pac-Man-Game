#include "Music.h"
#include <iostream>
using namespace std;
void playBGM(const std::wstring& wfile, bool loop, bool overlap) {
    if (!overlap && isMusicPlaying) {
        //cout << 2 << '\n';
        return;  // �̹� ������ ��� ���̸� ���� �������� ����
    }
  
    // SND_ASYNC�� �񵿱� ���, loop�� true�̸� SND_LOOP�� �߰��Ͽ� �ݺ� ���
    DWORD flags = SND_FILENAME | SND_ASYNC;
    if (loop) {
        flags |= SND_LOOP;  // �ݺ� ���
    }

    // ���� ���
    PlaySound(wfile.c_str(), NULL, flags);

    // ���� ��� ���¸� true�� ����
    isMusicPlaying = true;
}

void stopBGM() {
    PlaySound(NULL, 0, 0);  // ���� ����
    isMusicPlaying = false;  // ���� ��� ���¸� false�� ����
}

void playBGMWithCallback(const std::wstring& wfile, int durationMs, std::function<void()> onFinished) {
    // �񵿱� ���� ���
    playBGM(wfile, false, false);

    // ������� ��� �ð���ŭ ��ٸ� �� �ݹ� ȣ��
    std::thread([durationMs, onFinished]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
        isMusicPlaying = false;  // ��� �Ϸ�� ����
        if (onFinished) {
            onFinished();  // �ݹ� ȣ��
        }
        }).detach();  // ���� ������ ���������� ����
}