#include<initializer_list>//值列表类
#include<memory>//allocator类	
#include<stdexcept>//用到其中几个异常类
#include<utility>//std::move()std::forward()
#define _STD ::std::

template<typename _Ty,class _Alloc=_STD allocator<_Ty>>
class vector
{
	using value_type = _Ty;
	using reference = _Ty & ;
	using const_reference = const _Ty &;
	using iterator = _Ty * ;
	using const_iterator = const _Ty *;
	using size_type = unsigned ;//类似 size_t
	using difference_type = _Ty;
	using allocator_type =_Alloc;
	using pointer = _Ty *;
	using const_pointer=const _Ty *;

private:
	iterator elem = nullptr;//首地址				==beign()
	iterator space = nullptr;//未使用空间首地址	==end()
	iterator last = nullptr;//总共地址尾后地址
	allocator_type alloc;//内存管理对象

public:
	iterator& _first() { return elem; }
	iterator& _end() { return space; }
	iterator& _last() { return last; }
	allocator_type& _allocator() { return alloc; }
	void _allocator_reset() { alloc = allocator_type(); }

	vector()
	{
		elem=alloc.allocate(4);
		space = elem;
		last = elem + 4;
	}

	explicit vector(const _Alloc& _Al) noexcept 
	:vector(void)
	{
		alloc = _Al;
	}

	explicit vector(const size_type _Count,const _Alloc& _Al=_Alloc()) :vector(_Count,_Ty(),_Al)
	{
		alloc = _Al;
	}

	vector(const size_type _Count,const value_type& _Val,const _Alloc& _Al=_Alloc()) //填充为value
	{
		alloc = _Al;
		elem = alloc.allocate(_Count);
		_STD uninitialized_fill_n(elem, _Count, value);
		space = last = elem + _Count;
	}
	
	template<typename _Iter>
	vector(const _Iter _First,const _Iter _Last,const _Alloc& _Al=_Alloc()) 
	{
		difference_type p_diff = _Last-_First;
		if ( 4 < p_diff)
		{
			elem = alloc.allocate(p_diff);
			last = elem + p_diff;
		}
		else
		{
			elem = alloc.allocate(4);
			last = elem + 4;
		}
		for (int i = 0; _First != _Last; ++_First, ++i)
			*(elem + i) = _First;
		space = elem + p_diff;
	}

	vector(_STD initializer_list<_Ty> &il,const _Alloc& _Al=_Alloc()) :vector(il.begin(),il.end(),_Al) //值列表 只返回const指针
	{    }

	vector(const vector& _Right):vector(_Right.cbegin(),_Right.cend(),_Right._allocator())
	{    }

	vector(const vector& _Right, const _Alloc& _Al) :vecetor(_Right.cbegin(), _Right.cend(), _Al)
	{    }

	vector(vector&& _Right) noexcept
	{
		elem = _Right._first();
		space = _Right._end();
		last = _Right._last();
		alloc = _STD move(_Right._allocator());

		_Right._first() = pointer();
		_Right._end() = pointer();
		_Right._last() = pointer();
		_Right._allocator_reset();
	}

	vector(vector&& _Right,const _Alloc& _Al)
	{
		elem = _Right._first();
		space = _Right._end();
		last = _Right._last();
		alloc = _Al;

		_Right._first() = pointer();
		_Right._end() = pointer();
		_Right._last() = pointer();
	}

	~vector() noexcept
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

