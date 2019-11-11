#pragma once
#include <type_traits>
namespace Meltdown
{
	namespace ECS
	{
		class BaseComponent
		{
		protected:
			size_t typeId = -1;
		};

		template<typename T>
		struct ComponentHandle
		{
			BaseComponent* componentPtr = nullptr;
		};
	}
}