#include "PlayerController.h"
#include <InputSystem.h>
#include <GameObject.h>
#include <sstream>

#include "Movement.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(PlayerController);

PlayerController::PlayerController(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), movement(nullptr), player()
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::start()
{
	inputSystem = InputSystem::GetInstance();
	movement = gameObject->getComponent<Movement>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	moving = false;
	hasMoved = false;
}

void PlayerController::update(float deltaTime)
{
	checkInput();
	if (hasMoved && !moving) {
		moving = true;
		soundEmitter->playSound("Paddle_Move");
	}
	else if (moving && !hasMoved) moving = false;
}

void PlayerController::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "id")
		{
			if (!(ss >> player.id))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "index")
		{
			if (!(ss >> player.index))
				LOG("PLAYER CONTROLLER: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER CONTROLLER: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void PlayerController::setPlayer(int id, int index)
{
	player.id = id;
	player.index = index;
}

Player PlayerController::getPlayer() const
{
	return player;
}

void PlayerController::checkInput()
{
	if (movement == nullptr) return;

	Vector3 normal = movement->getNormal();
	Vector3 motionDirection(-normal.z, 0.0, normal.x);
	Vector3 directionMask = Vector3(abs(-normal.z), 0.0, abs(normal.x)).normalized();
	Vector3 axis = getInputAxis();
	axis *= directionMask; axis *= motionDirection;

	if (axis.x > 0.0) { movement->moveRight(); hasMoved = true; }
	else if (axis.x < 0.0) { movement->moveLeft(); hasMoved = true; }
	else if (axis.z > 0.0) { movement->moveRight(); hasMoved = true; }
	else if (axis.z < 0.0) { movement->moveLeft(); hasMoved = true; }
	else { movement->stop(); hasMoved = false; }
}

Vector3 PlayerController::getInputAxis() const
{
	if (player.index < 0 || player.index > 4) return Vector3::ZERO;

	if (player.index == 4)
		return getKeyboardAxis();

	return getControllerAxis();
}

Vector3 PlayerController::getKeyboardAxis() const
{
	Vector3 axis = Vector3::ZERO;
	if (inputSystem->isKeyPressed("A")) axis.x += -1;	// Left
	if (inputSystem->isKeyPressed("D")) axis.x += 1;	// Right
	if (inputSystem->isKeyPressed("W")) axis.z += -1;	// Up
	if (inputSystem->isKeyPressed("S")) axis.z += 1;	// Down

	return axis;
}

Vector3 PlayerController::getControllerAxis() const
{
	Vector3 axis = Vector3::ZERO;
	std::pair<int, int> leftJoystick = inputSystem->getLeftJoystick(player.index);
	if (leftJoystick.first < 0 || inputSystem->isButtonPressed(player.index, "Left")) axis.x += -1;	// Left
	if (leftJoystick.first > 0 || inputSystem->isButtonPressed(player.index, "Right")) axis.x += 1;	// Right
	if (leftJoystick.second < 0 || inputSystem->isButtonPressed(player.index, "Up")) axis.z += -1;	// Up
	if (leftJoystick.second > 0 || inputSystem->isButtonPressed(player.index, "Down")) axis.z += 1;	// Down

	return axis;
}