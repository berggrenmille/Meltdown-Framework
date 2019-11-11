#include "Engine.h"
#include "Settings.hpp"

Meltdown::Core::Engine::Engine()
{
	globalAllocator  = new Memory::ChunkListAllocator(Settings::GLOBAL_MEMORY, malloc(Settings::GLOBAL_MEMORY));
	proxyAllocator = new Memory::ProxyAllocator(*globalAllocator);

	void* dynamicMemory = proxyAllocator->Allocate(Settings::DYNAMIC_MEMORY, Memory::NORMAL_ALIGNMENT);
	dynamicAllocator = new Memory::LinearAllocator(Settings::DYNAMIC_MEMORY, dynamicMemory);
}

Meltdown::Core::Engine::~Engine()
{
	delete proxyAllocator;
	delete dynamicAllocator;
	delete globalAllocator;
}

Meltdown::Memory::ProxyAllocator& Meltdown::Core::Engine::GetGlobalAllocator() const
{
	return *proxyAllocator;
}

Meltdown::Memory::LinearAllocator& Meltdown::Core::Engine::GetDynamicAllocator() const
{
	return *dynamicAllocator;
}
