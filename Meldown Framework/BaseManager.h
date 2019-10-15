#pragma once
namespace Meltdown
{
	class Core; //Forward decl.

	/// <summary>
	/// BaseManager is the Base Class all managers inherits from, it grants access to the Meltdown::Core class.
	/// </summary>
	class BaseManager
	{
	public:
		explicit BaseManager(Core& _core)
			: core(_core) {}

	protected:
		Core const& core;
	};
}