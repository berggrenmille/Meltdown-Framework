#pragma once
#include <vector>
#include "Entity.h"
#include "Component.h"
#include "PoolAllocator.h"
#include "ChunkListAllocator.h"
#include "Engine.h"

namespace Meltdown
{
	namespace ECS
	{
		class ECSManager
		{
		public:
			explicit ECSManager(Core::Engine* engine);
			~ECSManager();

			void AddEntity();
			void RemoveEntity();

			template <typename C, typename ... Args>
			void AddComponent(EntityHandle& entity);
			template <typename C>
			void RemoveComponent(EntityHandle& entity);

			template <typename S, typename ... Args>
			void AddSystem(Args&& ... args);
			template <typename S>
			void RemoveSystem();

			void Refresh();
			void Quit();
		private:
			std::vector<EntityHandle*>  entityVector;
			std::vector<void*>          componentVector;

			Memory::PoolAllocator*      entityAllocator;	   //Entites are stored in a pool
			Memory::ChunkListAllocator* componentAllocator; //Components are stored in a mega array

			Core::Engine* engine;
		};
	}
}
