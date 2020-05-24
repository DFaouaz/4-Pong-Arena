#include "PlayerIndex.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <sstream>

REGISTER_FACTORY(PlayerIndex);
PlayerIndex::PlayerIndex(GameObject* gameObject) :UserComponent(gameObject)
{
	id = -1;
}

PlayerIndex::~PlayerIndex()
{
}

void PlayerIndex::handleData(ComponentData* data)
{
	if (data == nullptr) return;

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "id")
		{
			if (!(ss >> id))
				LOG("PLAYER INDEX: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("PLAYER INDEX: Invalid property name \"%s\"", prop.first.c_str());
	}
}

int PlayerIndex::getId() const
{
	return id;
}

void PlayerIndex::setId(int ind)
{
	id = ind;
}

int PlayerIndex::getPosVector() const
{
	return pos;
}

void PlayerIndex::setPos(int posVector)
{
	pos = posVector;
}
