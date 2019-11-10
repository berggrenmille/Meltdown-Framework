
#pragma once
#include <list>
namespace Meltdown
{
	namespace Util
	{
		template <typename T>
		class IdFactory
		{
		public:
			static std::size_t GetId()
			{
				std::size_t id;
				if (freeIds.size() == 0)
					id = counter++;
				else 			//Get free id
				{
					id = freeIds.back();
					freeIds.pop_back();
				}
				return id;
			}
			static void FreeId(std::size_t id)
			{
				freeIds.push_back(id);
			}
		private:
			static std::list<std::size_t> freeIds;
			static std::size_t counter;
		};

		/// <summary>
		/// TypeIdFactory generates an unique ID for each type U. Class T represents the domain. 
		/// Id's generated for types within the same domain are guaranteed to have unique ids.
		/// For example you could generate type id from components within the domain of BaseComponent. 
		/// </summary>
		template <typename Domain>
		class TypeIdFactory
		{
		public:
			template<typename T>
			static std::size_t GetId()
			{
				static const std::size_t id = counter++;
				return id;
			};
			/// <summary>
			/// GetFlag returns a unique flag within the domain, i.e. an unique id counted in powers of 2.
			/// </summary>
			template<typename T>
			static std::size_t GetFlag()
			{
				static const std::size_t id = TypeIdFactory<Domain>::template GetId<T>();
				static const std::size_t flag = exp2(id); //Return id in powers of 2 so that it can represent a flag
				return flag;
			};
		private:
			static std::size_t counter;
		};

		//Initialization of variables
		template<typename T>
		std::list<std::size_t> IdFactory<T>::freeIds = {};
		template<typename T>
		std::size_t TypeIdFactory<T>::counter = 0;
		template<typename T>
		std::size_t IdFactory<T>::counter = 0;
	}
}