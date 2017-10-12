#include<initializer_list>
#include<memory>
#include<stdexcept>
#include<utility>

class vector
{

	using reference = int &;
	using const_reference = const int &;
	using iterator = int *;
	using const_iterator = const int *;
	using size_type = unsigned;
	using difference_type = int;
	using value_type = int;
	using allocator_type = std::allocator<int>;
	using pointer = int *;
	using const_pointer = const int *;

private:
	iterator elem = nullptr;
	iterator space = nullptr;
	iterator last = nullptr;
	allocator_type alloc;

public:
	iterator& _first() { return elem; }
	iterator& _end() { return space; }
	iterator& _last() { return last; }

public:
	vector()
	{
		elem = alloc.allocate(4);
		space = elem;
		last = elem + 4;
	}

	explicit vector(size_type s) :vector(s, 0)
	{ }

	vector(size_type s, value_type value) 
	{
		elem = alloc.allocate(s);
		std::uninitialized_fill_n(elem, s, value);
		space = last = elem + s;
	}

	vector(const int *b, const int *c)
	{
		difference_type p_diff = c - b;
		elem = alloc.allocate(p_diff);
		space = last = elem + p_diff;
		for (int i = 0; elem + i != last; ++i)
		{
			*(elem + i) = *(b + i);
		}
	}

	vector(std::initializer_list<int> &il) :vector(il.begin(), il.end())
	{ }

	vector(vector& v) :vector(v.cbegin(), v.cend())
	{ }

	~vector()
	{
		for (auto p = elem; p != space; ++p)
		{
			alloc.destroy(p);
		}
		alloc.deallocate(elem, last - elem);
	}

	vector& operator=(const vector&v)
	{
		int rsize = v.capacity();
		clear();
		alloc.deallocate(elem, last - elem);
		elem = alloc.allocate(rsize);
		space = elem;
		for (iterator p = v.begin(); p != v.end(); ++p)
		{
			alloc.construct(space++, *(p));
		}
		last = elem + rsize;
		return *this;
	}

	vector& operator=(std::initializer_list<int> &li)
	{
		int lsize = li.size();
		if (lsize > last - elem)
		{
			throw std::range_error("initializer_list can't longer than vector");
		}
		clear();
		for (const_iterator p = li.begin(); p != li.end(); ++p)
		{
			alloc.construct(space++, *p);
		}
		for (; space != last; ++space)
		{
			alloc.construct(space);
		}
		return *this;
	}

	iterator begin()const
	{
		return elem;
	}

	const iterator cbegin()const
	{
		return elem;
	}

	iterator end() const
	{
		return space;
	}

	const iterator cend()const
	{
		return space;
	}

	size_type size() const	
	{
		return (space - elem);
	}

	bool empty()const
	{
		return (begin() == end());
	}

	size_type capacity()const	
	{
		return static_cast<unsigned>(last - elem);
	}

	void reserve(size_type n)
	{
		iterator p = alloc.allocate((last - elem) + n);
		for (int i = 0; elem + i != space; ++i)
		{
			*(p + i) = *(elem + i);
		}
		for (iterator it = elem; it != space; ++it)
		{
			alloc.destroy(it);
		}
		alloc.deallocate(elem, (last - elem));
		space = p + (space - elem);
		last = p + (last - elem) + n;
		elem = p;
	}

	void resize(size_type n)
	{
		value_type v;
		resize(n, v);
	}

	void resize(size_type n, value_type value)
	{
		if (n <= (last - elem))
		{
			for (int i = n; elem + i != last; ++i)
			{
				alloc.destroy(elem + i);
			}
			alloc.deallocate(elem + n, (last - elem) - n);
			(space - elem) <= n ? last = elem + n : last = space = elem + n;
			for (; space != last; ++space)
			{
				alloc.construct(space, value);
			}
		}
		else
		{
			iterator p = alloc.allocate(n);
			for (int i = 0; elem + i != space; ++i)
			{
				*(p + i) = *(elem + i);
				alloc.destroy(elem + i);
			}
			alloc.deallocate(elem, (last - elem));
			last = p + n;
			space = p + (space - elem);
			elem = p;
			for (; space != last; ++space)
			{
				alloc.construct(space, value);
			}
		}
	}

	void shrink_to_fit()
	{
		if (space != last)
		{
			alloc.deallocate(space, last - space);
			last = space;
		}
	}

	void clear()
	{
		for (iterator p = elem; p != space; ++p)
		{
			alloc.destroy(p);
		}
		space = elem;
	}

	value_type& front()const
	{
		return *elem;
	}

	value_type& back()const
	{
		return *(space - 1);
	}

	value_type& operator[](int n)const
	{
		return *(elem + n);
	}

	value_type& at(int n)const
	{
		if (n<0 || n>(space - elem))
			throw std::out_of_range("vector out of range");
		else
			return *(elem + n);
	}

	void push_back(value_type v)
	{
		if (!(last - space))
		{
			reserve(last - elem);
		}
		*space = v;
		++space;
	}

	void pop_back()
	{
		alloc.destroy(space);
		--space;
	}

	template<typename ...Args>
	void emplace_back(Args&&...args)
	{
		if (last - space == 0)
		{
			reserve(last - elem);
		}
		alloc.construct(space++, std::forward<Args>(args)...);
	}

