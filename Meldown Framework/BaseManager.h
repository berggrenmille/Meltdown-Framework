#pragma once
#include "Engine.h"

namespace Meltdown
{
	namespace Core
	{
		class Engine; //Forward decl.
	}
	/// <summary>
	/// BaseManager is the Base Class for all Managers, it grants access to the Meltdown::Core::Engine class.
	/// </summary>
	class BaseManager
	{
	public:
		explicit BaseManager(Core::Engine& _engine)
			: engine(_engine) {}

	protected:
		Core::Engine const& engine;
	};
}
