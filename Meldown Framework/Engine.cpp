#include "Engine.h"
#include "Settings.hpp"
#include "ECSManager.h"
#include "EventManager.h"

Meltdown::Core::Engine::Engine()
{
	globalAllocator  = new Memory::ChunkListAllocator(Settings::GLOBAL_MEMORY, malloc(Settings::GLOBAL_MEMORY));
	proxyAllocator = new Memory::ProxyAllocator(*globalAllocator);

	void* dynamicMemory = proxyAllocator->Allocate(Settings::DYNAMIC_MEMORY, Memory::NORMAL_ALIGNMENT);
	dynamicAllocator = new Memory::LinearAllocator(Settings::DYNAMIC_MEMORY, dynamicMemory);

	ecsManager = Memory::AllocateNew<ECS::ECSManager>(*globalAllocator, this);
}

Meltdown::Core::Engine::~Engine()
{
	delete proxyAllocator;
	delete dynamicAllocator;
	delete globalAllocator;
}

Meltdown::ECS::ECSManager& Meltdown::Core::Engine::GetECSManager() const
{
	return *ecsManager;
}

EventManager& Meltdown::Core::Engine::GetEventManager() const
{
	return *eventManager;
}

Meltdown::Memory::ProxyAllocator& Meltdown::Core::Engine::GetGlobalAllocator() const
{
	return *proxyAllocator;
}

Meltdown::Memory::LinearAllocator& Meltdown::Core::Engine::GetDynamicAllocator() const
{
	return *dynamicAllocator;
}
