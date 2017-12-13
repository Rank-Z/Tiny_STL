#include<initializer_list>
#include<memory>
#include<stdexcept>
#include<utility>
#include<xutility>
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


	void assign(const _Ty& _Val)
	{
		for (int i = 0; i != _Size; ++i)
		{
			_Elems[i] = _Val;
		}
	}

	void fill(const _Ty& _Val)
	{
		for (int i = 0; i != _Size; ++i)
		{
			_Elems[i] = _Val;
		}
	}

	void swap(array& _Right)
	{
		for (int i = 0; i != _Size; ++i)
		{
			_Ty temp = _Elems[i];
			_Elems[i] = _Right._Elems[i];
			_Right._Elems[i] = temp;
		}
	}

	iterator beign() _NOEXCEPT
	{
		return &_Elems[0];
	}

	const_iterator begin() const _NOEXCEPT
	{
		return &_Elems[0];
	}

	const_iterator cbegin() const _NOEXCEPT
	{
		return (begin());
	}

	iterator end() _NOEXCEPT
	{
		return &_Elems[_Size];
	}

	const_iterator end() const _NOEXCEPT
	{
		return &_Elems[_Size];
	}

	const_iterator cend() const _NOEXCEPT
	{
		return (end());
	}

	constexpr size_type size() const _NOEXCEPT
	{
		return _Size;
	}

	constexpr size_type max_size() const _NOEXCEPT
	{
		return _Size;
	}

	constexpr bool empty() const _NOEXCEPT
	{
		return false;
	}

	reference at(size_type _Pos)
	{
		if (_Size <= _Pos)
			throw _STD out_of_range("invalid array<T, N> subscript");

		return _Elems[_Pos];
	}

	constexpr const_reference at(size_type _Pos) const
	{
		if (_Size <= _Pos)
			throw _STD out_of_range("invalid array<T, N> subscript");
		return _Elems[_Pos];
	}

	reference operator[](size_type _Pos)
	{
		if (_Size <= _Pos)
			throw _STD out_of_range("array subscript out of range");
		return _Elems[_Pos];
	}

	constexpr const_reference operator[](size_type _Pos) const
	{
		if (_Size <= _Pos)
			throw _STD out_of_range("array subscript out of range");
		return _Elems[_Pos];
	}

	reference front()
	{
		return _Elems[0];
	}

	constexpr const_reference front() const
	{
		return _Elems[0];
	}

	reference back()
	{
		return _Elems[_Size-1];
	}

	constexpr const_reference back() const
	{
		return _Elems[_Size-1];
	}

	_Ty* data() _NOEXCEPT
	{
		return _Elems;
	}

	const _Ty* data() _NOEXCEPT
	{
		return _Elems;
	}

public:
	_Ty _Elems[_Size];
};

template<typename _Ty>
class array<_Ty, 0>
{
public:
	using value_type = _Ty;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = _Ty *;
	using const_pointer = const _Ty *;
	using reference = _Ty&;
	using const_reference = const _Ty&;

	using iterator = _STD _Array_iterator<_Ty,0>;
	using const_iterator =_STD _Array_const_iterator<_Ty,0>;

	void assign(const _Ty&)
	{   }

	void fill(const _Ty&)
	{   }

	void swap(array&) _NOEXCEPT
	{   }

	iterator begin()
	{
		return iterator(0,0);
	}

	const_iterator begin() const
	{
		return iterator(0, 0);
	}

	const_iterator cbegin() const
	{
		return const_iterator(0,0);
	}

	iterator end()
	{
		return iterator(0, 0);
	}

	const_iterator end() const
	{
		return iterator(0, 0);
	}

	const_iterator cend() const
	{
		return const_iterator(0, 0);
	}

	constexpr size_type size() const
	{
		return 0;
	}

	constexpr size_type max_size() const
	{
		return 0;
	}

	constexpr bool empty() const
	{
		return true;
	}

	reference at(size_type)
	{
		throw _STD out_of_range("invalid array<T,0> subscript");
	}

	const_reference at(size_type)
	{
		throw _STD out_of_range("invalid array<T,0> subscript");
	}

	reference operator[](size_type)
	{
		return _Elems[0];
	}

	const_reference operator[](size_type)
	{
		return _Elems[0];
	}

	reference front()
	{
		return _Elems[0];
	}

	const_reference front() const
	{
		return _Elems[0];
	}

	reference back()
	{
		return _Elems[0];
	}

	const_reference back() const
	{
		return _Elems[0];
	}

	_Ty* data()
	{
		return nullptr;
	}

	const _Ty* data() const
	{
		return nullptr;
	}

public:
	_Ty _Elems[1];
};