#include<initializer_list>//值列表类
#include<memory>//allocator类	
#include<stdexcept>//其中定义了几种常见的异常类


class vector
{

	using reference = int & ;
	using const_reference = const int &;
	using iterator = int * ;
	using const_iterator = const int *;
	using size_type = unsigned ;//类似 size_t
	using difference_type = int;
	using value_type = int;
	using allocator_type = std::allocator<int>;

private:
	int *elem = nullptr;//头		beign()
	int *space = nullptr;//未使用空间首地址	end()
	int *last = nullptr;//总共地址尾后地址
	allocator_type alloc;//内存管理对象

public:
	vector()
	{
		elem=alloc.allocate(4);
		space = elem;
		last = elem + 4;
	}

	vector(int s) :vector(s,0)//委托给 vector(int s,int value)，并填充为0
	{ }

	vector(int s, int value) //填充为value
	{
		elem = alloc.allocate(s);
		std::uninitialized_fill_n(elem, s, value);
		space = last = elem + s;
	}
	//TODO:这个构造是否高效、安全？？？
	vector(const int *b,const int *c) //why const ? 
	{
		difference_type p_diff = c - b;
		elem = alloc.allocate(p_diff);
		space = last = elem + p_diff;
		for (int i = 0; elem + i != last; ++i)
		{
			*(elem + i) = *(b + i);
		}
	}

	vector(std::initializer_list<int> &il) :vector(il.begin(),il.end())//值列表 返回const指针
	{ }

	vector(vector& v):vector(v.cbegin(),v.cend())
	{ }

	~vector()
	{
		for (auto p = elem; p != space; ++p)
		{
			alloc.destroy(p);
		}
		alloc.deallocate(elem, last - elem);
	}

	//以上为控制函数
	/***********************************************************************************************************************************************************/
	//功能函数


	//迭代器和指针
	iterator begin()
	{
		return elem;
	}

	const iterator cbegin()
	{
		return elem;
	}

	iterator end()
	{
		return space;
	}

	const iterator cend()
	{
		return space;
	}
	//TODO:反序迭代器

	//大小和容量
	size_type size()
	{
		return static_cast<unsigned>(space - elem);
	}

	bool empty()
	{
		return (size() == 0);
	}

	size_type capacity()
	{
		return static_cast<unsigned>(last - elem);
	}

	void reserve(int n)
	{
		auto p = alloc.allocate((last - elem) + n);
		for (int i = 0; elem + i != space; ++i)
		{
			*(p + i) = *(elem + i);
		}
		space = p + (space - elem);
		last = p + (space - elem);
		elem = p;
	}

	void resize(int n)
	{
		value_type v;
		resize(n, v);// !!! Some Complier don't allow to do this
	}

	void resize(int n, int value)
	{
		if (n <= (last - elem))
		{
			for (int i = n; elem + i != last; ++i)
			{
				alloc.destroy(elem + i);
			}
			alloc.deallocate(elem + n, (last - elem) - n);// !!!
			(space - elem) <= n ? last = elem + n : last = space = elem + n;
			for ( ; space != last; ++space)
			{
				alloc.construct(space, value);
			}
		}
		else
		{
			auto p = alloc.allocate(n);
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
		for ( iterator p=elem; p != space; ++p)
		{
			alloc.destroy(p);
		}
	}

	//元素访问
	value_type& front()
	{
		return *elem;
	}

	value_type& back()
	{
		return *(space - 1);
	}

	value_type& operator[](int n)//这里没有将参数限制为无符号数，是想提供无边界检查的访问
	{
		return *(elem + n);
	}

	value_type& at(int n)
	{
		if (n<0 || n>(space - elem))
			throw std::out_of_range("vector out of range");
		else
			return *(elem + n);
	}

	//栈操作

	void push_back(value_type v)
	{
		
	}

	void pop_back()
	{

	}

	void emplace_back()//args
	{

	}

	//列表操作

	iterator insert(iterator p, int x)
	{

	}

	iterator insert(iterator p, int n, int x)
	{

	}

	iterator insert(iterator p, void * first, void * last)
	{

	}

	iterator insert(iterator p, std::initializer_list<int> li)
	{

	}

	iterator earse(iterator p)
	{

	}

	iterator earse(iterator first, iterator last)
	{

	}

	void clear()
	{

	}

	//比较与交换

	bool operator==(vector& v);

	bool operator!=(vector& v);

	bool operator<(vector& v);

	bool operator<=(vector& v);

	bool operator>(vector& v);

	bool operator>=(vector& v);

	void swap(vector& v);

	friend void swap(vector&v1, vector&v2);


};
