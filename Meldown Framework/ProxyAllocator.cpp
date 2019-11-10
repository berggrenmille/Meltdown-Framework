#include "ProxyAllocator.h"
#include <iostream>
#include <cassert>

Meltdown::Memory::ProxyAllocator::ProxyAllocator(BaseAllocator& allocator)
	:	BaseAllocator(allocator.GetSize(), allocator.GetStart()), m_targetAllocator(allocator)
{
	//I hate not using default initializers...
}

Meltdown::Memory::ProxyAllocator::~ProxyAllocator()
{
	//Nothing here
}

void* Meltdown::Memory::ProxyAllocator::Allocate(size_t size, U8 alignment)
{
	assert(size != 0);
	numAllocations++;
	//used memory before allocation
	size_t previousMemory = m_targetAllocator.GetUsedMemory();

	void* ptr = m_targetAllocator.Allocate(size, alignment);

	//used memory after allocation
	usedMemory += m_targetAllocator.GetUsedMemory() - previousMemory;

	///TODO: Log amount that got allocated
	///
#ifdef _DEBUG
	std::cout << "Allocated: " << (m_targetAllocator.GetUsedMemory() - previousMemory) << " B - For a total of: " << usedMemory << " B" << std::endl;
#endif


	return ptr;
}

void Meltdown::Memory::ProxyAllocator::Deallocate(void* ptr)
{
	numAllocations--;
	//used memory before deallocation
	size_t previousMemory = m_targetAllocator.GetUsedMemory();

	m_targetAllocator.Deallocate(ptr);

	//used memory after deallocation
	usedMemory += m_targetAllocator.GetUsedMemory() - previousMemory;

	///TODO: Log amount that got deallocated
}
