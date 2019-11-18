#include "LinearAllocator.h"
#include "BaseAllocator.h"
#include "MemoryHelper.hpp"

Meltdown::Memory::LinearAllocator::LinearAllocator(size_t dedicatedMemory, void* start)
	: BaseAllocator(dedicatedMemory, start), currentPtr(start)
{
	assert((dedicatedMemory > 0));
}

Meltdown::Memory::LinearAllocator::~LinearAllocator()
{
	currentPtr = nullptr;
}

/*
 * Allocate works as simple as moving the currentPtr forward to the first free adress
 */
void* Meltdown::Memory::LinearAllocator::Allocate(size_t size, U8 alignment)
{
	assert(size != 0);
	const U8 adjustment = AlignForwardAdjustment(currentPtr, alignment);

	//Check so that a the used memory won't exceed dedicated memory limit 
	if (usedMemory + adjustment + size > dedicatedSize) return nullptr;

	const UPtr alignedPtr = reinterpret_cast<UPtr>(currentPtr) + adjustment;
	currentPtr = reinterpret_cast<void*>(alignedPtr + size);
	usedMemory += size + adjustment;
	numAllocations++;
	return reinterpret_cast<void*>(alignedPtr);
}

void Meltdown::Memory::LinearAllocator::Deallocate(void* ptr)
{
	assert(false && "Cannot Deallocate specific parts of allocated memory, use reset() instead!");
}

/*
 * Reset allocator, allowing allocations to start from the start;
 */
void Meltdown::Memory::LinearAllocator::reset()
{
	numAllocations = 0;
	usedMemory = 0;
	currentPtr = startPtr;
}

