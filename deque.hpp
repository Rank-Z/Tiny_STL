#ifndef DEQUE_HPP
#define DEQUE_HPP

#include<memory>
#include<utility>
#include<iterator>
#include<algorithm>

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
	size_type cur_;
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
	using pointer = typename _STD allocator_traits<Allocator>::pointer;
	using const_pointer = typename _STD allocator_traits<Allocator>::const_pointer;
	using d_pointer = value_type * *;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using allocator_type = Allocator;
	using size_type = _STD size_t;
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
		init(right.size_);
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
		all_deallocate();
	}

	_deque_base& operator=(const _deque_base& right)
	{
		all_deallocate();
		alloc_ = right.alloc_;
		init(right.size_);
		size_type right_first = right._iterator_first();
		size_type right_last = right._iterator_last();
		for (; right_first != right_last; ++right_first)
			emplace_back(right._iterator_get_element(right_first));

		return *this;
	}

	_deque_base& operator=(_deque_base&& right)
		noexcept(_STD allocator_traits<allocator_type>::is_always_equal::value) //since C++17
	{
		all_deallocate();
		group_ = right.group_;
		first_ = right.first_;
		last_ = right.last_;
		size_ = right.size_;
		alloc_ = _STD move(right.alloc_);

		return *this;
	}

	_deque_base& operator=(_STD initializer_list<value_type> il)
	{
		all_deallocate();
		init(static_cast<size_type>(il.size());)
			for (auto it = il.begin(); it != il.end(); ++it)
				emplace_back(*it);

		return *this;
	}

	void assign(size_type count, const T& val)
	{
		all_deallocate();
		init(count);
		for (; count != 0; --count)
			emplace_back(val);
	}

	template<typename InputIt>
	void assign(InputIt first, InputIt last)
	{
		all_deallocate();
		init(_STD advance(first, last));
		for (; first != last; ++first)
			emplace_back(*first);
	}

	void assign(_STD initializer_list<value_type> il)
	{
		assign(il.begin(), il.end());
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

	iterator begin() noexcept
	{
		return iterator(this, first_);
	}

	const_iterator cbegin() const noexcept
	{
		return const_iterator(this, first_);
	}

	iterator end() noexcept
	{
		return iterator(this, last_);
	}

	const_iterator cend() const noexcept
	{
		return const_iterator(this, last_);
	}

	reference at(size_type index)
	{
		return _iterator_get_element(first_ + index);
	}

	const_reference at(size_type index) const
	{
		return _iterator_get_element(first_ + index);
	}

	reference operator[](size_type index)
	{
		size_type locate = first_ + index;
		return group_[locate / group_size][locate%group_size];
	}

	const_reference operator[](size_type index) const
	{
		size_type locate = first_ + index;
		return group_[locate / group_size][locate%group_size];
	}

	reference front()
	{
		return group_[first_ / group_size][first_%group_size];
	}

	const_reference front() const
	{
		return group_[first_ / group_size][first_%group_size];
	}

	reference back()
	{
		return group_[(last_ - 1) / group_size][(last_ - 1) % group_size];
	}

	const_reference back() const
	{
		return group_[(last_ - 1) / group_size][(last_ - 1) % group_size];
	}

	template<typename...Args>
	void emplace_front(Args&&...args)
	{
		if (first_ == 0)
			resize();
		--first_;
		if (first_ == (group_size - 1))
			group_[first_ / group_size] = alloc_.allocate(group_size);
		alloc_construct(relocate(first_), _STD forward<Args>(args)...);
	}

	template<typename...Args>
	void emplace_back(Args&&...args)
	{
		if (last_ > size_)
			resize();
		if (last_%group_size == 0 && group_[last_ / group_size] == nullptr)
			group_[last_ / group_size] = alloc_.allocate(group_size);

		alloc_.construct(relocate(last_), _STD forward<Args>(args)...);
		++last_;
	}

	allocator_type get_allocator() const
	{
		return alloc_;
	}

	bool empty() const
	{
		return (first_ == last_);
	}

	size_type size() const
	{
		return last_ = first_;
	}

	size_type max_size() const noexcept
	{
		return 99999; // magic number
	}

	void shrink_to_fit()
	{
		for (size_type i = 0; i != (first_ / group_size); ++i)
			if (group_[i] != nullptr)
			{
				alloc_.deallocate(group_[i], group_size);
				group_[i] = nullptr;
			}
		for (size_type i = (last_ / group_size) + 1; i<(size_ / group_size); ++i)
			if (group_[i] != nullptr)
			{
				alloc_.deallocate(group_[i], group_size);
				group_[i] = nullptr;
			}
	}

	void clear() noexcept
	{
		all_destory();
	}

	template<typename...Args>
	iterator emplace(const_iterator position, Args&&...args)
	{
		if (position.cur_ == last_)
			return emplace_back(_STD forward<Args>(args)...);
		if (position.cur_ == first_)
			return emplace_front(_STD forward<Args>(args)...);

		size_type index = position.cur_;
		emplace_black(*(--end()));
		for (size_type i = (last_ - 2); i != position.cur_; --i)
			_iterator_get_element(i - 1) = _iterator_get_element(i);
		alloc_.destroy(relocate(position.cur_));
		alloc_.construct(_STD forward<Args>(args)...);
		return iterator(this, position.cur_);
	}

	void pop_front()
	{
		alloc_.destroy(relocate(first_));
		--first_;
	}

	void pop_back()
	{
		alloc_.destroy(relocate(last_ - 1));
		--last_;
	}

	iterator erase(const_iterator position)
	{
		if (position.cur_ == first_)
		{
			pop_front();
			return iterator(this, first_);
		}
		if (position.cur_ == last_)
		{
			pop_back();
			return iterator(this, last_ - 1);
		}
		if ((position.cur_ - first_) < (last_ - position.cur_))
		{
			for (size_type i = position.cur_; i != first_; --i)
				_iterator_get_element(i - 1) = _iterator_get_element(i);
			pop_front();
			return iterator(this, position.cur_ + 1);
		}
		else
		{
			for (size_type i = position.cur_; i != (last_ - 1); ++i)
				_iterator_get_element(i) = _iterator_get_element(i + 1);
			pop_back();
			return iterator(this, position.cur_);
		}
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		size_type count = last.cur_ - first.cur_;
		if ((first.cur_ - first_) < (last_ - last.cur_))
		{
			size_type i = first.cur_ - 1;
			size_type j = last.cur_ - 1;
			for (; (count != 0) && (i >= first_); --count)
				_iterator_get_element(j--) = _iterator_get_element(i--);
			for (size_type t = last.cur_ - first.cur_; t != 0; --t)
				pop_front();
		}
		else
		{
			size_type i = first.cur_;
			size_type j = last.cur_;
			for (; (count != 0) && (j < last_); --count)
				_iterator_get_element(i++) = _iterator_get_element(j++);
			for (size_type t = last.cur_ - first.cur_; t != 0; --t)
				pop_back();
		}
	}

	void resize(size_type count)
	{
		resize(count, value_type());
	}

	void resize(size_type count, const value_type& val)
	{
		size_type num = last_ - first_;
		if (count < num)
		{
			for (size_type i = (num - count); i != 0; --i)
				pop_back();
			shrink_to_fit();
		}
		else if (count > num)
		{
			for (size_type i = (count - num); i != 0; --i)
				emplace_back(val);
		}
	}

	void swap(_deque_base& right)
	{
		_STD swap(group_, right.group_);
		_STD swap(first_, right.first_);
		_STD swap(last_, right.last_);
		_STD swap(size_, right.size_);
		_STD swap(alloc_, right.alloc_);
	}

private:

	void set_size(size_type count)
	{
		for (size_ = 4 * group_size; size_ < (count + group_size);)
			size_ *= 2;
	}

	void init(size_type count = (4 * group_size))
	{
		first_ = group_size;
		last_ = group_size;
		set_size(count);
		group_ = new pointer[size_ / group_size];
	}



	pointer relocate(size_type position) const
	{
		return &group_[position / group_size][position%group_size];
	}

	void expand()
	{
		d_pointer new_group = new pointer[(size_ * 2) / group_size];
		for (size_type i = 0; i != (size_ / group_size); ++i)
			new_group[i] = group_[i];
		size_ *= 2;
		delete[] group_;
		group_ = new_group;
	}

	void all_deallocate()
	{
		size_type count = size_ / group_size;
		for (size_type i = 0; i != count; ++i)
			if (group_[i] != nullptr)
			{
				alloc_.deallocate(group_[i], group_size);
				group_[i] = nullptr;
			}
		delete[] group_;
	}

	void all_destroy()
	{
		for (size_type temp = first_; temp != last_; ++temp)
			alloc_.destroy(&group_[temp / group_size][temp%group_size]);
		last_ = first_;
	}

	d_pointer group_;
	size_type first_;
	size_type last_;
	size_type size_;
	allocator_type alloc_;
};

