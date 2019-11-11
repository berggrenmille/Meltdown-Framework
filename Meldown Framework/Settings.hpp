#pragma once
namespace Meltdown
{
	constexpr size_t GB(double d) { return static_cast<size_t>(d * 1024 * 1024 * 1024); };
	constexpr size_t MB(double d) { return static_cast<size_t>(d * 1024 * 1024); };
	constexpr size_t KB(double d) { return static_cast<size_t>(d * 1024); };
	
	namespace Settings
	{
		constexpr size_t GLOBAL_MEMORY = GB(1);
		constexpr  size_t DYNAMIC_MEMORY = GB(0.5);

		constexpr size_t MAX_COMPONENT_TYPES = sizeof(size_t) * 8;
		constexpr size_t MAX_ENTITIES = 1 << 10;
		constexpr size_t MAX_SYSTEMS = 64;
	}
}
