#ifndef VECTOR_HPP
#define VECTOR_HPP

#include<initializer_list>
#include<memory>
#include<utility>
#include<iterator>
#include<algorithm>
#define _STD ::std::


template<typename T, typename Allocator=_STD allocator<T>>
class vector
{
public:
	using value_type=T;
	using allocator_type=Allocator;
	using size_type=_STD size_t;
	using difference_type=_STD ptrdiff_t;
	using reference=value_type&;
	using const_reference=const value_type&;
	using pointer=typename _STD allocator_traits<Allocator>::pointer;
	using const_pointer=typename _STD allocator_traits<Allocator>::const_pointer;
	using iterator=T*;
	using const_iterator=const T*;
	using reverse_iterator=_STD reverse_iterator<iterator>;
	using const_reverse_iterator=_STD reverse_iterator<const_iterator>;

	vector() noexcept(noexcept(allocator_type()))
		:vector(allocator_type())
	{   }

	explicit vector(const allocator_type& alloc) noexcept
		: alloc_(alloc)
	{
		first_=alloc_.allocate(4);
		end_=first_;
		space_end_=first_+4;
	}

	vector(size_type count, const value_type& value)
		:vector(count, value, allocator_type())
	{   }

	vector(size_type count, const value_type& value, const allocator_type& alloc)
		:alloc_(alloc)
	{
		if (count<4)
		{
			first_=alloc_.allocate(4);
			space_end_=first_+4;
		}
		else
		{
			first_=alloc_.allocate(count);
			space_end_=first_+count;
		}

		_STD uninitialized_fill_n(first_, count, value);
		end_=first_+count;
	}

	explicit vector(size_type count)
		:vector(count, allocator_type())
	{   }

	vector(size_type count, const allocator_type& alloc)
		:alloc_(alloc)
	{
		make_size(count);

		_STD uninitialized_default_construct_n(first_, count);
		end_=first_+count;
	}

	template<typename InputIt>
	vector(InputIt first, InputIt last)
		:vector(first, last, allocator_type())
	{   }

	template<typename InputIt>
	vector(InputIt first, InputIt last, const allocator_type& alloc)
		: alloc_(alloc)
	{
		size_type dist=_STD distance(first, last);
		make_size(dist);

		end_=first_;
		for (; first!=last; ++first)
			alloc_.construct(end_++, *first);
	}

	vector(const vector& right)
		:vector(right.begin(), right.end(), right.get_allocator())
	{   }

	vector(const vector& right, const allocator_type& alloc)
		:vector(right.begin(), right.end(), alloc)
	{   }

	vector(vector&& right) noexcept
		: first_(right.first_), end_(right.end_), space_end_(right.space_end_),
		alloc_(_STD move(right.alloc_))
	{   }

	vector(vector&& right, const allocator_type& alloc)
		:first_(right.first_), end_(right.end_), space_end_(right.space_end_),
		alloc_(alloc)
	{   }

	vector(_STD initializer_list<T> il)
		:vector(il, allocator_type())
	{   }

	vector(_STD initializer_list<T> il, const allocator_type& alloc)
		:alloc_(alloc)
	{
		size_type count=il.size();
		make_size(count);

		auto from=il.begin();
		auto end=il.end();
		end_=first_;
		for (; from!=end; ++from)
			alloc_.construct(end_++, *from);
	}

	~vector()
	{
		for (iterator it=first_; it!=end_; ++it)
			alloc_.destroy(it);

		alloc_.deallocate(first_, capacity());
	}

	vector& operator=(const vector& right)
	{
		int rsize=right.capacity();
		clear();
		alloc_.deallocate(first_, capacity());
		first_=alloc_.allocate(rsize);
		end_=first_;
		space_end_=first_+rsize;
		for (iterator p=right.begin(); p!=right.end(); ++p)
		{
			alloc_.construct(end_++, *(p));
		}
		return *this;
	}

