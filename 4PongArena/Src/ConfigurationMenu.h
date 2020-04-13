#pragma once
#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

const int MIN_TIME = 60;
const int MAX_TIME = 300;

const int MIN_HEALTH = 1;
const int MAX_HEALTH = 15;

const int MIN_PLAYERS = 1;

class InputSystem;
class UILayout;

class ConfigurationMenu : public UserComponent
{
private:
	InputSystem* inputSystem;

	std::vector<std::pair<int, UIElement>> slots;
	UILayout* configurationLayout;
	UIElement startButton;

	int filledSlots;
	int health;
	int time;

	std::vector<std::string> levelNames;
	int levelIndex;

	std::vector<std::string> songNames;
	int songIndex;


private:
	void checkInput();

	void fillSlot(int slotIndex, int deviceIndex);
	int isSlotFilled(int index);

	void clearSlot(int index);
	void reorderSlots(int index);

	bool changeHealth(int value);
	bool changeTime(int value);
	bool changeLevel(int value);
	bool changeSong(int value);

	bool startButtonClick();

public:
	ConfigurationMenu(GameObject* gameObject);
	virtual ~ConfigurationMenu();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif