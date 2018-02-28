#ifndef QUEUE_HPP
#define QUEUE_HPP

#include<deque>
#include<vector>
#include<utility>
#include<type_traits>
#include<algorithm>
#include<memory>
#include<functional>
#define _STD ::std::



template<typename T, typename Container=_STD deque<T>>
class queue
{
public:
	using container_type=Container;
	using value_type=typename Container::value_type;
	using size_type=typename Container::size_type;
	using reference=typename Container::reference;
	using const_reference=typename Container::const_reference;

	queue()
		:c_()
	{   }

	explicit queue(const container_type& cont)
		:c_(cont)
	{   }

	explicit queue(container_type&& cont)
		:c_(_STD move(cont))
	{   }

	queue(const queue& right)
		:c_(right.c_)
	{   }

	queue(queue&& right)
		:c_(_STD move(right.c_))
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<container_type, Allocator>>>
		explicit queue(const Allocator& alloc)
		:c_(alloc)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<container_type, Allocator>>>
		queue(const container_type& cont, const Allocator alloc)
		: c(cont, alloc)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<container_type, Allocator>>>
		queue(container_type&& cont, const Allocator& alloc)
		: c_(_STD move(cont), alloc)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<container_type, Allocator>>>
		queue(const queue& other, const Allocator& alloc)
		: c_(other.c_, alloc)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<container_type, Allocator>>>
		queue(queue&& other, const Allocator& alloc)
		: c_(_STD move(other.c_), alloc)
	{   }

	queue& operator=(const queue& right)
	{
		c_=right.c_;
		return *this;
	}

	queue& operator=(queue&& right)
	{
		c_=_STD move(right.c_);
		return *this;
	}

	reference front()
	{
		return c_.front();
	}

	const_reference front() const
	{
		return c_.front();
	}

	reference back()
	{
		return c_.back();
	}

	const_reference back() const
	{
		return c_.back();
	}

	bool empty() const
	{
		return c_.empty();
	}

	size_type size() const
	{
		return c_.size();
	}

	void push(const value_type& value)
	{
		c_.push_back(value);
	}

	void push(value_type&& value)
	{
		c_.push_back(_STD move(value));
	}

	template<typename...Args>
	decltype(auto) emplace(Args&&...args)
	{
		return c.emplace_back(_STD forward<Args>(args)...);
	}

	void pop()
	{
		c_.pop_front();
	}

	void swap(queue& right)
		noexcept(_STD _Is_nothrow_swappable<container_type>::value)
	{
		swap(c_, right.c_);
	}

	const container_type& _get_container() const
	{
		return c_;
	}

protected:
	container_type c_;
};

template<typename T, typename Container>
void swap(queue<T, Container>& left, queue<T, Container>& right)
{
	left.swap(right);
}

template<typename T, typename Container>
bool operator==(const queue<T, Container>& left, const queue<T, Container>& right)
{
	return ((left._get_container())==(right._get_container()));
}

template<typename T, typename Container>
bool operator!=(const queue<T, Container>& left, const queue<T, Container>& right)
{
	return ((left._get_container())!=(right._get_container()));
}

template<typename T, typename Container>
bool operator<(const queue<T, Container>& left, const queue<T, Container>& right)
{
	return ((left._get_container()) < (right._get_container()));
}

template<typename T, typename Container>
bool operator>(const queue<T, Container>& left, const queue<T, Container>& right)
{
	return ((left._get_container()) > (right._get_container()));
}

template<typename T, typename Container>
bool operator<=(const queue<T, Container>& left, const queue<T, Container>& right)
{
	return ((left._get_container())<=(right._get_container()));
}

template<typename T, typename Container>
bool operator>=(const queue<T, Container>& left, const queue<T, Container>& right)
{
	return ((left._get_container())>=(right._get_container()));
}

template<typename T, typename Container, typename Alloc>
struct _STD uses_allocator<queue<T, Container>, Alloc>
	:_STD uses_allocator<Container, Alloc>::type
{

};

template<typename T,
	typename Container=_STD vector<T>,
	typename Compare=_STD less<typename Container::value_type>>
	class priority_queue
{
public:
	using container_type=Container;
	using value_type=typename Container::value_type;
	using size_type=typename Container::size_type;
	using reference=typename Container::reference;
	using const_reference=typename Container::const_reference;
	using value_compare=Compare; // since C++ 17

	priority_queue()
		:c_(), comp_()
	{   }

