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
			T* GetNextRaw() const;
		private:
			friend class ECSManager;
			T* componentPtr = nullptr;
			T* next = nullptr;
		};

		template <typename T>
		T* ComponentHandle<T>::GetRaw() const
		{
			return componentPtr;
		}

		template <typename T>
		T* ComponentHandle<T>::GetNextRaw() const
		{
			return next;
		}
	}
}