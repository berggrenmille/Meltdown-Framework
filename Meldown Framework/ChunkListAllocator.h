#pragma once
#include "MemoryHelper.hpp"
namespace Meltdown
{
	namespace Memory
	{
		 ///-------------------------------------------------------------------------------------------------
		 /// @brief ChunkListAllocator is the most flexible allocator.
		 ///		ChunkListAllocator works like a chain, where there is a chain of "memory chunks".
		 ///		The allocator looks for the first available spot in memory, going from the first chunk to the next.
		 ///		Allocations and Deallocations

		class ChunkListAllocator : public BaseAllocator
		{
		public:
			ChunkListAllocator(size_t dedicatedMemory, void* start);
			~ChunkListAllocator();

			void* Allocate(size_t size, U8 alignment) override;
			void Deallocate(void* ptr) override;

			//NO COPIES
			ChunkListAllocator(const ChunkListAllocator&) = delete;
			ChunkListAllocator& operator=(const ChunkListAllocator&) = delete;
		private:

			struct Header
			{
				size_t size;
				U8 adjustment;
			};
			struct Chunk
			{
				size_t size;
				Chunk* nextPtr;
			};

			Chunk* chunkPtr;
		};
	}
}
