#pragma once
#include "LinearAllocator.h"
#include "ProxyAllocator.h"
#include "ChunkListAllocator.h"
//Forward declarations

class JobManager;

namespace Meltdown
{
	namespace Event
	{
		class EventManager;
	}
	namespace ECS
	{
		class ECSManager;
	}
	namespace Core
	{
		/// <summary>
		/// Framework acts like the main interface between the user and the architecture. It is what unifies the architecture.
		/// </summary>
		class Framework
		{
		public:
			Framework();
			~Framework();

			void Update();
			void Refresh();

			ECS::ECSManager& GetECSManager() const;
			Event::EventManager& GetEventManager() const;

			Memory::ProxyAllocator& GetGlobalAllocator() const;
			Memory::LinearAllocator& GetDynamicAllocator() const;
		private:
			Event::EventManager* eventManager = nullptr;
			ECS::ECSManager* ecsManager = nullptr;

			Memory::LinearAllocator*	dynamicAllocator = nullptr;	//Allocator that refreshes every update
			Memory::ChunkListAllocator* globalAllocator  = nullptr; //Allocator for application memory
			Memory::ProxyAllocator*		proxyAllocator   = nullptr;	//Allocator for debugging purposes 
		};
	}
}



