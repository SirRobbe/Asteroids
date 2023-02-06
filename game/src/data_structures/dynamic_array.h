#pragma once

#include "../platform/types.h"
// TODO: Do custom assert
#include <cassert>

namespace ks
{
	template<typename T>
	class DynamicArray
	{
	public:
		DynamicArray(uint64 capacity) :
			m_Elements(new T[capacity]),
			m_Capacity(capacity),
			m_Count(0)
		{}

		void Add(const T& item)
		{
			if(m_Count == m_Capacity)
			{
				T* tmp = m_Elements;
				m_Capacity *= 2;
				m_Elements = new T[m_Capacity];
				for(int64 i = 0; i < m_Count; i++)
				{
					m_Elements[i] = tmp[i];
				}

				delete[] tmp;
			}

			m_Elements[m_Count] = item;
			m_Count++;
		}

		void UnorderedRemove(uint64 index)
		{
			assert(index < m_Count);
			m_Elements[index] = m_Elements[m_Count - 1];
			m_Count--;
		}

		void Clear()
		{
			m_Count = 0;
		}

		uint64 Count() const { return m_Count; }

		T& operator[](uint64 index)
		{
			assert(index < m_Count);
			return m_Elements[index];
		}

		const T& operator[](uint64 index) const
		{
			assert(index < m_Count);
			return m_Elements[index];
		}


	private:
		T* m_Elements;
		uint64 m_Capacity;
		uint64 m_Count;
	};
}