#pragma once
#include "BaseManager.h"
#include <unordered_map>
#include <vector>
#include "Event.h"
#include "IdFactory.hpp"
#include <algorithm>
#include <queue>

namespace Meltdown
{
	namespace Event
	{
		class EventManager : BaseManager
		{

		public:
			explicit EventManager(Core::Engine& engine) : BaseManager(engine) {};
			void FlushEvents();
			template<typename T>
			void PushEvent(BaseEvent& newEvent);
			template<typename T>
			void SubscribeToEvent(std::function<void(BaseEvent&)> callback);

		private:
			typedef std::unordered_multimap<size_t, std::function<void(BaseEvent&)>> CallbackMultiMap;
			CallbackMultiMap callbacks;
			std::queue<BaseEvent> eventQueue;
		};

		inline void EventManager::FlushEvents()
		{
			while (!eventQueue.empty())
			{
				auto event = eventQueue.front();
				const auto range = callbacks.equal_range(event.id);
				std::for_each(
					range.first,
			   range.second,
			   [&](CallbackMultiMap::value_type& x)
					{x.second(event); });
				eventQueue.pop();
			}
		}

		template <typename T>
		void EventManager::PushEvent(BaseEvent& newEvent)
		{
			
			auto id = Util::TypeIdFactory<BaseEvent>::GetId<T>();
			newEvent.id = id;
			eventQueue.push(newEvent);
			auto itRange = callbacks.equal_range(id);
			std::for_each(
				itRange.first,
				itRange.second,
				[](CallbackMultiMap::value_type& x) 
				{x.second(newEvent); }
			);
		}

		template <typename T>
		void EventManager::SubscribeToEvent(std::function<void(BaseEvent&)> callback)
		{
			callbacks.insert(std::make_pair(Util::TypeIdFactory<BaseEvent>::GetId<T>(), callback));
		}
	}
}
