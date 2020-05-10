﻿#include "GameManager.h"
#include <Timer.h>
#include <ComponentRegister.h>

#include <SoundEmitter.h>
#include <GameObject.h>


REGISTER_FACTORY(GameManager);

GameManager* GameManager::instance = nullptr;

GameManager::GameManager() : UserComponent(nullptr)
{

}

GameManager::GameManager(GameObject* gameObject) : UserComponent(gameObject)
{
	if (instance == nullptr)
		instance = this;
	else
		destroy(gameObject);
}

GameManager::~GameManager()
{
	if (instance == this)
		instance = nullptr;
}

GameManager* GameManager::GetInstance()
{
	return instance;
}

void GameManager::start()
{
	dontDestroyOnLoad(gameObject);
}

void GameManager::setPlayersAlive(int players)
{
	this->playersAlive = players;
}

int GameManager::getPlayersAlive() const
{
	return playersAlive;
}

void GameManager::setTotalPlayers(int players)
{
	totalPlayers = players;
}

int GameManager::getTotalPlayers() const
{
	return totalPlayers;
}

void GameManager::setPlayers(std::vector<Player>& players)
{
	this->players = players;
}

std::vector<Player> GameManager::getPlayers() const
{
	return players;
}

void GameManager::setIA(bool IA)
{
	this->IA = IA;
}

bool GameManager::getIA() const
{
	return IA;
}

void GameManager::setHealth(int health)
{
	this->health = health;
}

int GameManager::getHealth() const
{
	return health;
}

void GameManager::setTime(int time)
{
	this->time = time;

}

int GameManager::getTime() const
{
	return time;
}

void GameManager::setInitialTime(int time)
{
	this->initialTime = time;
	this->time = time;
}

int GameManager::getInitialTime() const
{
	return initialTime;
}

void GameManager::setLevel(std::string level)
{
	this->level = level;
	this->lastLevel = level;
}

std::string GameManager::getLevel() const
{
	return level;
}

std::string GameManager::getLastLevel() const
{
	return lastLevel;
}

void GameManager::setSong(std::string song)
{
	this->song = song;
	this->lastSong = song;
}

std::string GameManager::getSong() const
{
	return song;
}

std::string GameManager::getLastSong() const
{
	return lastSong;
}

void GameManager::pause(bool paused)
{
	if (paused)
		Timer::GetInstance()->setTimeScale(0.0f); //Pause the game
	else
		Timer::GetInstance()->setTimeScale(1.0f); //Resume the game

	this->paused = paused;
}

bool GameManager::isPaused()
{
	return paused;
}

Score* GameManager::getScore()
{
	return &scores;
}

void GameManager::playMusic(std::string music)
{
	soundEmitter->playMusic(music);
}

void GameManager::stopMusic()
{
	soundEmitter->stopMusicAll();
}

void GameManager::setCurrentMusic(std::string _currentMusic)
{
	currentMusic = _currentMusic;
}

std::string GameManager::getCurrentMusic()
{
	return currentMusic;
}

void GameManager::registerMusicEmitter(MusicEmitter* mEmitter)
{
	soundEmitter = mEmitter;
}

bool GameManager::musicEmitterReady()
{
	return (soundEmitter != nullptr);
}
