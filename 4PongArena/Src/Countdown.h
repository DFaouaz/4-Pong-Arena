#pragma once
#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <UserComponent.h>
#include <UIElement.h>

#include <chrono>

class UILayout;
class SoundEmitter;

class Countdown : public UserComponent
{
private:
	
	void pauseGame();
	void startGame();

	float time;

	bool started, charged,paused;

	std::chrono::steady_clock::time_point last;

	UIElement text;

	SoundEmitter* soundEmitter;
	std::string previousCount;

public:
	Countdown(GameObject* gameObject);
	virtual ~Countdown();
	virtual void start();
	virtual void preUpdate(float deltaTime);
	virtual void handleData(ComponentData* data);
};

#endif