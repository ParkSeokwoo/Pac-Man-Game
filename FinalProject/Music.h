#ifndef MUSIC_H
#define MUSIC_H

#include <fmod.hpp>
#include <string>
#include <unordered_map>

class MusicManager {
private:
    FMOD::System* system; // FMOD 시스템 객체
    std::unordered_map<std::string, FMOD::Sound*> sounds; // 음악 파일 관리
    std::unordered_map<std::string, FMOD::Channel*> channels; // 채널 관리

public:
    MusicManager();
    ~MusicManager();

    void initialize(); // FMOD 초기화
    void loadMusic(const std::string& name, const std::string& filepath, bool loop = false); // 음악 로드
    void playMusic(const std::string& name, bool restart = false); // 음악 재생
    void stopMusic(const std::string& name); // 음악 정지
    void setVolume(const std::string& name, float volume); // 볼륨 설정 
    void update(); // FMOD 업데이트
    void cleanup(); // 리소스 해제
    void stopAllMusic();
};

#endif // MUSIC_H
