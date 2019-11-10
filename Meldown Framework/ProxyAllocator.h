#pragma once
#include "BaseAllocator.h"
namespace Meltdown
{
	namespace Memory
	{
		/*
		 * NOTICE
		 * The memory namespace is based on Tiago Costa's post "C++: Custom memory allocation"
		 * published: 15/04/2013, GameDev.net
		 * https://www.gamedev.net/articles/programming/general-and-gameplay-programming/c-custom-memory-allocation-r3010/
		 */
		/*
		 * Proxy Allocator is a form of allocator that redirect the allocation to another Allocator while keeping track of the memory used
		 * Great for debugging, Logging and finding memory leaks
		 */
		class ProxyAllocator : public BaseAllocator
		{
		public:

			ProxyAllocator(BaseAllocator& allocator);
			~ProxyAllocator();
			void* Allocate(size_t size, U8 alignment) override;
			void Deallocate(void* ptr) override;

			//NO COPIES
			ProxyAllocator(const ProxyAllocator&) = delete;
			ProxyAllocator& operator=(const ProxyAllocator&) = delete;
		private:
			BaseAllocator& m_targetAllocator;
		};
	}
}
