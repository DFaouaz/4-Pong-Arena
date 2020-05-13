#pragma once
#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

class InputSystem;

class PauseMenu : public UserComponent
{
private:
	InputSystem* inputSystem;

	UIElement pauseMenu;
	UIElement pausePanel;

	UIElement optionsMenu;

	bool optionsButtonClick();
	bool exitButtonClick();

	bool checkControllersInput();

public:
	PauseMenu(GameObject* gameObject);
	virtual ~PauseMenu();

	virtual void start();
	virtual void preUpdate(float deltaTime);

	// opens the Pause Menu and pauses the game
	void setPaused(bool paused);
	bool isVisible();
};

#endif