#include<memory>
#define _STD ::std::

template<typename _Ty>struct _list_iterator;
template <typename _Ty> struct _list_const_iterator;

template<typename _Ty>
struct _list_node
{
	void* next;
	void* prev;
	_Ty data;
};

template<typename _Ty>
struct _list_iterator
{
	using value_type = _Ty;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using size_type = unsigned;
	using difference_type = int;
	using link_type = _list_node<_Ty>*;
	using iterator = _list_iterator<_Ty>;
	using const_iterator = _list_const_iterator<_Ty>;


	link_type node;

	_list_iterator(link_type lt) :node(lt) {}
	_list_iterator() = default;

	bool operator==(const iterator& it) const { return node == x.node; }
	bool operator!=(const iterator& it) const { return node != x.node; }

	reference operator*() const { return (*node).data; }

	iterator& operator++()
	{
		node = static_cast<link_type>((*node).next);
		return *this;
	}

	iterator operator++(int)
	{
		iterator tmp = *this;
		++*this;
		return tmp;
	}

	iterator& operator--()
	{
		node = static_cast<link_type>((*node).prev);
		return *this;
	}

	iterator operator--(int)
	{
		iterator tmp = *this;
		--*this;
		return tmp;
	}
};

template<typename _Ty>
struct _list_const_iterator
{
	using iterator = _list_iterator<_Ty>;
	using const_iterator = _list_const_iterator<_Ty>;
	using value_type = _Ty;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using link_type = _list_node<_Ty>*;
	using size_type = unsigned;
	using difference_type = int;

	link_type node;

	_list_const_iterator(link_type lt) :node(lt) {}
	_list_const_iterator() = default;
	_list_const_iterator(const iterator& it) :node(x.node) {}

	bool operator == (const const_iterator& _Right) const { return node == _Right.node; }
	bool operator != (const const_iterator& _Right) const { return node != _Right.node; }

	const_reference operator*() const { return (*node).data; }

	const_iterator& operator++()
	{
		node = static_cast<link_type>((*node).next);
		return *this;
	}

	const_iterator operator++(int)
	{
		const_iterator tmp = *this;
		++*this;
		return tmp;
	}

	const_iterator& operator--()
	{
		node = static_cast<link_type>((*node).prev);
		return *this;
	}

	const_iterator operator--(int)
	{
		const_iterator tmp = *this;
		--*this;
		return tmp;
	}
};

template<_Ty>
struct simple_alloc
{
	using pointer = _Ty*;
	static pointer allocate()
	{
		return new _Ty;
	}

	static void deallocate(pointer p)
	{
		delete p;
	}
};

template<typename _Ty,typename _Alloc=simple_alloc<_list_node<_Ty>>>
class list
{
public:
	using list_node = _list_node;
	using list_node_allocator = _Alloc;
	using value_type = _Ty;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using link_type = list_node*;
	using size_type = unsigned;
	using difference_type = int;
	using iterator = _list_iterator<_Ty>;
	using const_iterator = _list_const_iterator<_Ty>;

private:
	link_type node;
	size_type length;

	link_type _Buynode()
	{
		return list_node_allocator::allocate();
	}

	void _Freenode(link_type p)
	{
		list_node_allocator::deallocate(p);
	}


public:

	list() :length(0),node(_Buynode())
	{
		(*node).next = node;
		(*node).prev = node;
	}

	iterator begin()
	{
		return static_cast<link_type>((*node).next);
	}

	const_iterator begin() const
	{
		return static_cast<link_type>((*node).next);
	}

	iterator end()
	{
		return node;
	}

	const_iterator end() const
	{
		return node;
	}

	bool empty() const
	{
		return length == 0;
	}

	size_type size() const
	{
		return length;
	}

	size_type max_size() const
	{
		return size_type(-1);
	}

	reference front()
	{
		return *static_cast<link_type>((*node).next);
	}

	const_reference front() const
	{
		return *static_cast<link_type>((*node).next);
	}

	reference back()
	{

	}







};