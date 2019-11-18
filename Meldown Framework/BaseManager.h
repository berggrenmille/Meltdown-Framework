#pragma once
#include "Framework.h"

namespace Meltdown
{
	namespace Core
	{
		class Framework; //Forward decl.
	}
	/// <summary>
	/// BaseManager is the Base Class for all Managers, it grants access to the Meltdown::Core::Framework class.
	/// </summary>
	class BaseManager
	{
	public:
		explicit BaseManager(Core::Framework& _framework)
			: framework(_framework) {}

	protected:
		Core::Framework const& framework;
	};
}
