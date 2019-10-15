#pragma once
#include "BaseManager.h"

namespace Meltdown
{
	class EventManger : BaseManager
	{
	public:
		explicit EventManger(Core& _core)
			: BaseManager(_core) {}
	};
}
