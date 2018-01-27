#ifndef STACK_HPP
#define STACK_HPP

#include<stack>
#include<utility>
#include<type_traits>
#define _STD ::std::

template<typename T, typename Container = _STD deque<T>>
class stack
{
public:
	using value_type = typename Container::value_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;
	using size_type = typename Container::size_type;
	using container_type = Container;

	explicit stack(const Container& cont)
		:c_(cont)
	{   }

	explicit stack(Container&& cont)
		:c_(_STD move(cont))
	{   }

	stack(const stack& right)
		:c_(right.c_)
	{   }

	stack(stack&& right)
		:c_(_STD move(right.c_))
	{   }

	//TODO:need TMP from here
	template<typename Allocator>
	explicit stack(const Allocator& alloc)
		:c_(alloc)
	{   }

	template<typename Allocator>
	stack(const Container&right, const Allocator& alloc)
		: c_(right, alloc)
	{   }

	template<typename Allocator>
	stack(Container&& right, const Allocator& alloc)
		: c_(_STD move(right), alloc)
	{   }

	template<typename Allocator>
	stack(const stack&right, const Allocator& alloc)
		: c_(right.c_, alloc)
	{   }

	template<typename Allocator>
	stack(stack&& right, const Allocator& alloc)
		: c_(_STD move(right), alloc)
	{   }

	~stack()
	{   }

	stack& operator=(const stack& right)
	{
		c_ = right.c_;
	}

	stack& operator=(stack&& right)
	{
		c_ = _STD move(right.c_);
	}

	reference top()
	{
		return c_.back();
	}

	const_reference top() const
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

	void push(const value_type& val)
	{
		c_.push_back(val);
	}

	void push(value_type&& val)
	{
		c_.push_back(_STD move(val));
	}

	template<typename...Args>
	void emplace_back(Args&&...args)
	{
		c_.emplace_back(_STD forward<Args>(args)...);
	}

	void pop()
	{
		c_.pop_back();
	}

	void swap(stack& right) noexcept(_STD _Is_nothrow_swappable<container_type>::value)
	{
		_STD swap(c_, right.c_);
	}

	const container_type& _Get_container() const
	{
		return c_;
	}

private:
	Container c_;

};

template<typename T, typename Container>
void swap(stack<T, Container>&left, stack<T, Container>&right) noexcept(noexcept(left.swap(right)))
{
	left.swap(right);
}

template<typename T, typename Container>
inline bool operator==(const stack<T, Container>&left, const stack<T, Container>& right)
{
	return (left._Get_container() == right._Get_container());
}

template<typename T, typename Container>
inline bool operator!=(const stack<T, Container>&left, const stack<T, Container>& right)
{
	return (!(left == right));
}

template<typename T, typename Container>
inline bool operator<(const stack<T, Container>&left, const stack<T, Container>& right)
{
	return (left._Get_container() < right._Get_container());
}

template<typename T, typename Container>
inline bool operator>(const stack<T, Container>&left, const stack<T, Container>& right)
{
	return (right < left);
}

template<typename T, typename Container>
inline bool operator<=(const stack<T, Container>&left, const stack<T, Container>& right)
{
	return(!(right<left));
}

template<typename T, typename Container>
inline bool operator>=(const stack<T, Container>&left, const stack<T, Container>& right)
{
	return (!(left < right));
}
#endif // !STACK_HPP
