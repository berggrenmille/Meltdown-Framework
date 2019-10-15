#pragma once
namespace Meltdown
{
	class Core; //Forward decl.

	class BaseManager
	{
	public:
		explicit BaseManager(Core& _core)
			: core(_core) {}

	protected:
		Core const& core;
	};
}