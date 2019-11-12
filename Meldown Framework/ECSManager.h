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

			/// <summary>
			/// AddEntity spawns a new entity to the world.
			/// The new entity will have 0 components attached.
			/// </summary>
			void AddEntity();
			/// <summary>
			/// RemoveEntity disables an entity from the world.
			/// The entity will still exist in memory, ready to be reborn.
			/// </summary>
			/// <param name="entity"></param>
			void RemoveEntity(EntityHandle& entity);
			/// <summary>
			/// AddComponent binds a component to an entity.
			/// </summary>
			template <typename C, typename ... Args>
			void AddComponent(EntityHandle& entity);
			/// <summary>
			/// RemoveComponent unbinds a component from an entity.
			/// </summary>
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

			Memory::PoolAllocator*      entityAllocator;	   //Entities are stored in a pool
			Memory::ChunkListAllocator* componentAllocator;    //Components are stored in a mega array

			unsigned aliveEntities = 0;

			Core::Engine* engine;
		};
	}
}
