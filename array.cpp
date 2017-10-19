#include<initializer_list>
#include<memory>
#include<stdexcept>
#include<utility>
#define _STD ::std::
#define _NOEXCEPT noexcept

template<typename _Ty,size_t _Size>
class array
{
	
public:

	using value_type = _Ty;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = _Ty *;
	using const_pointer = const _Ty *;
	using reference = _Ty&;
	using const_reference = const _Ty&;
	
	using iterator = _Ty*;
	using const_iterator = const _Ty *;

	_Ty _elem[_Size];

public:

	void assign(const _Ty& _Val)
	{
		for (int i = 0; i != _Size; ++i)
		{
			_elem[i] = _Val;
		}
	}

	void fill(const _Ty& _Val)
	{
		for (int i = 0; i != _Size; ++i)
		{
			_elem[i] = _Val;
		}
	}

	void swap(array& _Right)
	{
		for (int i = 0; i != _Size; ++i)
		{
			_Ty temp = _elem[i];
			_elem[i] = _Right._elem[i];
			_Right._elem[i] = temp;
		}
	}

	iterator beign()
	{
		return &_elem[0];
	}

	const_iterator begin()const
	{
		return const &_elem[0];
	}

	const_iterator cbegin()const
	{
		return (begin());
	}

	iterator end()
	{
		return &_elem[_Size];
	}

	const_iterator end()const
	{
		return const &_elem[_Size];
	}

	const_iterator cend()const
	{
		return (end());
	}

	constexpr size_type size()const
	{
		return _Size;
	}

	constexpr size_type max_size()const
	{
		return _Size;
	}






};