template<typename T, typename Allocator = _STD allocator<T>>
class deque
{
	using base_type = _deque_base<T, Allocator>;
public:
	using value_type = T;
	using pointer = typename _STD allocator_traits<Allocator>::pointer;
	using const_pointer = typename _STD allocator_traits<Allocator>::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using allocator_type = Allocator;
	using size_type = _STD size_t;
	using iterator = _deque_iterator<base_type>;
	using const_iterator = _deque_const_iterator<base_type>;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
	using difference_type = _STD ptrdiff_t;

	deque()
		:deque(allocator_type())
	{   }

	explicit deque(const allocator_type& alloc)
		:base_(alloc)
	{   }

	deque(size_type count, const T& val, const allocator_type& alloc = allocator_type())
		:base_(count, val, alloc)
	{   }

	explicit deque(size_type count, const allocator_type& alloc = allocator_type())
		:deque(count, value_type(), alloc)
	{   }

	deque(const deque& right)
		:base_(right.base_)
	{   }

	deque(const deque& right, const allocator_type& alloc)
		:base_(right.base_, alloc)
	{   }

	deque(deque&& right)
		:base_(_STD move(right.base_))
	{   }

	deque(deque&& right, const allocator_type& alloc)
		:base_(_STD move(right.base_), alloc)
	{   }

