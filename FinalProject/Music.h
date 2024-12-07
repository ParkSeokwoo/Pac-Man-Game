#ifndef MUSIC_H
#define MUSIC_H

#include <fmod.hpp>
#include <string>
#include <unordered_map>

class MusicManager {
private:
    FMOD::System* system; // FMOD �ý��� ��ü
    std::unordered_map<std::string, FMOD::Sound*> sounds; // ���� ���� ����
    std::unordered_map<std::string, FMOD::Channel*> channels; // ä�� ����

public:
    MusicManager();
    ~MusicManager();

    void initialize(); // FMOD �ʱ�ȭ
    void loadMusic(const std::string& name, const std::string& filepath, bool loop = false); // ���� �ε�
    void playMusic(const std::string& name, bool restart = false); // ���� ���
    void stopMusic(const std::string& name); // ���� ����
    void setVolume(const std::string& name, float volume); // ���� ���� 
    void update(); // FMOD ������Ʈ
    void cleanup(); // ���ҽ� ����
    void stopAllMusic();
};

#endif // MUSIC_H
