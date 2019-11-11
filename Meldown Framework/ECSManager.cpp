#include "ECSManager.h"


Meltdown::ECS::ECSManager::ECSManager(Core::Engine* engine)
{
	this->engine = engine;

	void* entityStartPtr = engine->GetGlobalAllocator().Allocate(Settings::MAX_ENTITIES * sizeof(EntityHandle), alignof(EntityHandle));
	this->entityAllocator = new Memory::PoolAllocator(sizeof(EntityHandle), alignof(EntityHandle), Settings::MAX_ENTITIES * sizeof(EntityHandle), entityStartPtr);

	void* componentStartPtr = engine->GetGlobalAllocator().Allocate(Settings::MAX_ENTITIES * sizeof(EntityHandle), Memory::NORMAL_ALIGNMENT);
	this->componentAllocator = new Memory::ChunkListAllocator(Settings::MAX_ENTITIES * sizeof(EntityHandle), componentStartPtr);
}

Meltdown::ECS::ECSManager::~ECSManager()
{
	delete entityAllocator;
	delete componentAllocator;
}

void Meltdown::ECS::ECSManager::AddEntity()
{
	EntityHandle* newEnt = Memory::AllocateNew<EntityHandle>(*entityAllocator);
	newEnt->componentMask.reset();
	newEnt->dataIndex = 
	entityVector.push_back(newEnt);
}