	vector& operator=(vector&& right)
	{
		clear();
		alloc_.deallocate(first_, capacity());
		first_=right.first_;
		end_=right.end_;
		space_end_=right.space_end_;
		alloc_=_STD move(right.alloc_);
		return *this;
	}

	vector& operator=(_STD initializer_list<value_type> il)
	{
		if (il.size()>(capacity()))
		{
			throw
				_STD range_error("initializer_list range must be small than vector size");
		}
		clear();
		for (auto it=il.begin(), it!=il.end(); ++it)
		{
			alloc_.construct(end_++, *it);
		}
		return *this;
	}

	void assign(size_type count, const T& value)
	{
		clear();
		for (; count; --count)
		{
			alloc_.construct(end_++, value);
		}
	}

	template<typename InputIt>
	void assign(InputIt first, InputIt last)
	{
		clear();
		size_type new_size=_STD distance(first, last);
		if (new_size>capacity())
		{
			reserve(new_size);
		}
		for (; first!=last; ++first)
		{
			alloc_.construct(end_++, *first);
		}
	}

	void assign(_STD initializer_list<value_type> il)
	{
		assign(il.begin(), il.end());
	}

	allocator_type get_allocator() const
	{
		return alloc_;
	}

	reference at(size_type index)
	{
		if (!(index<size()))
			throw _STD out_of_range("vector at() out of range");

		return *(first_+index);
	}

	const_reference at(size_type index) const
	{
		if (!(index<size()))
			throw _STD out_of_range("vector at() out of range");

		return *(first_+index);
	}

	reference operator[](size_type index)
	{
		return *(first_+index);
	}

	const_reference operator[](size_type index) const
	{
		return *(first_+index);
	}

	reference front()
	{
		if (empty())
			throw _STD out_of_range("vector front() throw when vector is empty");

		return (*first_);
	}

	const_reference front() const
	{
		if (empty())
			throw _STD out_of_range("vector front() throw when vector is empty");

		return (*first_);
	}

	reference back()
	{
		if (empty())
			throw _STD out_of_range("vector front() throw when vector is empty");

		return (*(end_-1));
	}

	const_reference back() const
	{
		if (empty())
			throw _STD out_of_range("vector front() throw when vector is empty");

		return (*(end_-1));
	}

	T* data() noexcept
	{
		return first_;
	}

	const T* data() const noexcept
	{
		return first_;
	}

	iterator begin() const noexcept
	{
		return first_;
	}

	const_iterator begin() const noexcept
	{
		return first_;
	}

	const_iterator cbegin() const noexcept
	{
		return first_;
	}

	iterator end() const noexcept
	{
		return end_;
	}

	const_iterator end() const noexcept
	{
		return end_;
	}

