#pragma once
#include "BaseAllocator.h"

namespace Meltdown
{
	namespace Memory
	{
		///-------------------------------------------------------------------------------------------------
		/// @brief PoolAllocator is the best allocator for arrays.
		/// 	   PoolAllocator is very similar to the ChunkListAllocator, except that it always store things of the same size.
		/// 	   It is extremely fast, but not as fast as the LinearAllocator or StackAllocator. The area where the PoolAllocator really shines is it's ability to Deallocate and reuse memory very efficiently.
		///		   The best allocator if you are looking for cache coherence.
		/// @see ChunkListAllocator

		class PoolAllocator : public BaseAllocator
		{
		public:

			PoolAllocator(size_t objectSize, U8 objectAlignment, size_t dedicatedMemory, void* start);
			~PoolAllocator();
			void* Allocate(size_t size, U8 alignment) override;
			void Deallocate(void* ptr) override;

			//NO COPIES
			PoolAllocator(const PoolAllocator&) = delete;
			PoolAllocator& operator=(const PoolAllocator&) = delete;
		private:
			size_t  objectSize;
			U8		objectAlignment;
			void**  freeSpaceList;
		};
	}
}
