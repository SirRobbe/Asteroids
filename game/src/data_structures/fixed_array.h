#pragma once


namespace ks
{
	// INCOMPLETE: This array is currently allocated on the heap
	// but I would rather prefer to allocate it on the stack,
	// however then i can´t pass arrays with unknown size as parameter
	// if I ever find a satisfying solutio to this problem change this implementation
	template<typename T>
	class FixedArray
	{
	public:
		FixedArray(size_t size)
		{
			Elements = new T[size];
			Count = size;
		}

		~FixedArray()
		{
			delete[] Elements;
		}

		T& operator[](size_t index)
		{
			return Elements[index];
		}

		const T& operator[](size_t index) const
		{
			return Elements[index];
		}

		T* Data()
		{
			return &Elements[0];
		}

		size_t Count;

	private:
		T* Elements;
	};
}