#pragma once
#include "LinearAllocator.h"
#include "ProxyAllocator.h"

//Forward declarations
class EventManager;
class SystemManager;
class EntityManager;
class ComponentManager;
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
		private:
			EventManager* eventManager = nullptr;

			Memory::LinearAllocator* dynamicAllocator = nullptr; //Memory refreshed every frame
			Memory::LinearAllocator* globalAllocator  = nullptr; //Memory is static during the whole program
			Memory::ProxyAllocator* ProxyAllocator   = nullptr; //For debugging purposes 
		};
	}
}
