#pragma once
#include <vector>
#include "Entity.h"
#include "Component.h"
#include "PoolAllocator.h"
#include "ChunkListAllocator.h"
#include "IdFactory.hpp"
#include <memory>
#include <array>
#include <iostream>

namespace Meltdown
{
	namespace Core
	{
		class Engine;
	}
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
			EntityHandle& AddEntity();
			/// <summary>
			/// RemoveEntity disables an entity from the world.
			/// The entity will still exist in memory, ready to be reborn.
			/// </summary>
			/// <param name="entity">The Entity to be removed.</param>
			void RemoveEntity(EntityHandle& entity);
			/// <summary>
			/// AddComponent binds a component to an entity.
			/// </summary>
			template <typename C, typename ... Args>
			void AddComponent(EntityHandle& entity, Args&& ... args);
			/// <summary>
			/// RemoveComponent unbinds a component from an entity.
			/// </summary>
			template <typename C>
			void RemoveComponent(EntityHandle& entity);
			template <typename C>
			void RemoveComponent(ComponentHandle<C>& component);
			/// <summary>
			/// GetComponent return a pointer to a component bound to an entity.
			/// Returns nullptr if there does not exist a component.
			/// </summary>
			template <typename C>
			C* GetComponent(EntityHandle& entity);
			/// <summary>
			/// HasComponents checks if the entity has a set of components attached.
			/// Only returns true if all given components are bound.
			/// </summary>
			template <typename... Cs>
			bool HasComponents(EntityHandle& entity) const;
			/// <summary>
			/// GetComponentTuples return a vector with tuples of components. Each tuple represents a unique 
			/// TODO: Implement an entity reference with every tuple
			/// </summary>
			template <typename... Cs>
			std::unique_ptr<std::vector<std::tuple<Cs& ...>>> GetComponentTuples();
			template <typename... Cs>
			void ForEach(std::function<void(Cs&...)> function);
			
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

		template <typename C, typename ... Args>
		void ECSManager::AddComponent(EntityHandle& entity, Args&&... args)
		{
			entity.componentMask |= Util::TypeIdFactory<ComponentHandle<void>>::GetFlag<C>();
			C* componentPtr = Memory::AllocateNew<C>(*componentAllocator,args...);
			auto componentIndex = Util::TypeIdFactory<ComponentHandle<void>>::GetId<C>() + entity.dataIndex * Settings::MAX_COMPONENT_TYPES;
			ComponentHandle<C>* componentHandlePtr = Memory::AllocateNew<ComponentHandle<C>>(*componentAllocator, componentPtr);
			if (componentVector[componentIndex] != nullptr)
			{
				//Chain component
				ComponentHandle<C>* next = reinterpret_cast<ComponentHandle<C>*>(componentVector[componentIndex]);
				while (next->next != nullptr)
					next = next->next;
				next->next = componentHandlePtr;
			}
			else
				componentVector[componentIndex] = static_cast<void*>(componentHandlePtr);
		}

		template <typename C>
		void ECSManager::RemoveComponent(EntityHandle& entity)
		{
			if (!entity.isAlive)
				return;
			auto componentIndex = Util::TypeIdFactory<ComponentHandle<void>>::GetId<C>() + entity.dataIndex * Settings::MAX_COMPONENT_TYPES;
			if (componentVector[componentIndex] != nullptr)
				this->RemoveComponent(*static_cast<ComponentHandle<C>*>(componentVector[componentIndex]));
			//Change entity component mask
			entity.componentMask ^= Util::TypeIdFactory<ComponentHandle<void>>::GetFlag<C>();
		}

		template <typename C>
		void ECSManager::RemoveComponent(ComponentHandle<C>& component)
		{
			if(component.GetNextRaw() != nullptr)
				this->RemoveComponent(*static_cast<ComponentHandle<C>*>(component.GetNextRaw()));
			//Remove component
			componentAllocator->Deallocate(static_cast<void*>(&component));
		}

		template <typename ... Cs>
		bool ECSManager::HasComponents(EntityHandle& entity) const
		{
			size_t flags[] = { Util::TypeIdFactory<ComponentHandle<void>>::GetFlag<Cs>()... };
			size_t mask = 0;
			for (auto m : flags)
				mask |= m;
			return mask == (entity.componentMask & mask);
		}

		template <typename ... Cs>
		std::unique_ptr<std::vector<std::tuple<Cs& ...>>> ECSManager::GetComponentTuples()
		{
			auto tuples = std::make_unique<std::vector<std::tuple<Cs& ...>>>();
			std::size_t componentIndexes[] = { Util::TypeIdFactory<ComponentHandle<void>>::GetId<Cs>()... };
			//Loop trough component vector
			for(int i = 0; i < componentVector.size()/Settings::MAX_COMPONENT_TYPES; ++i)
			{
				int offset = i * Settings::MAX_COMPONENT_TYPES;
				bool hasComponents = true;
				for (auto index : componentIndexes)
				{
					if (componentVector[index + offset] == nullptr)
						hasComponents = false;
				}
				if(!hasComponents)
					continue;
				//Push components
				tuples->push_back(std::forward_as_tuple(
					*(reinterpret_cast<ComponentHandle<Cs>*>(componentVector[Util::TypeIdFactory<ComponentHandle<void>>::GetId<Cs>() + offset])->GetRaw())...));
			}
			return tuples;
		}

		template <typename ... Cs>
		void ECSManager::ForEach(std::function<void(Cs&...)> function)
		{
			size_t componentMaskList[] = { Util::TypeIdFactory<ComponentHandle<void>>::GetFlag<Cs>() ... };
			size_t mask = 0;
			for (size_t flag : componentMaskList)
				mask |= flag;
		
			//Find all entities with a certain set of components and call the function
			for (auto i = 0; i < aliveEntities; ++i)
			{
				if ((entityVector[i]->componentMask & mask) == mask)
					function((*reinterpret_cast<ComponentHandle<Cs>*>(componentVector[entityVector[i]->dataIndex * Settings::MAX_COMPONENT_TYPES + Util::TypeIdFactory<ComponentHandle<void>>::GetId<Cs>()])->GetRaw())...);
			}
		}

		template <typename C>
		C* ECSManager::GetComponent(EntityHandle& entity)
		{
			auto componentIndex = Util::TypeIdFactory<ComponentHandle<void>>::GetId<C>() + entity.dataIndex * Settings::MAX_COMPONENT_TYPES;
			if (!componentVector[componentIndex])
				return nullptr;
			return reinterpret_cast<ComponentHandle<C>*>(componentVector[componentIndex])->GetRaw();
		}
	}
}
