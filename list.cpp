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

	iterator operator+(int i)
	{
		for (; i != 0; --i)
			++*this;
		return *this;
	}

	iterator operator-(int i)
	{
		for (; i != 0; --i)
			--*this;
		return *this;
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

	const_iterator operator+(int i)
	{
		for (; i != 0; --i)
			++*this;
		return *this;
	}

	const_iterator operator-(int i)
	{
		for (; i != 0; --i)
			--*this;
		retunr *this;
	}
};

template<typename _Ty>
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
	using list_node = _list_node<_Ty>;
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

	void _Reset()
	{
		(*node).next = node;
		(*node).prev = node;
		length = 0;
	}

	void _Delnode(iterator p)
	{
		(*static_cast<link_type>((*(p.node)).prev)).next = (*(p.node)).next;
		(*static_cast<link_type>((*(p.node)).next)).prev = (*(p.node)).prev;
		_Freenode(tmp);
	}

public:

	list() :length(0),node(_Buynode())
	{
		(*node).next = node;
		(*node).prev = node;
	}

	list(size_type _Count, const _Ty& _Val=_Ty()) :length(_Count),node(_Buynode())
	{
		iterator now = _Buynode();
		*now = _Val;
		(*now.node).prev = node;
		(*node).next = now.node;
		for (--_Count; _Count; --_Count)
		{
			iterator tmp = _Buynode();
			*tmp = _Val;
			(*now.node).next = tmp.node;
			(*tmp.node).prev = now.node;
			now = tmp;
		}
		(*now.node).next = node;
		(*node).prev = now.node;
	}

	template<typename Iter>
	list(Iter _First, Iter _Last):node(_Buynode())
	{
		if (_First == _Last)
		{
			_Reset();
			return;
		}
		iterator now = _Buynode();
		++length;
		*now = *_First;
		(*now.node).prev = node;
		(*node).next = now.node;
		for (++_First; _First != _Last; ++_First)
		{
			iterator tmp = _Buynode();
			++length;
			*tmp = *_First;
			(*now.node).next = tmp.node;
			(*tmp.node).prev = now.node;
			now = tmp;
		}
		(*now.node).next = node;
		(*node).prev = now.node;
	}

	list(const list<_Ty>& _Right):list(_Right.begin(),_Right.end()){ }

	list(list<_Ty>&& _Right):length(_Right.length),node(_Buynode())
	{
		(*node).prev = (*(_Right.node)).prev;
		(*node).next = (*(_Right.node)).next;
		(*static_cast<link_type>((*(_Right.node)).prev)).next = node;
		(*static_cast<link_type>((*(_Right.node)).next)).prev = node;
		_Right.length = 0;
		_Right._Reset();
	}

	~list()
	{
		iterator it1(node);
		iterator it2((*node).next);
		for (++length; length; --length)
		{
			_Freenode(it1.node);
			it1 = it2;
			++it2;
		}
	}

	list<_Ty>& operator=(const list<_Ty>& _Right)
	{
		clear();
		iterator first = node;
		iterator second;
		for (iterator it = _Right.begin(); it != _Right.end(); ++it)
		{
			second = _Buynode();
			*second = *it;
			(*(first.node)).next = second.node;
			(*(second.node)).prev = first.node;
			first = second;
		}
		(*(first.node)).next = node;
		(*node).prev = first.node;
		length = _Right.length;
	}

	lisg<_Ty>& operator==(list<_Ty>&& _Right)
	{
		clear();
		(*node).next = (*_Right.node).next;
		(*node).prev = (*_Right.node).prev;
		(*static_cast<link_type>((*(_Right.node)).next)).prev = node;
		(*static_cast<link_type>((*(_Right.node)).prev)).next = node;
		length = _Right.length;
		_Right._Reset();
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
		return *(--(static_cast<iterator>(end)));
	}

	const_reference back() const
	{
		return *(--(static_cast<const_iterator>(end)));
	}

	void swap(list<_Ty> _Right)
	{
		link_type tmp1 = node;
		node = _Right.node;
		_Right.node = tmp1;

		size_type tmp2 = length;
		length = _Right.length;
		_Right.length = tmp2;
	}

	iterator insert(iterator _Pos, const _Ty& _Val=_Ty())
	{
		link_type tmp = _Buynode();
		(*tmp).data = _Val;
		(*tmp).next = _Pos.node;
		(*tmp).prev = (*_Pos.node).prev;
		(*static_cast<link_type>((*_Pos.node).prev)).next = tmp;
		(*_Pos.node).prev = tmp;
		++length;
		return tmp;
	}

	iterator insert(iterator _Pos, size_type _Count, const _Ty& _Val)
	{
		if (_Count == 1)
			return insert(_Pos, _Val);
		link_type first = _Buynode();
		link_type last = _Buynode();
		(*first).prev = (*_Pos.node).prev;
		(*last).next = _Pos.node;
		(*static_cast<link_type>((*_Pos.node).prev)).next = first;
		(*_Pos.node).prev = last;
		link_type now = first;
		for (int i = _Count - 2; i != 0; --i)
		{
			link_type tmp = _Buynode();
			(*now).next = tmp;
			(*tmp).prev = now;
			now = tmp;
		}
		(*now).next = last;
		(*last).prev = now;
		length += _Count;
		return first;
	}

	template<typename Iter>
	iterator insert(iterator _Pos,Iter _First, Iter _Last)
	{
		if (_First == _Last)
			return _Pos;
		link_type first = _Buynode();
		++length;
		(*first).data = *_First;
		(*static_cast<link_type>((*_Pos.node).prev)).next = first;
		(*first).prev = (*_Pos.node).prev;
		link_type now = first;
		for (; ++_First != _Last;)
		{
			link_type tmp = _Buynode();
			(*now).next = tmp;
			(*tmp).prev = now;
			now = tmp;
			++length;
		}
		(*now).next = _Pos.node;
		(*_Pos.node).prev = now;
		return first;
	}

	void push_front(const _Ty& _Val)
	{
		insert(begin(), _Val);
	}

	void push_back(const _Ty& _Val)
	{
		insert(node, _Val);
	}

	iterator erase(iterator _Pos)
	{
		(*static_cast<link_type>((*_Pos.node).prev)).next = (*_Pos.node).next;
		(*static_cast<link_type>((*_Pos.node).next)).prev = (*_Pos.node).prev;
		iterator res (static_cast<link_type>((*_Pos.node).next));
		_Freenode(_Pos);
		--length;
		return res;
	}

	iterator erase(iterator _First, iterator _Last)
	{
		(*static_cast<link_type>((*_First.node).prev)).next = _Last.node;
		(*_Last.node).prev = (*_First.node).prev;
		for (; _First != _Last; ++_First)
		{
			_Freenode(_First);
			--length;
		}
		return _Last;
	}
	
	void resize(size_type _Newsize, const _Ty& _Val=_Ty())
	{
		if (_Newsize == length)
			return;
		else if (_Newsize < length)
		{
			difference_type dif = length - _Newsize;
			iterator it = begin();
			it += _Newsize;
			for (; it != node; ++it)
				_Freenode(it);
			it = begin() + (_Newsize - 1);
			(*it.node).next = node;
			(*node).prev = it.node;
			length = _Newsize;
		}
		else
		{
			difference_type dif = _Newsize - length;
			for (; dif != 0; --dif)
				push_back(_Val);
		}
	}

	void clear()
	{
		iterator d = begin();
		for (iterator it = begin() + 1; it != end(); ++it)
		{
			_Freenode(d);
			d = it;
		}
		_Reset();
	}

	void  pop_front()
	{
		erase(begin());
	}

	void pop_back()
	{
		erase(--end());
	}

	void remove(const _Ty& _Val)
	{
		for (iterator it = begin(); it != end(); )
		{
			if (*it == _Val)
			{
				iterator tmp = it;
				++it;
				_Delnode(tmp);
			}
			else
				++it;
		}
	}

	template<typename _Pr>
	void remove_if(_Pr _Pred)
	{
		for (iterator it = begin(); it != end(); )
		{
			if (_Pred(*it))
			{
				iterator tmp = it;
				++it;
				_Delnode(tmp);
			}
			else
				++it;
		}
	}

	void unique()
	{
		iterator first = begin();
		iterator second = begin() + 1;
		for (; second != end();)
		{
			if (*first == *second)
			{
				iterator it = second;
				++second;
				_Delnode(it);
			}
			++first;
			++second;
		}
	}

	template<typename _Pr>
	void remove_id(_Pr _Pred)
	{
		iterator first = begin();
		iterator second = begin() + 1;
		for (; second != end();)
		{
			if (_Pred(*first,*second))
			{
				iterator it = second;
				++second;
				_Delnode(it);
			}
			++first;
			++second;
		}
	}

	void merge(list<_Ty>& _Right)
	{

	}

	template<typename _Pr>
	void merge(list<_Ty>& _Right, _Pr _Pred)
	{

	}

	void sort()
	{

	}

	template<typename _Pr>
	void sort(_Pr _Pred)
	{

	}

	void reverse()
	{

	}

	void splice(iterator _Pos, list<_Ty>& _Right)
	{

	}

	void splice(iterator _Pos, list<_Ty>& _Right, iterator _Which)
	{

	}

	void splice(iterator _Pos, list<_Ty>& _Right, iterator _From, iterator _End)
	{

	}
};