#pragma once
#include "BaseAllocator.h"
#include "MemoryHelper.hpp"
#include <iostream>

namespace Meltdown
{
	template<typename T>
	class CustomVector
	{
	private:
		T** elements = nullptr;
		unsigned int numElements = 0;
		unsigned int numAllocated = 0;
		unsigned int reserve = 0;
		Memory::BaseAllocator& allocator;
	public:
		CustomVector(Memory::BaseAllocator& alloc, unsigned int reserve);
		void push_back(T value);
	};

	template <typename T>
	CustomVector<T>::CustomVector(Memory::BaseAllocator& alloc, const unsigned int reserve)
		: elements(nullptr), numAllocated(reserve), reserve(reserve), allocator(alloc)
	{
		elements = Memory::allocateArrayNoConstructor<T*>(alloc,8);
		elements[0] = Memory::allocateArrayNoConstructor<T>(alloc, reserve);
	}

	template <typename T>
	void CustomVector<T>::push_back(T value)
	{
		unsigned int temp = 0;
		for (auto i = 0; i < 8; ++i)
		{
			const auto maxSize = sizeof(T) * pow(reserve, i + 1);
			temp += maxSize;
			if(sizeof(elements[i]) + sizeof(T) < maxSize)
			{
				elements[i][sizeof(elements[i]) / sizeof(T)] = value;
				numElements++;
				return;
			}
			if(i == 7)
			{
				std::cout << "CustomVector overflow!";
				return;
			}
			elements[i + 1] = Memory::allocateArrayNoConstructor<T>(allocator, sizeof(T) * pow(reserve, i + 2));					
		}
	}
}