	template<typename InputIt>
	deque(InputIt first, InputIt last, const allocator_type&alloc = allocator_type())
		: base_(first, last, alloc)
	{   }

	deque(_STD initializer_list<value_type> il, const allocator_type& alloc = allocator_type())
		:base_(il, alloc)
	{   }

	deque& operator=(const deque& right)
	{
		base_ = right.base_;
	}

	deque& operator=(deque&& right)
	{
		base_ = _STD move(right.base_);
	}

	deque& operator=(_STD initializer_list<value_type> il)
	{
		base_ = il;
	}

	void assign(size_type count, const value_type& val)
	{
		base_.assign(count, val);
	}

	template<typename InputIt>
	void assign(InputIt first, InputIt last)
	{
		base_.assign(first, last);
	}

	void assign(_STD initializer_list<value_type> il)
	{
		base_.assign(il);
	}

	allocator_type get_allocator() const
	{
		return base_.get_allocator();
	}

	reference at(size_type index)
	{
		return base_.at(index);
	}

	const_reference at(size_type index) const
	{
		return base_.at(index);
	}

	reference operator[](size_type index)
	{
		return base_[index];
	}

	const_reference operator[](size_type index)
	{
		return base_[index];
	}

	reference front()
	{
		return base_.front();
	}

	const_reference front() const
	{
		return base_.front();
	}

	reference back()
	{
		return base_.back();
	}

	const_reference back() const
	{
		return base_.back();
	}

	iterator begin() noexcept
	{
		return base_.begin();
	}

	const_iterator begin() const noexcept
	{
		return base_.cbegin();
	}

	const_iterator cbegin() const noexcept
	{
		return base_.cbegin();
	}

	iterator end() noexcept
	{
		return base_.end();
	}

	const_iterator end() const noexcept
	{
		return base_.cend();
	}

	const_iterator cend() const noexcept
	{
		returb base_.cend();
	}

	reverse_iterator rbegin() noexcept
	{
		return base_.end();
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return base_.cend();
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return base_.cend();
	}

