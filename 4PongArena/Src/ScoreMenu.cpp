#include "ScoreMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <Camera.h>

#include "Score.h"
#include "GameManager.h"

REGISTER_FACTORY(ScoreMenu);

ScoreMenu::ScoreMenu(GameObject* gameObject) : Menu(gameObject), gameManager(nullptr), texts(), panels()
{

}

ScoreMenu::~ScoreMenu()
{
	gameManager = nullptr;
	texts.clear();
	panels.clear();
}

void ScoreMenu::start()
{
	Menu::start();

	gameManager = GameManager::GetInstance();

	if (gameManager == nullptr) return;

	GameObject* camera = findGameObjectWithName("MainCamera");
	UILayout* layout = nullptr;
	UIElement root = NULL;

	if (camera != nullptr)
	{
		layout = camera->getComponent<UILayout>();

		if (layout != nullptr)
			root = layout->getRoot();
	}

	Score* score = gameManager->getScore();
	positions = score->getPlayerIDs();

	for (int i = 0; i < positions.size(); i++)
	{
		std::string name = "P" + std::to_string(positions[i]);
		texts.push_back(root.getChild(name));
		root.getChild(name).setText("Player " + std::to_string(positions[i]) + ": " + std::to_string(gameManager->getPlayerRanking(positions[i])) + "�");

		name = name + "Background";
		panels.push_back(root.getChild(name));
	}

	if (gameManager->getWinner() != -1)
		root.getChild("Result").setText("WINNER: PLAYER " + std::to_string(positions[gameManager->getWinner() - 1]));
	else
		root.getChild("Result").setText("TIE");

	reposition(gameManager->getInitialPlayers());
	initStatistics(gameManager->getInitialPlayers());
}

void ScoreMenu::update(float deltaTime)
{
	if (InputSystem::GetInstance() != nullptr && (InputSystem::GetInstance()->getKeyPress("ESCAPE") || checkControllersInput()))
		if (SceneManager::GetInstance() != nullptr) SceneManager::GetInstance()->changeScene("ConfigurationMenu");
}

bool ScoreMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (InputSystem::GetInstance() != nullptr && InputSystem::GetInstance()->getButtonPress(i, "B"))
			result = true;

		i++;
	}

	return result;
}

void ScoreMenu::reposition(int numOfPlayers)
{

	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();

	float size = (1 - 0.2) / numOfPlayers;
	float iTextPos = 0.15;
	float iPanelPos = 0.1;

	for (int i = 0; i < numOfPlayers; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;

		if (i < texts.size()) {
			texts.at(i).setVisible(true);
			texts.at(i).setPosition(textPos, 0.2);

			panels.at(i).setVisible(true);
			panels.at(i).setPosition(panelPos, 0.3);
			panels.at(i).setSize(size, 0.4);
		}
	}
}

void ScoreMenu::initStatistics(int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
		setNumOfHits(i);
		setNumOfGoals(i);
		setNumOfSelfGoals(i);
		setTimeAlive(i);
	}
}

void ScoreMenu::setNumOfHits(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	if (score == nullptr)return;
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "NumOfHits";

	if (playerIndex >= 0)
		panels.at(playerIndex).getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));
}

void ScoreMenu::setNumOfGoals(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	if (score == nullptr)return;
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "NumOfGoals";

	if (playerIndex >= 0)
		panels.at(playerIndex).getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
}

void ScoreMenu::setNumOfSelfGoals(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	if (score == nullptr)return;
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "NumOfSelfGoals";

	if (playerIndex >= 0)
		panels.at(playerIndex).getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
}

void ScoreMenu::setTimeAlive(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	if (score == nullptr)return;
	std::string name = "P" + std::to_string(positions[playerIndex]);
	name = name + "TimeAlive";

	if (playerIndex >= 0)
		panels.at(playerIndex).getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
}