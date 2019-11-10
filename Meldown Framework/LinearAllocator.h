#pragma once
#include "MemoryHelper.hpp"

namespace Meltdown
{
	namespace Memory
	{
		/*
		 * LinearAllocator is the simplest and fastest allocator
		 * It starts allocating at the first point in memory and then allocates at the nearest available space forward
		 * No deallocations can be made, but it can be reset so that allocation starts from zero
		 */

		 ///-------------------------------------------------------------------------------------------------
		 /// @brief LinearAllocator is the simplest and fastest allocator.
		 /// 	   The LinearAllocator starts allocating at the first point in memory and continues allocating memory at the end of the last allocation.
		 /// @attention 
		 /// 	   No deallocations can be made, but it can be reset so that allocation starts from zero

		class LinearAllocator : public BaseAllocator
		{
		public:
			LinearAllocator(size_t dedicatedMemory, void* start);
			~LinearAllocator();

			void* Allocate(size_t size, U8 alignment) override;
			void  Deallocate(void* ptr) override;
			void  reset();

			//NO COPIES
			LinearAllocator(const LinearAllocator&) = delete;
			LinearAllocator& operator=(const LinearAllocator&) = delete;
		private:
			void* currentPtr;
		};
	}
}

