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
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_CREATECOMPRESSEDSAMPLE; // 압축된 샘플로 로드
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

    // 기존 채널 재사용
    if (channels.find(name) != channels.end()) {
        channel = channels[name];
        bool isPlaying = false;
        channel->isPlaying(&isPlaying);

        // 이미 재생 중이고 restart가 false라면 종료
        if (isPlaying) {
            return;
        }
    }

    // 새로운 채널에 재생
    if (system->playSound(sounds[name], nullptr, false, &channel) != FMOD_OK) {
        std::cerr << "Failed to play music: " << name << std::endl;
        return;
    }

    channels[name] = channel; // 채널 저장
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
        if (channel.second) { // 채널이 유효한지 확인
            bool isPlaying = false;
            channel.second->isPlaying(&isPlaying); // 현재 재생 중인지 확인
            if (isPlaying) {
                channel.second->stop(); // 재생 중이면 중지
            }
        }
    }
}

