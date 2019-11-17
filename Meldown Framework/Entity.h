#pragma once
#include <bitset>
#include "Settings.hpp"
namespace Meltdown
{
	namespace ECS
	{
		struct EntityHandle
		{
			uint32_t index = -1;
			uint32_t dataIndex = -1;
			int8_t isAlive = 0;
			int8_t pendingDeath = 0;
			uint32_t componentMask = 0;

			EntityHandle& operator=(EntityHandle& other) noexcept;
		};
	}
}
