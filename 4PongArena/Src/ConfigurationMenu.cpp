#include "ConfigurationMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <InputSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <MathUtils.h>

#include "GameManager.h"

REGISTER_FACTORY(ConfigurationMenu);

ConfigurationMenu::ConfigurationMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), configurationLayout(nullptr), startButton(NULL), settingsPanel(NULL),
nPlayers(0), health(4), time(60), mode(false), levelBaseType(0), levelForcesType(0), levelObstaclesType(0), songIndex(0)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->registerEvent("-healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(-CHANGE_HEALTH); }));
	interfaceSystem->registerEvent("+healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(+CHANGE_HEALTH); }));

	interfaceSystem->registerEvent("-timeModeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTimeMode(!mode); }));
	interfaceSystem->registerEvent("+timeModeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTimeMode(!mode); }));

	interfaceSystem->registerEvent("-timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(-CHANGE_TIME); }));
	interfaceSystem->registerEvent("+timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(+CHANGE_TIME); }));

	interfaceSystem->registerEvent("-levelBaseButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelBase(-1); }));
	interfaceSystem->registerEvent("+levelBaseButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelBase(+1); }));

	interfaceSystem->registerEvent("-levelObstaclesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelObstacles(-1); }));
	interfaceSystem->registerEvent("+levelObstaclesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelObstacles(+1); }));

	interfaceSystem->registerEvent("-levelForcesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelForces(-1); }));
	interfaceSystem->registerEvent("+levelForcesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelForces(+1); }));

	interfaceSystem->registerEvent("randomizeButtonClick", UIEvent("ButtonClicked", [this]() {return randomizeButtonClick(); }));

	interfaceSystem->registerEvent("-songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(-1); }));
	interfaceSystem->registerEvent("+songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(+1); }));

	interfaceSystem->registerEvent("startButtonClick", UIEvent("ButtonClicked", [this]() {return startButtonClick(); }));
	interfaceSystem->registerEvent("settingsButtonClick", UIEvent("ButtonClicked", [this]() {return settingsButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));

	interfaceSystem->registerEvent("slot1ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(0, "Slot1"); }));
	interfaceSystem->registerEvent("slot2ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(1, "Slot2"); }));
	interfaceSystem->registerEvent("slot3ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(2, "Slot3"); }));
	interfaceSystem->registerEvent("slot4ButtonClick", UIEvent("ButtonClicked", [this]() {return slotButtonClick(3, "Slot4"); }));

	timeModes = { "Time", "Infinite" };
	songNames = { "Canci�n 1", "Canci�n 2", "Canci�n 3" };
}

ConfigurationMenu::~ConfigurationMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->unregisterEvent("-healthButtonClick");
	interfaceSystem->unregisterEvent("+healthButtonClick");

	interfaceSystem->unregisterEvent("-timeModeButtonClick");
	interfaceSystem->unregisterEvent("+timeModeButtonClick");

	interfaceSystem->unregisterEvent("-timeButtonClick");
	interfaceSystem->unregisterEvent("+timeButtonClick");

	interfaceSystem->unregisterEvent("-levelBaseButtonClick");
	interfaceSystem->unregisterEvent("+levelBaseButtonClick");

	interfaceSystem->unregisterEvent("-levelObstaclesButtonClick");
	interfaceSystem->unregisterEvent("+levelObstaclesButtonClick");

	interfaceSystem->unregisterEvent("-levelForcesButtonClick");
	interfaceSystem->unregisterEvent("+levelForcesButtonClick");

	interfaceSystem->unregisterEvent("randomizeButtonClick");

	interfaceSystem->unregisterEvent("-songButtonClick");
	interfaceSystem->unregisterEvent("+songButtonClick");

	interfaceSystem->unregisterEvent("startButtonClick");
	interfaceSystem->unregisterEvent("settingsButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");

	interfaceSystem->unregisterEvent("slot1ButtonClick");
	interfaceSystem->unregisterEvent("slot2ButtonClick");
	interfaceSystem->unregisterEvent("slot3ButtonClick");
	interfaceSystem->unregisterEvent("slot4ButtonClick");
}

void ConfigurationMenu::start()
{
	inputSystem = InputSystem::GetInstance();

	GameManager* gameManager = GameManager::GetInstance();
	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	if (mainCamera != nullptr)
	{
		configurationLayout = mainCamera->getComponent<UILayout>();
		settingsPanel = mainCamera->getChildren()[0]->getComponent<UILayout>()->getRoot();
	}

	if (configurationLayout != nullptr)
		startButton = configurationLayout->getRoot().getChild("StartButton");

	slots = std::vector<std::pair<int, UIElement>>(4, { -1, NULL });
	std::vector<int> indexes = gameManager->getPlayerIndexes();

	for (int i = 0; i < 4; i++)
	{
		if (configurationLayout != nullptr)
		{
			slots[i] = { indexes[i] , configurationLayout->getRoot().getChild("Slot" + std::to_string(i + 1)) };

			if (indexes[i] != -1)
			{
				if (indexes[i] == 9)
					slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Text").setText("IA");
				else
					slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Text").setText("Player " + std::to_string(i + 1));
			}
		}
	}

	nPlayers = gameManager->getInitialPlayers();

	if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
		startButton.setVisible(true);

	health = gameManager->getHealth();
	mode = gameManager->getTimeMode();
	time = gameManager->getInitialTime();

	changeHealth(0);
	changeTimeMode(mode);
	changeTime(0);

	levelBaseType = gameManager->getLevelBase();
	levelForcesType = gameManager->getLevelForces();
	levelObstaclesType = gameManager->getLevelObstacles();

	changeLevelBase(0);
	changeLevelForces(0);
	changeLevelObstacles(0);

	songIndex = gameManager->getSong();

	changeSong(0);
}

void ConfigurationMenu::update(float deltaTime)
{
	checkInput();
}

void ConfigurationMenu::checkInput()
{
	bool pressed = false;

	int i = 0;
	while (i < 5 && !pressed)
	{
		// Fill or clear slots
		int slotIndex = isIndexConnected(i);
		bool enterButton = (i < 4 && inputSystem->getButtonPress(i, "X")) || (i == 4 && inputSystem->getKeyPress("SPACE"));
		bool exitButton = (i < 4 && (inputSystem->getButtonPress(i, "X") || !inputSystem->isControllerConnected(i))) || (i == 4 && inputSystem->getKeyPress("SPACE"));

		if (nPlayers < 4 && slotIndex == -1 && enterButton)
			fillSlot(i);
		else if (slotIndex != -1 && exitButton)
			clearSlot(slotIndex);

		// Fill or clear with IA
		int enterIASlot = isIndexConnected(-1);
		bool enterIAButton = (i < 4 && inputSystem->getButtonPress(i, "RB"));

		int exitIASlot = isIndexConnected(9);
		bool exitIAButton = (i < 4 && inputSystem->getButtonPress(i, "LB"));

		if (nPlayers < 4 && enterIASlot != -1 && enterIAButton)
			slotButtonClick(enterIASlot, "Slot" + std::to_string(enterIASlot + 1));
		else if (exitIASlot != -1 && exitIAButton)
			slotButtonClick(exitIASlot, "Slot" + std::to_string(exitIASlot + 1));

		// Close Settings Panel or back to Main Menu
		bool escape = i == 4 && inputSystem->getKeyPress("ESCAPE");
		bool b = i < 4 && inputSystem->getButtonPress(i, "B");

		if (escape || b)
		{
			pressed = true;

			if (settingsPanel.isVisible())
				settingsButtonClick();
			else
				backButtonClick();
		}

		// Enter or Start to initiate the game
		bool enter = i == 4 && inputSystem->getKeyPress("RETURN");
		bool start = i < 4 && inputSystem->getButtonPress(i, "START");

		if ((enter || start) && startButton.isVisible())
		{
			pressed = true;
			startButtonClick();
		}

		i++;
	}
}

void ConfigurationMenu::fillSlot(int index)
{
	int i = 0;
	bool find = false;

	while (i < slots.size() && !find)
	{
		if (slots[i].first == -1)
		{
			find = true;

			slots[i].first = index;
			slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Text").setText("Player " + std::to_string(i + 1));
			slots[i].second.getChild("Slot" + std::to_string(i + 1) + "Button").setVisible(false);
		}
		i++;
	}

	nPlayers++;

	if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
		startButton.setVisible(true);
}

void ConfigurationMenu::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.getChild("Slot" + std::to_string(index + 1) + "Text").setText("Press SPACE or X");
	slots[index].second.getChild("Slot" + std::to_string(index + 1) + "Button").setVisible(true);

	nPlayers--;

	if (startButton.isVisible() && nPlayers < MIN_PLAYERS)
		startButton.setVisible(false);
}

int ConfigurationMenu::isIndexConnected(int index)
{
	int i = 0;

	if (index == 9)
	{
		i = slots.size() - 1;
		while (i >= 0 && slots[i].first != index) i--;
	}
	else
	{
		i = 0;
		while (i < slots.size() && slots[i].first != index) i++;
	}

	if (i < slots.size() && slots[i].first == index)
		return i;

	return -1;
}

bool ConfigurationMenu::changeHealth(int value)
{
	health += value;

	if (health < MIN_HEALTH) health = MIN_HEALTH;
	if (health > MAX_HEALTH) health = MAX_HEALTH;

	settingsPanel.getChild("Background").getChild("Health").setText(std::to_string(health));

	return false;
}

bool ConfigurationMenu::changeTimeMode(int value)
{
	mode = value;
	settingsPanel.getChild("Background").getChild("TimeMode").setText(timeModes[mode]);

	return false;
}

bool ConfigurationMenu::changeTime(int value)
{
	time += value;

	if (time < MIN_TIME) time = MIN_TIME;
	if (time > MAX_TIME) time = MAX_TIME;

	settingsPanel.getChild("Background").getChild("Time").setText(std::to_string(time));

	return false;
}

bool ConfigurationMenu::changeLevelBase(int value)
{
	levelBaseType += value;

	int index = abs(levelBaseType) % (BASE_TYPES + 1);

	configurationLayout->getRoot().getChild("BaseImage").setVisible(true);
	configurationLayout->getRoot().getChild("BaseImage").setProperty("Image", "base" + std::to_string(index + 1));

	GameManager::GetInstance()->setLevelBase(index);

	return false;
}

bool ConfigurationMenu::changeLevelObstacles(int value)
{
	levelObstaclesType += value;

	int index = abs(levelObstaclesType) % (OBSTACLES_TYPES + 1);

	if (index == 0)
		configurationLayout->getRoot().getChild("ObstaclesImage").setVisible(false);
	else
	{
		configurationLayout->getRoot().getChild("ObstaclesImage").setVisible(true);
		configurationLayout->getRoot().getChild("ObstaclesImage").setProperty("Image", "obstacles" + std::to_string(index));
	}

	GameManager::GetInstance()->setLevelObstacles(index);

	return false;
}

bool ConfigurationMenu::changeLevelForces(int value)
{
	levelForcesType += value;

	int index = abs(levelForcesType) % (FORCES_TYPES + 1);

	if (index == 0)
		configurationLayout->getRoot().getChild("ForcesImage").setVisible(false);
	else
	{
		configurationLayout->getRoot().getChild("ForcesImage").setVisible(true);
		configurationLayout->getRoot().getChild("ForcesImage").setProperty("Image", "forces" + std::to_string(index));
	}

	GameManager::GetInstance()->setLevelForces(index);

	return false;
}

bool ConfigurationMenu::randomizeButtonClick()
{
	levelBaseType = random(0, BASE_TYPES);
	levelObstaclesType = random(0, OBSTACLES_TYPES + 1);
	levelForcesType = random(0, FORCES_TYPES + 1);

	changeLevelBase(0);
	changeLevelObstacles(0);
	changeLevelForces(0);

	return false;
}

bool ConfigurationMenu::changeSong(int value)
{
	songIndex += value;

	int index = abs(songIndex) % songNames.size();
	configurationLayout->getRoot().getChild("PreviewSongButton").setText(songNames[index]);

	GameManager::GetInstance()->setSong(index);
	GameManager::GetInstance()->setSongName(songNames[index]);

	return false;
}

bool ConfigurationMenu::slotButtonClick(int index, std::string name)
{
	if (slots[index].first == -1)
	{
		slots[index].first = 9;
		slots[index].second.getChild(name + "Text").setText("IA");
		slots[index].second.getChild(name + "Button").setText("Clear IA");

		nPlayers++;

		if (!startButton.isVisible() && nPlayers >= MIN_PLAYERS)
			startButton.setVisible(true);
	}
	else
	{
		slots[index].first = -1;
		slots[index].second.getChild(name + "Text").setText("Press SPACE or X");
		slots[index].second.getChild(name + "Button").setText("Insert IA");

		nPlayers--;

		if (startButton.isVisible() && nPlayers < MIN_PLAYERS)
			startButton.setVisible(false);
	}

	return false;
}

bool ConfigurationMenu::startButtonClick()
{
	GameManager* gameManager = GameManager::GetInstance();

	gameManager->setPlayersAlive(nPlayers);
	gameManager->initPlayerRanking(nPlayers);

	std::vector<int> indexes;
	for (int i = 0; i < 4; i++)
		indexes.push_back(slots[i].first);

	gameManager->setPlayerIndexes(indexes);

	gameManager->setHealth(health);
	gameManager->setTimeMode(mode);

	if (mode)
		gameManager->setTime(-1);
	else
		gameManager->setTime(time);

	SceneManager::GetInstance()->changeScene("Game");
	return false;
}

bool ConfigurationMenu::settingsButtonClick()
{
	if (!settingsPanel.isVisible())
	{
		settingsPanel.setVisible(true);
		settingsPanel.setAlwaysOnTop(true);

		InterfaceSystem::GetInstance()->clearControllerMenuInput();
		InterfaceSystem::GetInstance()->initControllerMenuInput(&settingsPanel);
	}
	else
	{
		settingsPanel.setVisible(false);
		settingsPanel.setAlwaysOnTop(false);

		InterfaceSystem::GetInstance()->clearControllerMenuInput();
	}

	return false;
}

bool ConfigurationMenu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	return false;
}