	reverse_iterator rend() noexcept
	{
		return base_.begin();
	}

	const_reverse_iterator rend() const noexcept
	{
		return base_.cbegin();
	}

	const_reverse_iterator crend() const noexcept
	{
		return base_.cbegin();
	}

	bool empty() const
	{
		return base_.empty();
	}

	size_type size() const
	{
		return base_.size();
	}

	size_type max_size() const noexcept
	{
		return 99999; // magic number
	}

	void shrink_to_fit()
	{
		base_.shrink_to_fit();
	}

	void clear() noexcept
	{
		base_.clear();
	}

	template<typename...Args>
	void emplace_front(Args&&...args)
	{
		base_.emplace_front(_STD forward<Args>(args)...);
	}

	template<typename...Args>
	void emplace_back(Args&&...args)
	{
		base_.emplace_back(_STD forward<Args>(args)...);
	}

	template<typename...Args>
	iterator emplace(const_iterator position, Args&&...args)
	{
		return base_.emplace(position, _STD forward<Args>(args)...);
	}

	void push_front(const value_type& val)
	{
		base_.emplace_front(val);
	}

	void push_front(value_type&& val)
	{
		base_.emplace_front(_STD move(val));
	}

	void push_back(const value_type& val)
	{
		base_.emplace_back(val);
	}

	void push_back(value_type&& val)
	{
		base_.emplace_back(_STD move(val));
	}

	iterator insert(const_iterator position, const value_type& val)
	{
		return emplace(position, val);
	}

	iterator insert(const_iterator position, value_type&& val)
	{
		return emplace(position, _STD move(val));
	}

	void insert(const_iterator position, size_type count, const value_type& val)
	{
		for (; count != 0; --count)
			emplace(position, val);
	}

	template<typename InputIt>
	iterator insert(const_iterator position, InputIt first, InputIt last)
	{
		for (; first != last; ++first)
			emplace(position++, *first);
		return iterator(&base_, position.cur_);
	}

	iterator insert(const_iterator position, _STD initializer_list<value_type> il)
	{
		return insert(position, il.begin(), il.end());
	}

	iterator erase(const_iterator position)
	{
		return base_.erase(position);
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		return base_.erase(first, last);
	}

	void pop_front()
	{
		base_.pop_front();
	}

	void pop_back()
	{
		base_.pop_back();
	}

	void resize(size_type count)
	{
		base_.resize(count);
	}

	void resize(size_type count, const value_type& val)
	{
		base_.resize(count, val);
	}

	void swap(deque& right) noexcept
	{
		base_.swap(right.swap);
	}

private:
	_deque_base<T, Allocator> base_;

};

template<typename T, typename Allocator>
void swap(deque<T, Allocator>& left, deque<T, Allocator>& right) noexcept(noexcept(left.swap(right)))
{
	left.swap(right);
}

template<typename T, typename Allocator>
bool operator==(const deque<T, Allocator>& left, const deque<T, Allocator>& right)
{
	return _STD equal(left.cbegin(), left.cend(), right.cbegin(), right.cend());
}

template<typename T, typename Allocator>
bool operator!=(const deque<T, Allocator>& left, const deque<T, Allocator>& right)
{
	return (!(left == right));
}

template<typename T, typename Allocator>
bool operator<(const deque<T, Allocator>& left, const deque<T, Allocator>& right)
{
	return _STD lexicographical_compare(left.cbegin(), left.cend(), right.cbegin(), right.cend());
}

template<typename T, typename Allocator>
bool operator>=(const deque<T, Allocator>& left, const deque<T, Allocator>& right)
{
	return (!(left < right));
}

template<typename T, typename Allocator>
bool operator>(const deque<T, Allocator>& left, const deque<T, Allocator>& right)
{
	return _STD lexicographical_compare(right.cbegin(), right.cend(), left.cbegin(), left.cend());
}
template<typename T, typename Allocator>
bool operator<=(const deque<T, Allocator>& left, const deque<T, Allocator>& right)
{
	return (!(left > right));
}


#endif // !DEQUE_HPP
