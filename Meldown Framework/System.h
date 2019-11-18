#pragma once
namespace Meltdown
{
	namespace Core
	{
		class Framework;
	}
	namespace ECS
	{
		class System
		{
		public:
			virtual ~System() = default;
			virtual void Init() {};
			virtual void Quit() {};
		protected:
			friend Core::Framework;
			Core::Framework& framework;
			size_t id = -1;
		};
	}
}