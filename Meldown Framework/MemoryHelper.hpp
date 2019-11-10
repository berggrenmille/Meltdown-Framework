#pragma once
#include "BaseAllocator.h"
#include <functional>
#include <cassert>

namespace Meltdown
{
	///-------------------------------------------------------------------------------------------------
	/// @brief Memory contains all classes and functions relating to Meltdown's custom memory management.
	/// 	   The Memory namespace has global helper functions to help instantiate new objects easier.
	/// @see allocateNew()
	/// @remarks
	/// >    The Memory namespace is mainly based on Tiago Costa's post "C++: Custom memory allocation".\n
	///	>	 published: 15/04/2013, GameDev.net\n
	///	>	 https://www.gamedev.net/articles/programming/general-and-gameplay-programming/c-custom-memory-allocation-r3010/

	namespace Memory
	{

		/*
		 * Ptr align methods
		 */
		inline void* AlignForward(void* address, U8 alignment)
		{
			return reinterpret_cast<void*>((reinterpret_cast<UPtr>(address) + static_cast<UPtr>(alignment - 1)) & static_cast<UPtr>(~(
				alignment - 1)));
		}
		inline const void* AlignForward(const void* address, U8 alignment)
		{
			return reinterpret_cast<void*>((reinterpret_cast<UPtr>(address) + static_cast<UPtr>(alignment - 1)) & static_cast<UPtr>(~(
				alignment - 1)));
		}
		inline U8 AlignForwardAdjustment(const void* address, U8 alignment)
		{
			const U8 adjustment = alignment - (reinterpret_cast<UPtr>(address) & static_cast<UPtr>(alignment - 1));

			if (adjustment == alignment)
				return 0; //already aligned

			return adjustment;
		}
		inline U8 AlignForwardAdjustmentWithHeader(const void* address, U8 alignment, U8 headerSize)
		{
			U8 adjustment = AlignForwardAdjustment(address, alignment);
			U8 neededSpace = headerSize;

			if (adjustment < neededSpace)
			{
				neededSpace -= adjustment;

				//Increase adjustment to fit header 
				adjustment += alignment * (neededSpace / alignment);

				if (neededSpace % alignment > 0) adjustment += alignment;
			}

			return adjustment;
		}

		//Push ptr forward in memory
		inline void* Push(void* ptr, size_t amount)
		{
			return reinterpret_cast<void*>(reinterpret_cast<UPtr>(ptr) + amount);
		}
		//Pull ptr backwards in memory
		inline void * Pull(void* ptr, size_t amount)
		{
			return reinterpret_cast<void*>(reinterpret_cast<UPtr>(ptr) - amount);
		}

		

		///-------------------------------------------------------------------------------------------------
		/// @brief Allocates a new T from an allocator.
		/// 	   AllocateNew is much faster than using the "new" operator.
		/// 	   Performance depends on what allocator is being used.
		/// @tparam	T    Type to be instantiated.
		/// @tparam	Args The types of the constructor argument (Implicit).
		/// @param [in]	allocator The allocator to Allocate from.
		/// @param args	  The constructor arguments.
		///
		/// @returns Null if it fails, else a pointer to the new T.
		template <typename T, typename... Args>
		T* AllocateNew(Meltdown::Memory::BaseAllocator& allocator, Args&& ... args)
		{
			return new (allocator.Allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
		}

		template <typename T>
		T* AllocateNewCopy(Meltdown::Memory::BaseAllocator& allocator, const T& t)
		{
			return new (allocator.Allocate(sizeof(T), alignof(T))) T(t);
		}

		template <class T>
		void DeallocateDelete(Meltdown::Memory::BaseAllocator& allocator, T& object)
		{
			object.~T();
			allocator.Deallocate(&object);
		}

		template <class T, typename... Args>
		T* AllocateArray(Meltdown::Memory::BaseAllocator& allocator, size_t length, Args&& ... args)
		{
			assert(length != 0);
			U8 headerSize = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0) headerSize += 1;

			//Allocate extra space to store array length in the bytes before the array 
			T * p = reinterpret_cast<T*>(allocator.Allocate(sizeof(T) * (length + headerSize), __alignof(T))) + headerSize;
			*(reinterpret_cast<size_t*>(p) - 1) = length;

			for (size_t i = 0; i < length; i++)
				new (&p[i]) T(std::forward<Args>(args)...);

			return p;
		}

		template <class T>
		T* AllocateArrayNoConstructor(Meltdown::Memory::BaseAllocator & allocator, size_t length)
		{
			assert(length != 0);

			U8 header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			//Allocate extra space to store array length in the bytes before the array
			T * p = reinterpret_cast<T*>(allocator.Allocate(sizeof(T) * (length + header_size), __alignof(T))) + header_size;

			*(reinterpret_cast<size_t*>(p) - 1) = length;

			return p;
		}

		template <class T>
		void DeallocateArray(Meltdown::Memory::BaseAllocator & allocator, T * arr)
		{
			assert(arr != nullptr);

			const size_t length = *(reinterpret_cast<size_t*>(arr) - 1);

			//should be reverse order
			for (size_t i = 0; i < length; i++)
				arr[i].~T();

			//Calculate how much extra memory was allocated to store the length before the array
			U8 header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			allocator.Deallocate(arr - header_size);
		}

		template <class T>
		void DeallocateArrayNoDestructor(Meltdown::Memory::BaseAllocator & allocator, T * arr)
		{
			assert(arr != nullptr);

			//Calculate how much extra memory was allocated to store the length before the array
			U8 header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			allocator.Deallocate(arr - header_size);
		}	

	}
}