	priority_queue(const Compare& comp, const Container& cont)
		:c_(cont), comp_(comp)
	{   }

	explicit priority_queue(const Compare& comp,
		Container&& cont=Container())
		:c_(cont), comp_(comp)
	{   }

	priority_queue(const priority_queue& right)
		:c_(right.c_), comp_(right.comp_)
	{   }

	priority_queue(priority_queue&& right)
		:c_(_STD move(right.c_)), comp_(_STD move(right.comp_))
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<Container, Allocator>>>
		explicit priority_queue(const Allocator& alloc)
		:c_(alloc), comp_()
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<Container, Allocator>>>
		priority_queue(const Compare& comp, const Allocator& alloc)
		: c_(alloc), comp_(comp)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<Container, Allocator>>>
		priority_queue(const Compare& comp, const Container& cont,
			const Allocator& alloc)
		: c_(cont, alloc), comp_(comp)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<Container, Allocator>>>
		priority_queue(const Compare& comp, Container&& cont, const Allocator& alloc)
		: c_(_STD move(cont), alloc), comp_(comp)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<Container, Allocator>>>
		priority_queue(const priority_queue& right, const Allocator& alloc)
		: c_(right.c_, alloc), comp_(right.comp_)
	{   }

	template<typename Allocator,
		class=_STD enable_if_t<_STD uses_allocator_v<Container, Allocator>>>
		priority_queue(priority_queue&& right, const Allocator& alloc)
		: c_(_STD move(right.c_), alloc), comp_(_STD move(right.comp_))
	{   }

	template<typename InputIt>
	priority_queue(InputIt first, InputIt last, const Compare& comp,
		const Container& cont)
		: c_(cont), comp_(comp)
	{
		c_.insert(c_.end(), first, last);
		_STD make_heap(c_.begin(), c_.end(), comp);
	}

	template<typename InputIt>
	priority_queue(InputIt first, InputIt last)
		:c_(first, last), comp_()
	{
		_STD make_heap(c_.begin(), c_, end(), comp_);
	}

	template<typename InputIt>
	priority_queue(InputIt first, InputIt last, const Compare& comp)
		: c_(first, last), comp_(comp)
	{
		_STD make_heap(c_.begin(), c_.end(), comp_);
	}

	template<typename InputIt>
	priority_queue(InputIt first, InputIt last, const Compare& comp, Container&& cont)
		: c_(cont), comp_(comp)
	{
		c_.insert(c_.end(), first, last);
		_STD make_heap(c_.begin(), c_.end(), comp_);
	}

	priority_queue& operator=(const priority_queue& right)
	{
		c_=right.c_;
		comp_=right.comp_;
		return *this;
	}

	priority_queue& operator=(priority_queue&& right)
	{
		c_=_STD move(right.c_);
		comp_=_STD move(right.comp_);
		return *this;
	}

	const_reference top() const
	{
		return (c_.front());
	}

	bool empty() const
	{
		return (c_.empty());
	}

	size_type size() const
	{
		return (c_.size());
	}

	void push(const value_type& val)
	{
		c_.push_back(val);
		std::push_heap(c_.begin(), c_.end(), comp_);
	}

	void push(value_type&& val)
	{
		c_.push_back(_STD move(val));
		std::push_heap(c_.begin(), c_.end(), comp_);
	}

	template<typename...Args>
	void emplace(Args&&...args)
	{
		c_.emplace_back(_STD forward<Args>(args)...);
		std::push_heap(c_.begin(), c_.end(), comp_);
	}

	void pop()
	{
		_STD pop_heap(c_.begin(), c_.end(), comp_);
		c_.pop_back();
	}

	void swap(priority_queue& right)
		noexcept(
			_STD is_nothrow_swappable<container_type>::value&&
			_STD is_nothrow_swappable<value_compare>::value)
	{
		_STD swap(c_, right.c_);
		_STD swap(comp_, right.comp_);
	}

protected:
	container_type c_;
	value_compare comp_;
};

template<typename T, typename Container, typename Compare>
void swap(priority_queue<T, Container, Compare>& left,
	priority_queue<T, Container, Compare>& right)
	noexcept(noexcept(left.swap(right)))
{
	left.swap(right);
}

template<typename T, typename Container, typename Compare, typename Allocator>
struct _STD uses_allocator<priority_queue<T, Compare, Container>, Allocator> :
	public _STD uses_allocator<Container, Allocator>::type
{   };


#endif // !QUEUE_HPP

