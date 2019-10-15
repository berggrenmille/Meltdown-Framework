#pragma once
#include "BaseManager.h"

namespace Meltdown
{
	/// <summary>
	/// EventManager manage all event related activity in the framework. It allows for communication between different parts of the architecture.
	/// </summary>
	class EventManager : BaseManager
	{
	public:
		explicit EventManager(Core::Engine& _engine)
			: BaseManager(_engine) {}
	};
}