	iterator insert(iterator position, value_type &&v)
	{
		if (last - space)
		{
			iterator p = space - 1;
			for (; p >= position; --p)
			{
				alloc.construct(p + 1, *p);
				alloc.destroy(p);
			}
			alloc.construct(position, std::move(v));
			++last;
			return position;
		}
		else
		{
			iterator p = alloc.allocate(2 * (last - elem));
			difference_type dif = position - elem;
			for (int i = 0; i != dif; ++i)
			{
				alloc.construct(p + i, *(elem + i));
				alloc.destroy(elem + i);
			}
			alloc.construct(p + (position - elem), std::forward<value_type>(v));
			for (; position != space; ++position)
			{
				alloc.construct(p + 1 + (position - elem), *(position));
				alloc.destroy(position);
			}
			alloc.deallocate(elem, last - elem);
			last = p + 2 * (last - elem);
			space = p + 1 + (space - elem);
			elem = p;
			return p + dif;
		}
	}

	iterator insert(iterator position, int n, value_type v)
	{
		if (last - space >= n)
		{
			iterator p = space - 1;
			for (; p >= position; --p)
			{
				alloc.construct(p + n, *p);
				alloc.destroy(p);
			}
			for (int i = 0; i != n; ++n)
			{
				alloc.construct(position + i, v);
			}
			last += n;
			return position;
		}
		else
		{
			int rsize = 2 * (last - elem);
			difference_type dist = position - elem;
			while (rsize < n + (space - elem))
			{
				rsize *= 2;
			}
			iterator tp = alloc.allocate(rsize);
			for (int i = 0; elem + i != position; ++i)
			{
				alloc.construct(tp + i, *(elem + i));
				alloc.destroy(elem + i);
			}
			for (int i = 0; i != n; ++i)
			{
				alloc.construct(tp + dist + i, v);
			}
			for (; position != space; ++position)
			{
				alloc.construct(tp + (position - elem) + n, *(position));
				alloc.destroy(position);
			}
			alloc.deallocate(elem, space - elem);
			last = tp + rsize;
			space = tp + n + (last - elem);
			elem = tp;
			return tp + dist;
		}
	}

	iterator insert(iterator position, iterator  begin, iterator  end)
	{
		size_type size = end - begin;
		if (last - space >= size)
		{
			iterator p = space - 1;
			for (; p >= position; --p)
			{
				alloc.construct(p + size, *p);
				alloc.destroy(p);
			}
			for (; begin != end; ++begin)
			{
				alloc.construct(position++, *begin);
			}
			return p + 1;
		}
		else
		{
			difference_type dif = position - elem;
			int rsize = 2 * (last - elem);
			while (rsize < size + (space - elem))
			{
				rsize *= 2;
			}
			iterator p = alloc.allocate(rsize);
			for (int i = 0; elem + i != position; ++i)
			{
				alloc.construct(p + i, *(elem + i));
				alloc.destroy(elem + i);
			}
			for (int i = position - elem; begin != end; ++begin, ++i)
			{
				alloc.construct(p + i, *(begin));
			}
			for (; position != space; ++position)
			{
				alloc.construct(p + (position - elem) + size);
				alloc.destroy(position);
			}
			alloc.deallocate(elem, last - elem);
			space = p + rsize;
			last = p + (last - elem) + size;
			elem = p;
			return elem + dif;
		}
	}

	iterator insert(iterator position, std::initializer_list<int> li)
	{
		return insert(position, (iterator)li.begin(), (iterator)li.end());
	}

	iterator earse(iterator p)
	{
		return earse(p, p + 1);
	}

	iterator earse(iterator begin, iterator end)
	{
		for (int i = 0; begin + i != end; ++i)
		{
			alloc.destroy(begin + i);
		}
		for (int i = 0; end + i != space; ++i)
		{
			alloc.construct(begin + i, *(end + i));
			alloc.destroy(end + i);
		}
		space = space - (end - begin);
		return begin;
	}

	bool operator==(const vector& v)const
	{
		if (size() != v.size())return false;

		for (int i = 0; elem + i != space; ++i)
		{
			if (*(elem + i) != v[i])
				return false;
		}
		return true;
	}

	bool operator!=(const vector& v)const
	{
		return !operator==(v);
	}

	bool operator<(const vector& v)const
	{
		size_type min_size = size() < v.size() ? size() : v.size();
		for (int i = 0; i != min_size; ++i)
		{
			if (*(elem + i) == v[i])continue;
			if (*(elem + i) < v[i])
				return true;
			else
				return false;
		}
		return size() < v.size();
	}

	bool operator<=(const vector& v)const
	{
		return !(v < *(this));
	}

	bool operator>(const vector& v)const
	{
		return v <*(this);
	}

	bool operator>=(const vector& v)const
	{
		return !(operator<(v));
	}

	void swap(vector& v)
	{
		iterator t = _first();
		_first() = v._first();
		v._first() = t;

		t = _end();
		_end() = v._end();
		v._end() = t;

		t = _last();
		_last() = v._last();
		v._last() = t;
	}

	friend void swap(vector&v1, vector&v2);

};

void swap(vector &v1, vector& v2)
{
	v1.swap(v2);
}