	const_iterator cend() const noexcept
	{
		return end_;
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	bool empty() const noexcept
	{
		return (first_==end_);
	}

	size_type size() const noexcept
	{
		return static_cast<size_type>(end_-first_);
	}

	size_type capacity() const noexcept
	{
		return static_cast<size_type>(space_end_-first_);
	}

	size_type max_size() const noexcept
	{
		return static_cast<size_type>(99999) // magic number
	}

	void shrink_to_fit()
	{
		if (end_!=space_end_)
		{
			alloc_.deallocate(end_, space_end_-end_);
			space_end_=end_;
		}
	}

	void clear() noexcept
	{
		for (iterator it=first_; it!=end_; ++it)
		{
			alloc_.destroy(it);
		}
		end_=first_;
	}

	template<typename...Args>
	iterator emplace(const_iterator pos, Args&&...args)
	{
		if (pos==cend())
		{
			emplace_back(_STD forward<Args>(args)...);
			return end();
		}
		else
		{
			difference_type dist=pos-first_;
			_expand_if_need();
			iterator new_pos=first_+dist;

			iterator rit=end()-1;
			alloc_.construct(end_++, *rit);
			for (; rit!=new_pos; --rit)
			{
				*rit=(*(rit-1));
			}

			::new (new_pos) value_type(_STD forward<Args>(args)...);
			return new_pos;
		}
	}

	template<typename...Args>
	reference emplace_back(Args&&...args)
	{
		_expand_if_need();

		alloc_.construct(end_, _STD forward<Args>(args)...);
		++end_;
		return *(end_-1);
	}

	iterator insert(const_iterator pos, const value_type& value)
	{
		return emplace(pos, value);
	}

	iterator insert(const_iterator pos, value_type&& value)
	{
		return emplace(pos, _STD move(value));
	}

	iterator insert(const_iterator pos, size_type count, const value_type& value)
	{
		if ((space_end_-end_)<count)
		{
			size_type size=capacity();
			for (; size<((end_-first_)+count); size*=2)
				;
			iterator new_first=alloc_.allocate(size);
			iterator dest=new_first;
			iterator sour=first_;
			for (; sour!=pos;)
			{
				alloc_.construct(dest, sour);
				++dest;
				++sour;
			}
			iterator ret=dest;
			for (; count!=0; --count)
			{
				alloc_.construct(dest, value);
				++dest;
			}
			for (; sour!=end_;)
			{
				alloc_.construct(dest, _STD move(*sour));
				++dest;
				++sour;
			}
			for (sour=first_; sour!=end_; ++sour)
				alloc_.destroy(sour);

			alloc_.deallocate(first_, (space_end_-first_));
			end_=dest;
			first_=new_first;
			space_end_=first_+size;

			return ret;
		}
		else
		{
			iterator rit=end()-1;
			for (; rit>=pos; --rit)
			{
				alloc_.construct(rit+count, (*rit));
				alloc_.destroy(rit);
			}
			for (iterator it=pos; count!=0; --count)
				alloc_.construct(it++, value);

			return const_cast<iterator>(pos);
		}
	}

	template<typename InputIt>
	void insert(const_iterator pos, InputIt first, InputIt last)
	{
		difference_type dist=_STD distance(first, last);
		size_type count=static_cast<size_type>(dist);
		if ((space_end_-end_)<count)
		{
			size_type size=capacity();
			for (; size<((end_-first_)+count); size*=2)
				;
			iterator new_first=alloc_.allocate(size);
			iterator dest=new_first;
			iterator sour=first_;
			for (; sour!=pos;)
			{
				alloc_.construct(dest, sour);
				++dest;
				++sour;
			}
			iterator ret=dest;
			for (; first!=last; ++first)
			{
				alloc_.construct(dest++, (*first));
			}
			for (sour=first_; sour!=end_; ++sour)
				alloc_.destroy(sour);

			alloc_.deallocate(first_, (space_end_-first_));
			end_=dest;
			first_=new_first;
			space_end_=first_+size;

			return ret;
		}
		else
		{
			iterator rit=end()-1;
			for (; rit>=pos; --rit)
			{
				alloc_.construct(rit+count, (*rit));
				alloc_.destroy(rit);
			}
			for (iterator it=pos; first!=last; ++first)
				alloc_.construct(it++, (*first));

			return const_cast<iterator>(pos);
		}
	}

	iterator insert(const_iterator pos, _STD initializer_list<value_type> il)
	{
		return insert(pos, il.begin(), il.end());
	}

	void push_back(const value_type& value)
	{
		emplace_back(value);
	}

	void push_back(value_type&& value)
	{
		emplace_back(_STD move(value));
	}

	void pop_back()
	{
		if (!empty())
		{
			alloc_.destroy(--end_);
		}
	}

	iterator erase(const_iterator pos)
	{
		alloc_.destroy(pos);
		++pos;
		iterator ret=const_cast<iterator>(pos);
		for (; pos!=end_; ++pos)
		{
			alloc_.construct(pos-1, *(pos));
			alloc_.destroy(pos);
		}
		--end_;
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		for (int i=0; first+i!=last; ++i)
		{
			alloc_.destroy(first+i);
		}
		for (int i=0; last+i!=end_; ++i)
		{
			alloc_.construct(first+i, *(last+i));
			alloc_.destroy(last+i);
		}
		end_=end_-(last-first);
		return first;
	}

	void resize(size_type newsize)
	{
		value_type defalut_val;
		resize(newsize, defalut_val);
	}

	void resize(size_type newsize, const value_type& value)
	{
		if (newsize<=(capacity()))
		{
			for (int i=newsize; first_+i!=space_end_; ++i)
			{
				alloc_.destroy(first_+i);
			}
			alloc_.deallocate(first_+newsize, (space_end_-first_)-_Count);
			(end_-first_)<=newsize ? space_end_=first_+newsize : space_end_=end_=first_+newsize;
			for (; end_!=space_end_; ++end_)
			{
				alloc_.construct(end_, value);
			}
		}
		else
		{
			iterator it=alloc.allocate(newsize);
			for (int i=0; first_+i!=end_; ++i)
			{
				*(it+i)=*(first_+i);
				alloc_.destroy(first_+i);
			}
			alloc_.deallocate(first_, (space_end_-first_));
			space_end_=it+newsize;
			end_=it+(end_-first_);
			first_=it;
			for (; end_!=space_end_; ++end_)
			{
				alloc_.construct(end_, value);
			}
		}
	}

	void swap(vector& right)
		noexcept(std::allocator_traits<allocator_type>::propagate_on_container_swap::value
			||std::allocator_traits<allocator_type>::is_always_equal::value)
	{
		_STD swap(first_, right.first_);
		_STD swap(end_, right.end_);
		_STD swap(space_end_, right.space_end_);
		_STD swap(alloc_, right.alloc_);
	}



private:
	void make_size(size_type count)
	{
		if (count<4)
		{
			first_=alloc_.allocate(4);
			space_end_=first_+4;
		}
		else
		{
			first_=alloc_.allocate(count);
			space_end_=first_+count;
		}
	}

	void _expand(size_type newsize)
	{
		iterator new_first=alloc_.allocate(newsize);
		auto it1=first_;
		auto it2=new_first;
		for (; it1!=end_;)
		{
			alloc_.construct(it2, *it1);
			alloc_.destroy(it1);
			++it1;
			++it2;
		}
		alloc_.deallocate(first_, capacity());
		end_=new_first+(end_-first_);
		first_=new_first;
		space_end_=first_+(newsize);
	}

	void _expand_if_need()
	{
		if (end_==space_end_)
			_expand(2*capacity());
	}

	iterator first_;
	iterator end_;
	iterator space_end_;
	allocator_type alloc_;
};

template<typename T, typename Allocator>
bool operator==(const vector<T, Allocator>& left,
	const vector<T, Allocator>& right)
{
	return _STD equal(left.cbegin(), left.cend(), right.cbegin(), right.cend());
}

template<typename T, typename Allocator>
bool operator!=(const vector<T, Allocator>& left,
	const vector<T, Allocator>& right)
{
	return (!(left==right));
}

template<typename T, typename Allocator>
bool operator<(const vector<T, Allocator>& left,
	const vector<T, Allocator>& right)
{
	return _STD lexicographical_compare(left.cbegin(), left.cend(), right.cbegin(), right.cend());
}

template<typename T, typename Allocator>
bool operator>=(const vector<T, Allocator>& left,
	const vector<T, Allocator>& right)
{
	return (!(left<right));
}

template<typename T, typename Allocator>
bool operator>(const vector<T, Allocator>& left,
	const vector<T, Allocator>& right)
{
	return _STD lexicographical_compare(right.cbegin(), right.cend(), left.cbegin(), left.cend());
}

template<typename T, typename Allocator>
bool operator<=(const vector<T, Allocator>& left,
	const vector<T, Allocator>& right)
{
	return (!(left>right));
}

template<typename T, typename Allocator>
void swap(vector<T, Allocator>& left,
	vector<T, Allocator>& right) noexcept(noexcept(left.swap(right)))
{
	left.swap(right);
}


#endif // !VECTOR_HPP
