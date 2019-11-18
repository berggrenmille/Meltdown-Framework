#include "Framework.h"
#include "Settings.hpp"
#include "ECSManager.h"
#include "EventManager.h"

Meltdown::Core::Framework::Framework()
{
	void* globalMemory = malloc(Settings::GLOBAL_MEMORY);
	assert(globalMemory && "Failed to assign memory to application!");
	globalAllocator  = new Memory::ChunkListAllocator(Settings::GLOBAL_MEMORY, globalMemory);
	proxyAllocator	 = new Memory::ProxyAllocator(*globalAllocator);

	void* dynamicMemory = proxyAllocator->Allocate(Settings::DYNAMIC_MEMORY, Memory::NORMAL_ALIGNMENT);
	dynamicAllocator	= new Memory::LinearAllocator(Settings::DYNAMIC_MEMORY, dynamicMemory);

	ecsManager = Memory::AllocateNew<ECS::ECSManager>(*globalAllocator, this);
	eventManager = Memory::AllocateNew<Event::EventManager>(*globalAllocator, this);
}

Meltdown::Core::Framework::~Framework()
{
	void* mem = globalAllocator->GetStart();
	delete proxyAllocator;
	delete dynamicAllocator;
	delete globalAllocator;
	free(mem);
}
class UpdateEvent : public Meltdown::Event::BaseEvent {};
void Meltdown::Core::Framework::Update()
{
	eventManager->PushEvent<UpdateEvent>(*Memory::AllocateNew<UpdateEvent>(*dynamicAllocator));
}

void Meltdown::Core::Framework::Refresh()
{

}

Meltdown::ECS::ECSManager& Meltdown::Core::Framework::GetECSManager() const
{
	return *ecsManager;
}

Meltdown::Event::EventManager& Meltdown::Core::Framework::GetEventManager() const
{
	return *eventManager;
}

Meltdown::Memory::ProxyAllocator& Meltdown::Core::Framework::GetGlobalAllocator() const
{
	return *proxyAllocator;
}

Meltdown::Memory::LinearAllocator& Meltdown::Core::Framework::GetDynamicAllocator() const
{
	return *dynamicAllocator;
}
