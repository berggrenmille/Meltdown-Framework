
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
			static uint32_t GetId()
			{
				uint32_t id;
				if (freeIds.empty())
					id = counter++;
				else 			//Get free id
				{
					id = freeIds.back();
					freeIds.pop_back();
				}
				return id;
			}
			static void FreeId(uint32_t id)
			{
				freeIds.push_back(id);
			}
		private:
			static std::list<uint32_t> freeIds;
			static uint32_t counter;
		};

		/// <summary>
		/// TypeIdFactory generates an unique ID for each type U. Class T represents the domain. 
		/// Id's generated for types within the same domain are guaranteed to have unique ids.
		/// For example you could generate type id from components within the domain of. 
		/// </summary>
		template <typename Domain>
		class TypeIdFactory
		{
		public:
			template<typename T>
			static uint32_t GetId()
			{
				static const uint32_t id = counter++;
				return id;
			};
			/// <summary>
			/// GetFlag returns a unique flag within the domain, i.e. an unique id counted in powers of 2.
			/// </summary>
			template<typename T>
			static uint32_t GetFlag()
			{
				static const uint32_t id = TypeIdFactory<Domain>::template GetId<T>();
				static const uint32_t flag = 1ULL<<id; //Return id in powers of 2 so that it can represent a flag
				return flag;
			};
		private:
			static uint32_t counter;
		};

		//Initialization of variables
		template<typename T>
		std::list<uint32_t> IdFactory<T>::freeIds = {};
		template<typename T>
		uint32_t TypeIdFactory<T>::counter = 0;
		template<typename T>
		uint32_t IdFactory<T>::counter = 0;
	}
}