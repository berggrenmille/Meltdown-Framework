#include "Engine.h"
#include "Settings.hpp"

Meltdown::Core::Engine::Engine()
{
	globalAllocator  = new Memory::LinearAllocator(Settings::GLOBAL_MEMORY, malloc(Settings::GLOBAL_MEMORY));
	ProxyAllocator   = new Memory::ProxyAllocator(*globalAllocator);

	void* dynamicMemory = globalAllocator->Allocate(Settings::DYNAMIC_MEMORY, Memory::NORMAL_ALIGNMENT);
	dynamicAllocator = new Memory::LinearAllocator(Settings::DYNAMIC_MEMORY, dynamicMemory);
}

Meltdown::Core::Engine::~Engine()
{
	delete ProxyAllocator;
	delete dynamicAllocator;
	delete globalAllocator;
}
