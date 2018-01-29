#ifndef DEQUE_HPP
#define DEQUE_HPP

#include<memory>
#include<utility>
#include<iterator>

#define _STD ::std::
constexpr int DEFAULT_SIZE = 16;


template<typename _Deque_Base_Type>
class _deque_const_iterator
{
public:
	using deque_type = _Deque_Base_Type;
	using value_type = const typename deque_type::value_type;
	using pointer = typename deque_type::pointer;
	using reference = const value_type&;
	using size_type = typename deque_type::size_type;
	using difference_type = typename deque_type::difference_type;

	_deque_const_iterator()
		:deque_(nullptr), cur_(0)
	{   }

	_deque_const_iterator(deque_type* deque, size_type position)
		:deque_(deque), cur_(position)
	{   }

	_deque_const_iterator& operator=(const _deque_const_iterator& right)
	{
		deque_ = right.deque_;
		cur_ = right.cur_;
		return *this;
	}

	_deque_const_iterator& operator=(_deque_const_iterator&& right)
	{
		deque_ = _STD move(right.deque_);
		cur_ = _STD move(right.cur_);
		return *this;
	}

	reference operator*() const
	{
		return (deque_->_iterator_get_element(cur_));
	}

	pointer operator->() const
	{
		return (_STD pointer_traits<pointer>::pointer_to(**this));
	}

	_deque_const_iterator& operator++()
	{
		++cur_;
		return *this;
	}

	_deque_const_iterator operator++(int)
	{
		_deque_const_iterator ret(*this);
		++(*this);
		return ret;
	}

	_deque_const_iterator& operator--()
	{
		--cur_;
		return *this;
	}

	_deque_const_iterator operator--(int)
	{
		_deque_const_iterator ret(*this);
		--(*this);
		return *this;
	}

	_deque_const_iterator& operator+=(difference_type n)
	{
		cur_ += n;
		return *this;
	}

	_deque_const_iterator operator+(difference_type n) const
	{
		_deque_const_iterator ret(*this);
		return (ret += n);
	}

	_deque_const_iterator& operator-=(difference_type n)
	{
		cur_ -= n;
		return *this;
	}

	_deque_const_iterator operator-(difference_type n) const
	{
		_deque_const_iterator ret(*this);
		return (ret -= n);
	}

	difference_type operator-(const _deque_const_iterator& right) const
	{
		return (static_cast<difference_type>(cur_ - right.cur_));
	}

	reference operator[](difference_type offset) const
	{
		return (*(*this + offset));
	}

	bool operator==(const _deque_const_iterator& right) const
	{
		return ((deque_ == right.deque_) && (cur_ == right.cur_));
	}

	bool operator!=(const _deque_const_iterator& right) const
	{
		return (!(*this == right));
	}

	bool operator<(const _deque_const_iterator& right) const
	{
		if (deque_ == right.deque_)
			return (cur_ < right.cur_);
		else
		{
			//TODO
		}
	}

	bool operator>(const _deque_const_iterator& right) const
	{
		return (right < *this);
	}

	bool operator>=(const _deque_const_iterator& right) const
	{
		return (!(*this < right));
	}

	bool operator<=(const _deque_const_iterator& right) const
	{
		return (!(*this > right));
	}


	deque_type * deque_;
	int cur_;
};

template<typename _Deque_Base_Type>
inline _deque_const_iterator<_Deque_Base_Type> operator+(
	typename _deque_const_iterator<_Deque_Base_Type>::difference_type n,
	_deque_const_iterator<_Deque_Base_Type>& right_arg)
{
	return (right_arg + n);
}

template<typename _Deque_Base_Type>
class _deque_iterator :public _deque_const_iterator<_Deque_Base_Type>
{
	using base = _deque_const_iterator<_Deque_Base_Type>;
public:
	using value_type = typename deque_type::value_type;

	using _deque_const_iterator::_deque_const_iterator;

	_deque_iterator& operator=(const _deque_iterator& right)
	{
		base* l = static_cast<base*>(left);
		base* r = static_cast<base*>(&right);
		(*l) = (*r);
		return (*this);
	}

	reference operator*() const
	{
		return (deque_->_iterator_get_element(cur_));
	}

	pointer operator->() const
	{
		return (_STD pointer_traits<pointer>::pointer_to(**this));
	}

	_deque_iterator& operator++()
	{
		++*(static_cast<base*>(this));
		return (*this);
	}

	_deque_iterator operator++(int)
	{
		_deque_iterator ret(*this);
		++*this;
		return ret;
	}

	_deque_iterator& operator--()
	{
		--*(static_cast<base*>(this));
		return (*this);
	}

	_deque_iterator operator--(int)
	{
		_deque_iterator ret(*this);
		--(*this);
		return ret;
	}

	_deque_iterator& operator+=(difference_type n)
	{
		*(static_cast<base*>(this)) += n;
		return (*this);
	}

	_deque_iterator operator+(difference_type n) const
	{
		_deque_iterator ret(*this);
		return (ret += n);
	}