	vector& operator=(_STD initializer_list<_Ty> &li)
	{
		int lsize = li.size();
		if (lsize > last - elem)
		{
			throw _STD range_error("initializer_list can't longer than vector");
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

	//以上为控制函数
	/***********************************************************************************************************************************************************/
	//功能函数


	//迭代器和指针********************************************************************************

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
	//TODO:反序迭代器

	//大小和容量*************************************************************************************

	size_type size() const	//元素数目
	{
		return static_cast<unsigned>(space - elem);
	}

	bool empty()const
	{
		return (begin()==end());
	}

	size_type capacity()const	//已分配的空间大小
	{
		return static_cast<unsigned>(last - elem);
	}

	void reserve(size_type _Count)
	{
		iterator p = alloc.allocate((last - elem) + _Count);
		for (int i = 0; elem + i != space; ++i)
		{
			*(p + i) = *(elem + i);
		}
		for (iterator it = elem; it != space; ++it)
		{
			alloc.destroy(it);//析构容器中每个元素
		}
		alloc.deallocate(elem, (last - elem));
		space = p + (space - elem);
		last = p + (last - elem)+_Count;
		elem = p;
	}

	void resize(size_type _Count)
	{
		value_type v;
		resize(_Count, v);// !!! Some Complier don't allow to do this
	}

	void resize(size_type _Count, value_type value)
	{
		if (_Count <= (last - elem))//_Count比原空间小
		{
			for (int i = _Count; elem + i != last; ++i)
			{
				alloc.destroy(elem + i);
			}
			alloc.deallocate(elem + _Count, (last - elem) - _Count);// !!!
			(space - elem) <= _Count ? last = elem + _Count : last = space = elem + _Count;
			for ( ; space != last; ++space)
			{
				alloc.construct(space, value);
			}
		}
		else//_Count比原空间大
		{
			iterator p = alloc.allocate(_Count);
			for (int i = 0; elem + i != space; ++i)
			{
				*(p + i) = *(elem + i);
				alloc.destroy(elem + i);
			}
			alloc.deallocate(elem, (last - elem));
			last = p + _Count;
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
		for ( iterator p=elem; p != space; ++p)
		{
			alloc.destroy(p);
		}
		space = elem;
	}

	//元素访问**************************************************************************************

	value_type& front()const
	{
		return *elem;
	}

	value_type& back()const
	{
		return *(space - 1);
	}

	value_type& operator[](int n)const//这里没有将参数限制为无符号数，是想提供无边界检查的访问
	{
		return *(elem + n);
	}

	value_type& at(int n)const
	{
		if (n<0 || n>(space - elem))
			throw _STD out_of_range("vector out of range");
		else
			return *(elem + n);
	}

	//栈操作*****************************************************************************************

	void push_back(const value_type& _Val)
	{
		emplace_back(_Val);
	}

	void push_back(const value_type&& _Val)
	{
		emplace_back(_STD move(_Val));
	}

	void pop_back()
	{
		alloc.destroy(space);
		--space;
	}

	template<typename ...Args>
	void emplace_back(Args&&...args)
	{
		if (last==space)
			reserve(capacity());
		alloc.construct(space++, _STD forward<Args>(args)...);
	}

	template<typename ...Args>
	iterator emplace_back(const_iterator _Where, Args&&...args)
	{
		if (_Where == space)
		{
			emplace_back(_STD forward<Args>(args)...);
			return space - 1;
		}
		else
		{
			const size_type p_diff = _Where - elem;
			if (last == space)
			{
				pointer ptemp = alloc.allocator(2 * capacity());
				for (int i = 0; elem + i != _Where; ++i)
					alloc.construct(ptemp + i, *(elem + i));
				alloc.construct(ptemp + p_diff, _STD forward<Args>(args)...);
				for (int i = 0; _Where + i != space; ++i)
					alloc.construct(ptemp + p_diff + i, *(_Where + i));
				for (auto p = elem; p != space; ++p)
					alloc.destroy(p);
				alloc.deallocator(elem,last-elem);
				space = ptemp + 1 + (space - elem);
				last = ptemp + 2 * capacity();
				elem = ptemp;
				return ptemp + p_diff;
			}
			else
			{
				for (int i = 0; _Where + i != space; ++i)
				{
					alloc.construct(space - i, *(_Where + i));
					alloc.destroy(_Where + i);
				}
				alloc.construct(_Where, _STD forward<Args>(args)...);
				++space;
				return _Where;
			}
		}
	}

	//列表操作*********************************************************************************************

	iterator insert(const_iterator _Where, const value_type& _Val)
	{
		return emplace(_Where, _Val);
	}

	iterator insert(const_iterator _Where, const value_type&& _Val)//可以使用移动操作
	{
		return emplace(_Where,_STD move(_Val));
	}

	iterator insert(iterator position,const size_type _Count,const value_type& _Val)//不能使用移动，不能对同一对象移动两次，这里要求类型允许拷贝
	{
		if (last - space >= _Count)
		{
			iterator p = space - 1;
			for ( ; p >=position; --p)
			{
				alloc.construct(p + _Count, *p);
				alloc.destroy(p);
			}
			for (int i = 0; i != _Count; ++_Count)
			{
				alloc.construct(position+i, _Val);
			}
			last += _Count;
			return position;
		}
		else
		{
			int rsize = 2*(last - elem);
			difference_type dist = position - elem;
			while (rsize < _Count+(space-elem))
			{
				rsize *= 2;
			}
			iterator tp = alloc.allocate(rsize);
			for (int i=0; elem + i != position; ++i)
			{
				alloc.construct(tp + i, *(elem + i));
				alloc.destroy(elem + i);
			}
			for (int i = 0; i != _Count; ++i)
			{
				alloc.construct(tp + dist + i, _Val);
			}
			for ( ; position != space; ++position)
			{
				alloc.construct(tp + (position - elem) + _Count, *(position));
				alloc.destroy(position);
			}
			alloc.deallocate(elem, space - elem);
			last = tp + rsize;
			space = tp + _Count + (last - elem);
			elem = tp;
			return tp + dist;
		}
	}

	template<typename Iter>
	iterator insert(const_iterator position, Iter  _First, Iter  _Last)
	{
		size_type size = _Last-_First;
		if (last - space >= size)
		{
			iterator p = space - 1;
			for (; p >= position; --p)
			{
				alloc.construct(p + size, *p);
				alloc.destroy(p);
			}
			for (; _First != _Last; ++_First)
			{
				alloc.construct(position++, *_First);
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
			for (int i = position - elem; _First != _Last; ++_First,++i)
			{
				alloc.construct(p + i, *(_First));
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

	iterator insert(iterator position, _STD initializer_list<int> li)
	{
		return insert(position, (iterator)li.begin(), (iterator)li.end());
	}

	void assign(const size_type _Newsize, const value_type& _Val)
	{
		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{
			for (auto p = elem; p != space; ++p)
				alloc.destroy(p);
			alloc.deallocate(elem, last - elem);
			iterator it = alloc.allocate(_Newsize);
			elem = it;
			last=space = elem + _Newsize;
			for (; it != last; ++it)
				alloc.construct(it, _Val);
		}
		else if (_Newsize > _Oldsize)
		{
			for (auto p = elem; p != space; ++elem)
				*p = _Val;
			for ( ; space - elem <= _Newsize; ++space)
				alloc.construc(space, _Val);
		}
		else
		{
			const_iterator _Newspace = elem + _Newsize;
			iterator temp = elem;
			for ( ; temp != _Newspace; ++temp)
				*(temp) = _Val;
			for (; temp != space; ++temp)
				alloc.destroy(temp);
			space = _Newspace;
		}
	}

	template<typename Iter>
	void assign(Iter _First, Iter _Last)
	{

	}

	void assign(_STD initializer_list<_Ty>il)
	{
		assign(il.begin(), il.end());
	}



	iterator earse(const_iterator p)
	{
		return earse(p, p+1);
	}

	iterator earse(const_iterator begin,const_iterator end )
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

	//比较与交换*********************************************************************************

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

template<typename _T1, typename _T2>
void swap(vector<_T1> &v1, vector<_T2>& v2)
{
	v1.swap(v2);
}
