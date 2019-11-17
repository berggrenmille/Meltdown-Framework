#include "ChunkListAllocator.h"
#include <cassert>

Meltdown::Memory::ChunkListAllocator::ChunkListAllocator(uint32_t dedicatedMemory, void* start)
	: BaseAllocator(dedicatedMemory, start), chunkPtr(static_cast<Chunk*>(start))
{
	assert(dedicatedMemory > sizeof(Chunk));
	chunkPtr->size = dedicatedMemory;
	chunkPtr->nextPtr = nullptr;
	allowDeallocate = true;
}

Meltdown::Memory::ChunkListAllocator::~ChunkListAllocator()
{
	chunkPtr = nullptr;
}

void* Meltdown::Memory::ChunkListAllocator::Allocate(uint32_t size, U8 alignment)
{
	assert(size != 0 && alignment != 0);
	Chunk * previousChunk = nullptr;
	Chunk * currentChunk = chunkPtr;
	while (currentChunk != nullptr)
	{
		//Calculate adjustment needed to keep object correctly aligned 
		const U8 adjustment = AlignForwardAdjustmentWithHeader(currentChunk, alignment, sizeof(Header));
		uint32_t totalSize = size + adjustment;

		//If allocation doesn't fit in this FreeBlock, try the next 
		if (currentChunk->size < totalSize)
		{
			previousChunk = currentChunk;
			currentChunk = currentChunk->nextPtr;
			continue;
		}

		//check so the memory specification are valid
		static_assert(sizeof(Header) >= sizeof(currentChunk), "sizeof(Header) < sizeof(Chunk)");

		//If allocations in the remaining memory will be impossible 
		if (currentChunk->size - totalSize <= sizeof(Header))
		{
			//Increase allocation size instead of creating a new Chunk 
			totalSize = currentChunk->size;
			if (previousChunk != nullptr)
				previousChunk->nextPtr = currentChunk->nextPtr;
			else
				chunkPtr = currentChunk->nextPtr;
		}
		else
		{
			//Else create a new Chunk containing remaining memory 
			auto* nextChunk = static_cast<Chunk*>(Push(currentChunk, totalSize));
			nextChunk->size = nextChunk->size - totalSize;
			nextChunk->nextPtr = currentChunk->nextPtr;

			if (previousChunk != nullptr)
				previousChunk->nextPtr = nextChunk;
			else
				chunkPtr = nextChunk;
		}

		UPtr alignedPtr = reinterpret_cast<UPtr>(currentChunk) + adjustment;
		Header* header = reinterpret_cast<Header*>(alignedPtr - sizeof(Header));
		header->size = totalSize;
		header->adjustment = adjustment;
		usedMemory += totalSize;
		numAllocations++;

		assert(AlignForwardAdjustment(reinterpret_cast<void*>(alignedPtr), alignment) == 0);

		return reinterpret_cast<void*>(alignedPtr);
	}
	//No free memory chunk ;(
	return nullptr;
}

void Meltdown::Memory::ChunkListAllocator::Deallocate(void* ptr)
{
	assert(ptr != nullptr);
	Header * header = static_cast<Header*>(Pull(ptr, sizeof(Header)));
	UPtr	chunkStart = reinterpret_cast<UPtr>(ptr) - header->adjustment;
	uint32_t	chunkSize = header->size;
	UPtr	chunkEnd = chunkStart + chunkSize;
	Chunk * previousChunk = nullptr;
	Chunk * currentChunk = chunkPtr;

	while (currentChunk != nullptr)
	{
		if (reinterpret_cast<UPtr>(currentChunk) >= chunkEnd)
			break;
		//Go backward trough chain
		previousChunk = currentChunk;
		currentChunk = currentChunk->nextPtr;
	}

	if (previousChunk == nullptr)
	{
		previousChunk = reinterpret_cast<Chunk*>(chunkStart);
		previousChunk->size = chunkSize;
		previousChunk->nextPtr = chunkPtr;
		chunkPtr = previousChunk;
	}
	else if (reinterpret_cast<UPtr>(previousChunk) + previousChunk->size == chunkSize)
	{
		previousChunk->size += chunkSize;
	}
	else
	{
		auto* tempChunk = reinterpret_cast<Chunk*>(chunkStart);
		tempChunk->size = chunkSize;
		tempChunk->nextPtr = previousChunk->nextPtr;
		previousChunk->nextPtr = tempChunk;
		previousChunk = tempChunk;
	}

	if (currentChunk != nullptr && reinterpret_cast<UPtr>(currentChunk) == chunkEnd)
	{
		previousChunk->size += currentChunk->size;
		previousChunk->nextPtr = currentChunk->nextPtr;
	}

	numAllocations--;
	usedMemory -= chunkSize;
}
