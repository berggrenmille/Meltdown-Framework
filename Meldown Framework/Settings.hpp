#pragma once
namespace Meltdown
{
	constexpr uint32_t GB(double d) { return static_cast<size_t>(d * 1024 * 1024 * 1024); };
	constexpr uint32_t MB(double d) { return static_cast<size_t>(d * 1024 * 1024); };
	constexpr uint32_t KB(double d) { return static_cast<size_t>(d * 1024); };
	
	namespace Settings
	{
		constexpr  uint32_t GLOBAL_MEMORY = GB(2);
		constexpr  uint32_t DYNAMIC_MEMORY = GB(0.5);

		constexpr uint32_t MAX_COMPONENT_TYPES = sizeof(uint32_t) * 8;
		constexpr uint32_t MAX_ENTITIES = 2e+6;//1 << 10;
		constexpr uint32_t MAX_SYSTEMS = 64;
	}
}