	_deque_iterator& operator-=(difference_type n)
	{
		*(static_cast<base*>(this)) -= n;
		return (*this);
	}

	_deque_iterator operator-(difference_type n) const
	{
		_deque_iterator ret(*this);
		return (ret -= n);
	}

	reference operator[](difference_type offset) const
	{
		return (*(*this + offset));
	}
};

template<typename _Deque_Base_Type>
inline _deque_iterator<_Deque_Base_Type> operator+(
	typename _deque_iterator<_Deque_Base_Type>::difference_type n,
	_deque_iterator<_Deque_Base_Type> right_arg)
{
	return (right_arg + n);
}



template<typename T, typename Allocator = _STD allocator<T>>
class _deque_base
{
public:
	using value_type = T;
	using pointer = value_type * ;
	using d_pointer = value_type * *;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using allocator_type = Allocator;
	using size_type = unsigned int;
	using iterator = _deque_iterator<_deque_base>;
	using const_iterator = _deque_const_iterator<_deque_base>;
	using difference_type = _STD ptrdiff_t;

	static constexpr int group_size = sizeof(T) <= 8 ?
		DEFAULT_SIZE : (DEFAULT_SIZE / 2); //16 or 8

	_deque_base() :_deque_base(allocator_type())
	{   }

	explicit _deque_base(const allocator_type& alloc)
		:group_(new pointer[4](nullptr)), first_(group_size), last_(group_size),
		size_(4 * group_size), alloc_(alloc)
	{   }

	_deque_base(size_type count, const T&value, const allocator_type&alloc = allocator_type())
		:alloc_(alloc), first_(group_size), last_(group_size)
	{
		set_size(count);
		for (; count != 0; --count)
			emplace_back(value);
	}

	explicit _deque_base(size_type count, const allocator_type& alloc = allocator_type())
		:_deque_base(count, value_type(), alloc)
	{   }

	template<typename InputIt>
	_deque_base(InputIt first, InputIt last, const allocator_type& alloc = allocator_type())
		: alloc_(alloc)
	{
		difference_type diff = static_cast<difference_type>(_STD advance(first, last));
		set_size(static_cast<size_type>(diff));
		first_ = group_size;

		for (; first != last; ++first)
			emplace_back(*first);
	}

	_deque_base(_STD initializer_list<value_type> il, const allocator_type& alloc = allocator_type())
		:_deque_base(il.begin(), il.end(), alloc)
	{   }

	_deque_base(const _deque_base& right)
		:_deque_base(right, right.alloc_)
	{   }

	_deque_base(const _deque_base& right, const allocator_type& alloc)
		:alloc_(alloc)
	{
		size_ = right.size_;
		group_ = new pointer[size_ / group_size](nullptr);
		first_ = group_size;
		last_ = group_size;
		size_type right_first = right._iterator_first();
		size_type right_last = right._iterator_last();
		for (; right_first != right_last; ++right_first)
			emplace_back(right._iterator_get_element(right_first));
	}

	_deque_base(_deque_base&& right)
		:group_(right.group_), first_(right.first_), last_(right.last_), size_(right.size_),
		alloc_(_STD move(right.alloc_))
	{   }

	_deque_base(_deque_base&& right, const allocator_type& alloc)
		:group_(right.group_), first_(right.first_), last_(right.last_), size_(right.size_),
		alloc_(alloc)
	{   }

	~_deque_base()
	{
		size_type count = size_ / group_size;
		for (size_type i = 0; i != count; ++i)
			if (group_[i] != nullptr)
				alloc_.deallocate(group_[i], group_size);
		delete[] group_;
	}


	size_type _iterator_first() const
	{
		return first_;
	}

	size_type _iterator_last() const
	{
		return last_;
	}

	reference _iterator_get_element(size_type index) const
	{
		if (index < first_ || index >= last_)
			throw _STD out_of_range{ "deque out of range access" };

		return group_[index / group_size][index%group_size];
	}

	template<typename...Args>
	void emplace_back(Args&&...args)
	{
		if (last_ > size_)
			resize();
		if (last_%group_size == 0)
			group_[last_ / group_size] = alloc_.allocate(group_size);

		alloc_.construct(relocate(last_), _STD forward<Args>(args)...);
		++last_;
	}



private:

	void set_size(size_type count)
	{
		for (size_ = 4 * group_size; size_ < (count + group_size);)
			size_ *= 2;
	}

	void resize()
	{
		size_ *= 2;
		d_pointer new_group = new pointer[size_ / group_size](nullptr);
		size_type cur = 1; //start from group_size
		for (size_type temp = first_; temp < last_; temp += group_size)
		{
			new_group[cur] = group_[temp / group_size];
			++cur;
		}
		last_ = (last_first_)+group_size;
		first_ = group_size;
		delete[] group_;
		group_ = new_group;
	}

	pointer relocate(size_type position) const
	{
		return group_[position / group_size][position%group_size];
	}

	d_pointer group_;
	size_type first_;
	size_type last_;
	size_type size_;
	allocator_type alloc_;
};






#endif // !DEQUE_HPP
