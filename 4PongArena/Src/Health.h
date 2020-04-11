#pragma once
#ifndef HEALTH_H
#define HEALTH_H

#include <UserComponent.h>

class Health : public UserComponent
{
public:
	Health(GameObject* gameObject);
	virtual ~Health();

	virtual void start();
	virtual void handleData(ComponentData* data);
	virtual void onObjectEnter(GameObject* other);

	int getHealth();
	int getMaxHealth();

	void setHealth(int health);
	void receiveDamage(int damage);

	bool isAlive();

private:
	int maxHealth;
	int health;

	bool alive;
};

#endif