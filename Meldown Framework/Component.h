#pragma once
#include <type_traits>
namespace Meltdown
{
	namespace ECS
	{
		template<typename T>
		class ComponentHandle
		{
		public:
			explicit ComponentHandle(T* ptr):componentPtr(ptr){}
			T* GetRaw() const;
			T* GetNext() const;
		private:
			friend class ECSManager;
			T* componentPtr = nullptr;
			ComponentHandle<T>* next = nullptr;
		};

		template <typename T>
		T* ComponentHandle<T>::GetRaw() const
		{
			return componentPtr;
		}

		template <typename T>
		T* ComponentHandle<T>::GetNext() const
		{
			return next;
		}
	}
}