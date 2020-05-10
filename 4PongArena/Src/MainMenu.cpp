#include "MainMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include <SoundEmitter.h>
#include "GameManager.h"

REGISTER_FACTORY(MainMenu);

bool MainMenu::singlePlayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("ConfigurationMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::multiplayerButtonClick()
{
	SceneManager::GetInstance()->changeScene("ConfigurationMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::optionsButtonClick()
{
	SceneManager::GetInstance()->changeScene("OptionsMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::exitButtonClick()
{
	WindowManager::GetInstance()->closeWindow();
	buttonClick(backSound);
	return false;
}

void MainMenu::initMusic()
{
	if (GameManager::GetInstance()->getSong() != "Menu_loop" && GameManager::GetInstance()->musicEmitterReady()) {
		GameManager::GetInstance()->stopMusic();
		GameManager::GetInstance()->playMusic("Menu_loop");
		GameManager::GetInstance()->setSong("Menu_loop");
		GameManager::GetInstance()->setMusicVolume(0.3);
		musicPlaying = true;
	}
	else musicPlaying = false;
}

MainMenu::MainMenu(GameObject* gameObject) : Menu(gameObject)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->registerEvent("singlePlayerButtonClick", UIEvent("ButtonClicked", [this]() {return singlePlayerButtonClick(); }));
	interfaceSystem->registerEvent("multiplayerButtonClick", UIEvent("ButtonClicked", [this]() {return multiplayerButtonClick(); }));
	interfaceSystem->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", [this]() {return optionsButtonClick(); }));
	interfaceSystem->registerEvent("exitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
}

MainMenu::~MainMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->unregisterEvent("singlePlayerButtonClick");
	interfaceSystem->unregisterEvent("multiplayerButtonClick");
	interfaceSystem->unregisterEvent("optionsButtonClick");
	interfaceSystem->unregisterEvent("exitButtonClick");
}

void MainMenu::start()
{
	Menu::start();
	initMusic();
}

void MainMenu::update(float deltaTime)
{
	if (!musicPlaying) {
		initMusic();
	}
}
