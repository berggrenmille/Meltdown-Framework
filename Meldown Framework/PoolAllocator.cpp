#include "PoolAllocator.h"
#include <cassert>
#include "MemoryHelper.hpp"

Meltdown::Memory::PoolAllocator::PoolAllocator(size_t objectSize, U8 objectAlignment, size_t dedicatedMemory, void* start)
	: BaseAllocator(dedicatedMemory, start), objectSize(objectSize), objectAlignment(objectAlignment)
{
	assert(objectSize >= sizeof(void*) && "The size of object ready to be PoolAllocated need to be larger than that of a voidPtr");

	U8 adjustment = AlignForwardAdjustment(start, objectAlignment);
	freeSpaceList = static_cast<void**>(Push(start, adjustment));
	size_t numObjects = (dedicatedMemory - adjustment) / objectSize;
	void** freeListPtr = freeSpaceList;

	//Init list
	for (size_t i = 0; i < numObjects - 1; ++i)
	{
		*freeListPtr = Push(freeListPtr, objectSize);
		freeListPtr = static_cast<void**>(*freeListPtr);
	}

	*freeListPtr = nullptr;
	allowDeallocate = true;
}

Meltdown::Memory::PoolAllocator::~PoolAllocator()
{
	freeSpaceList = nullptr;
}

void* Meltdown::Memory::PoolAllocator::Allocate(size_t size, U8 alignment)
{
	assert((size == objectSize && alignment == objectAlignment) && "In a poolAllocator, allocations always must be of the same size and aligment");
	if (freeSpaceList == nullptr)
		return nullptr;
	void* ptr = freeSpaceList;
	freeSpaceList = static_cast<void**>(*freeSpaceList);
	usedMemory += size;
	numAllocations++;
	return ptr;
}

void Meltdown::Memory::PoolAllocator::Deallocate(void* ptr)
{
	*static_cast<void**>(ptr) = freeSpaceList;
	freeSpaceList = static_cast<void**>(ptr);
	usedMemory -= objectSize;
	numAllocations--;
}
