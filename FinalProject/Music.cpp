#include "Music.h"
#include <iostream>

MusicManager::MusicManager() : system(nullptr) {}

MusicManager::~MusicManager() {
    cleanup();
}

void MusicManager::initialize() {
    FMOD::System_Create(&system);
    if (system->init(512, FMOD_INIT_NORMAL, 0) != FMOD_OK) {
        std::cerr << "FMOD initialization failed!" << std::endl;
    }
}

void MusicManager::loadMusic(const std::string& name, const std::string& filepath, bool loop) {
    FMOD::Sound* sound = nullptr;
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_CREATECOMPRESSEDSAMPLE; // ����� ���÷� �ε�
    if (system->createSound(filepath.c_str(), mode, nullptr, &sound) != FMOD_OK) {
        std::cerr << "Failed to load music: " << filepath << std::endl;
        return;
    }
    sounds[name] = sound;
}

void MusicManager::playMusic(const std::string& name, bool restart) {
    if (sounds.find(name) == sounds.end()) {
        std::cerr << "Music not found: " << name << std::endl;
        return;
    }

    FMOD::Channel* channel = nullptr;

    // ���� ä�� ����
    if (channels.find(name) != channels.end()) {
        channel = channels[name];
        bool isPlaying = false;
        channel->isPlaying(&isPlaying);

        // �̹� ��� ���̰� restart�� false��� ����
        if (isPlaying) {
            return;
        }
    }

    // ���ο� ä�ο� ���
    if (system->playSound(sounds[name], nullptr, false, &channel) != FMOD_OK) {
        std::cerr << "Failed to play music: " << name << std::endl;
        return;
    }

    channels[name] = channel; // ä�� ����
}


void MusicManager::stopMusic(const std::string& name) {
    if (channels.find(name) == channels.end()) {
        return;
    }
    channels[name]->stop();
}

void MusicManager::setVolume(const std::string& name, float volume) {
    if (channels.find(name) == channels.end()) {
        std::cerr << "Channel not found for music: " << name << std::endl;
        return;
    }
    channels[name]->setVolume(volume);
}

void MusicManager::update() {
    if (system) {
        system->update();
    }
}

void MusicManager::cleanup() {
    for (auto& elem : sounds) {
        if (elem.second) {
            elem.second->release();
        }
    }
    sounds.clear();

    if (system) {
        system->close();
        system->release();
    }
}

void MusicManager::stopAllMusic() {
    for (auto& channel : channels) {
        if (channel.second) { // ä���� ��ȿ���� Ȯ��
            bool isPlaying = false;
            channel.second->isPlaying(&isPlaying); // ���� ��� ������ Ȯ��
            if (isPlaying) {
                channel.second->stop(); // ��� ���̸� ����
            }
        }
    }
}

