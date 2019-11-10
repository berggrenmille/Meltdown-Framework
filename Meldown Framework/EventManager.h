#pragma once
#include "BaseManager.h"
#include <unordered_map>
#include <vector>
#include "Event.h"

namespace Meltdown
{
	namespace Event
	{
		class EventManager : BaseManager
		{
		public:
			explicit EventManager(Core::Engine& engine) : BaseManager(engine) {};
			template <typename T>
			void PushEvent(T&& newEvent);
			template<typename T>
			void SubscribeToEvent();

		private:
			std::vector<BaseEvent*> events;
		};

		template <typename T>
		void EventManager::PushEvent(T&& newEvent)
		{
		}

		template <typename T>
		void EventManager::SubscribeToEvent()
		{
		}
	}
}
