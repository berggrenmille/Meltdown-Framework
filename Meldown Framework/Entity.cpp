#include "Entity.h"

Meltdown::ECS::EntityHandle& Meltdown::ECS::EntityHandle::operator=(EntityHandle& other) noexcept
{
	EntityHandle temp;
	temp.dataIndex = this->dataIndex;
	temp.componentMask = this->componentMask;
	temp.index = this->index;
	temp.isAlive = this->isAlive;

	this->dataIndex = other.dataIndex;
	this->componentMask = other.componentMask;
	this->index = other.index;
	this->isAlive = other.isAlive;

	other.dataIndex = temp.dataIndex;
	other.componentMask = temp.componentMask;
	other.index = temp.index;
	other.isAlive = temp.isAlive;

	return *this;
}