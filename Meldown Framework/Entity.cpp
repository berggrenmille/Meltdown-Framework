#include "Entity.h"

Meltdown::ECS::EntityHandle& Meltdown::ECS::EntityHandle::operator=(EntityHandle&& other) noexcept
{
	EntityHandle temp;
	temp.dataIndex = this->dataIndex;
	temp.componentMask = this->componentMask;
	temp.id = this->id;
	temp.isAlive = this->isAlive;

	this->dataIndex = other.dataIndex;
	this->componentMask = other.componentMask;
	this->id = other.id;
	this->isAlive = other.isAlive;

	other.dataIndex = temp.dataIndex;
	other.componentMask = temp.componentMask;
	other.id = temp.id;
	other.isAlive = temp.isAlive;

	return *this;
}