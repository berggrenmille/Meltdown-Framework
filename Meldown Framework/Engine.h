#pragma once
#include "LinearAllocator.h"
#include "ProxyAllocator.h"
#include "ChunkListAllocator.h"

//Forward declarations
class EventManager;
class ECSManager;
class JobManager;

namespace Meltdown
{
	namespace Core
	{
		/// <summary>
		/// Engine acts like the main interface between the user and the architecture. It is what unifies the architecture.
		/// </summary>
		class Engine
		{
		public:
			Engine();
			~Engine();

			Memory::ProxyAllocator& GetGlobalAllocator() const;
			Memory::LinearAllocator& GetDynamicAllocator() const;
		private:
			EventManager* eventManager = nullptr;

			Memory::LinearAllocator* dynamicAllocator = nullptr; //Memory refreshed every frame
			Memory::ChunkListAllocator* globalAllocator  = nullptr; //Memory is static during the whole program
			Memory::ProxyAllocator* proxyAllocator   = nullptr; //For debugging purposes 
		};
	}
}
