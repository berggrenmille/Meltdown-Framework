#pragma once
#include <cstdint>

namespace Meltdown
{
	namespace Memory
	{
		using U8 = uint8_t;
		using UPtr = uintptr_t;
		constexpr U8 NORMAL_ALIGNMENT = 8;
		///-------------------------------------------------------------------------------------------------
		/// @brief All custom allocators must derive from BaseAllocator.

		class BaseAllocator
		{
		public:
			BaseAllocator(size_t dedicatedMemory, void* startPtr)
			{
				this->startPtr = startPtr;
				dedicatedSize = dedicatedMemory;
				usedMemory = 0;
				numAllocations = 0;
			}

			virtual ~BaseAllocator()
			{
				startPtr = nullptr;
				dedicatedSize = 0;
			}

			///-------------------------------------------------------------------------------------------------
			/// @brief Allocates a certain amounts of byte in memory.
			///
			/// @param	size	  The requested size of the allocation.
			/// @param	alignment (Optional) The requested bit alignment of the allocation.
			///
			/// @returns nullptr if it fails, else a pointer to allocated memory.
			/// @warning Be cautious about allocating while the allocator is full, without proper code it will result in nullreference exceptions. Always have a bit of extra memory available.

			virtual void* Allocate(size_t size, U8 alignment = NORMAL_ALIGNMENT) = 0;

			///-------------------------------------------------------------------------------------------------
			/// @brief Deallocates the space occupied by given pointer
			///	@warning It is not always certain if a derived allocator is able to Deallocate memory.
			/// @param [in]	ptr Pointer to an allocated part in memory.

			virtual void Deallocate(void* ptr) = 0;

			///-------------------------------------------------------------------------------------------------
			/// @brief Get the pointer to the start of allocated memory.
			///
			/// @returns The start pointer.

			void* GetStart() const { return startPtr; }

			///-------------------------------------------------------------------------------------------------
			/// @brief Gets the total size allocated at allocator creation.
			///
			/// @returns The amount of dedicated memory in bytes.

			size_t GetSize() const { return  dedicatedSize; }

			///-------------------------------------------------------------------------------------------------
			/// @brief Gets how much of allocated memory is occupied.
			///
			/// @returns The total amount of used memory.

			size_t GetUsedMemory() const { return usedMemory; }

			///-------------------------------------------------------------------------------------------------
			/// @brief Gets the number of allocations made on This allocator
			///
			/// @returns The number of allocations.

			size_t GetNumAllocations() const { return numAllocations; }

		protected:
			void* startPtr;
			size_t dedicatedSize;
			size_t usedMemory;
			size_t numAllocations;
			bool allowDeallocate = false;
		};
	}
}
