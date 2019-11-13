#pragma once
#include <bitset>
#include "Settings.hpp"
namespace Meltdown
{
	namespace ECS
	{
		struct EntityHandle
		{
			size_t index = -1;
			size_t dataIndex = -1;
			int8_t isAlive = 0;
			int8_t pendingDeath = 0;
			size_t componentMask;

			EntityHandle& operator=(EntityHandle& other) noexcept;
		};
	}
}
