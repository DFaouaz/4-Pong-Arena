#pragma once
#ifndef IA_PADDLE_H
#define IA_PADDLE_H

#include <UserComponent.h>

class Movement;

class IAPaddle : public UserComponent
{
private:
	enum class State { CHOOSE_TARGET, MOVE };
	State currentState;
	GameObject* targetBall;
	Movement* movement;
	float timeToChange;
	float timerToChange;

	std::vector<GameObject*> balls;

public:
	IAPaddle(GameObject* gameObject);
	~IAPaddle();

	virtual void start();
	virtual void update(float deltaTime);

private:
	void processChooseTargetState();
	void processMoveState();

	void takeDecision();
	bool canReachToTarget();
	bool isBallBehind(const Vector3& ballPosition);
	bool isBallHeadingToMe(const Vector3& ballDirection);
};

#endif