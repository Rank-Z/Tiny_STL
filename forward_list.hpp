#include<memory>
#include<initializer_list>
#include<utility>
#define _STD ::std::

template<typename _Ty>
struct _Node
{
	_Node* next = nullptr;
	_Ty val;

	template<typename..._Args>
	_Node(_Node* p, _Args&&..._args) :next(p), val(_STD forward<_Args>(_args)...)
	{   }
};


template<typename _Ty>
struct _forward_list_const_iterator
{
	using value_type = _Ty;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using size_type = unsigned;
	using difference_type = int;
	using _Nodeptr = _Node<_Ty>*;


	_Nodeptr node;


	_forward_list_const_iterator() = default;
	_forward_list_const_iterator(void* p) :node(static_cast<_Nodeptr>(p))
	{   }
	_forward_list_const_iterator(const _forward_list_const_iterator<_Ty>& _Right) :node(_Right.node)
	{   }

	bool operator==(const _forward_list_const_iterator& _Right)
	{
		return (node == _Right.node);
	}

	bool operator!=(const _forward_list_const_iterator& _Right)
	{
		return(node != _Right.node);
	}

	_forward_list_const_iterator& operator++()
	{
		node = node->next;
		return (*this);
	}

	_forward_list_const_iterator operator++(int)
	{
		_forward_list_const_iterator tmp(*this);
		node = node->node;
		return tmp;
	}

	const_reference operator*() const
	{
		return (node->val);
	}
};

template<typename _Ty>
struct _forward_list_iterator :public _forward_list_const_iterator<_Ty>
{
	using value_type = _Ty;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using size_type = unsigned;
	using difference_type = int;
	using _Nodeptr = _Node<_Ty>*;


	_forward_list_iterator() = default;
	_forward_list_iterator(void* p) :node(static_cast<_Nodeptr>(p))
	{   }
	_forward_list_iterator(const _forward_list_const_iterator<_Ty>& _Right) :node(_Right.node)
	{   }

	reference operator*()
	{
		return (node->val);
	}

	_forward_list_iterator& operator++()
	{
		node = node->next;
		return *this;
	}

	_forward_list_iterator operator++(int)
	{
		_forward_list_iterator tmp(*this);
		node = node->next;
		return tmp;
	}
};

template<typename _Ty>
class forward_list
{
	using value_type = _Ty;
	using reference = value_type&;
	using const_reference = const reference;
	using iterator = _forward_list_iterator<_Ty>;
	using const_iterator = _forward_list_const_iterator<_Ty>;
	using size_type = unsigned;
	using difference_type = int;
	using allocator_type = _STD allocator<_Node<_Ty>>;
	using _Nodeptr = _Node<_Ty>*;
	using const_Nodeptr = const _Nodeptr;

private:
	_Nodeptr _Head = nullptr;

public:

	forward_list()
	{   }

	forward_list(size_type _Size, const _Ty& _Val)
	{
		_Construct_n(_Size, _Val);
	}

	forward_list(const forward_list& _Right)
	{
		_Nodeptr Rp = _Right.begin();
		if (Rp) :
		{
			_Nodeptr temp = _BuyNode(nullptr, Rp->val);
			_Head = temp;
			Rp = Rp->next;
		}
		while (Rp)
		{
			temp->next = _BuyNode(nullptr, Rp->val);
			temp = temp->next;
			Rp = Rp->next;
		}
	}

	forward_list(forward_list&& _Right)
	{
		_Head = _STD move(_Right._GetHead());
	}

	template<typename Iter>
	forward_list(Iter begin, Iter end)
	{
		_Head = _Construct_from_end(nullptr, begin, end);
	}

	forward_list(_STD initializer_list<_Ty> il) :forward_list(il.begin(), il.end())
	{   }

	forward_list& operator=(const forward_list& _Right)
	{
		_Nodeptr Rp = _Right.begin();
		if (Rp) :
		{
			_Nodeptr temp = _BuyNode(nullptr, Rp->val);
			_Head = temp;
			Rp = Rp->next;
		}
		while (Rp)
		{
			temp->next = _BuyNode(nullptr, Rp->val);
			temp = temp->next;
			Rp = Rp->next;
		}
		return *this;
	}

	forward_list& operator=(forward_list<_Ty>&& _Right)
	{
		_Clear_n(_Head);
		_Head = _STD move(_Right._GetHead());
		return *this;
	}

	forward_list& operator=(_STD initializer_list<_Ty> il)
	{
		_Clear_n(_Head);
		_Head = _Construct_from_end(nullptr, il.begin(), il.end());
		return *this;
	}

	~forward_list()
	{
		_Clear_n(_Head);
	}

	/***********************************************************************************************/
private:
	void _Construct_n(size_type _Size, const _Ty& _Val)
	{
		_Nodeptr temp = nullptr;
		for (int i = 0; i != _Size; ++i)
			temp = _BuyNode(temp, &_Val);
		_Head = temp;
	}

	template<typename..._Args>
	_Nodeptr _BuyNode(_Nodeptr addr, _Args&&..._args)
	{
		return new _Node(addr, _STD forward(_args)...);
	}

	void _Clear_n(_Nodeptr _Where)
	{
		if (_Where->next)
			_Clear_n(_Where->next);
		_Freenode(_Where);
	}

	template<typename Iter>
	_Nodeptr _Construct_from_end(_Nodeptr _Where, Iter begin, Iter end)
	{
		//assuem Iter support operator -- and begin is always less than end
		_Nodeptr temp = _Where->next;
		for (Iter it = end; it > begin, --it)
		{
			temp = _Insert_after(temp, *it);
		}
		return temp;
	}

	size_type _Size()const
	{
		size_type _Counter = 0;
		for (_Nodeptr temp = _Head; temp; ++_Counter)
			temp = temp->next;
		return _Counter;
	}

	_Nodeptr _Before_end()
	{
		_Node_poninter temp = _Head;
		if (temp)
			for (; temp->next; temp = temp->next)
			{
			}
		return temp;
	}

	void _Freenode(_Nodeptr _Where)
	{
		delete _Where;
	}

	_Nodeptr _jump(_Nodeptr _Where, size_type _Size = 1)
	{
		_Nodeptr temp = _Where;
		for (; _Size; --_Size)
			temp = temp->next;
		return temp;
	}

	_Nodeptr& _GetHead()
	{
		return this->_Head;
	}

	struct less
	{
		bool operator()(_Nodeptr _Left, _Nodeptr _Right)
		{
			return _Left->val < _Right->val;
		}
	};
	/************************************************************************************************/
public:

	iterator begin()const
	{
		return _Head;
	}

	const_iterator cbegin()const
	{
		return _Head;
	}

	iterator end()const
	{
		return nullptr;
	}

	const_iterator cend()const
	{
		return nullptr;
	}

	void push_front(const _Ty& _Val)
	{
		_Nodeptr p = new _Node;
		p->val = _Val;
		p->next = _Head;
		_Head = p;
	}

	template<typename..._Args>
	void emplace_front(_Args&&..._args)
	{
		_Head = _BuyNode(nullptr, _STD forward(_args)...);
	}

	template<typename..._Args>
	_Nodeptr _Insert_after(const_iterator _Where, _Args&&..._args)
	{
		return _BuyNode(_Where.node->next, _STD forward(_args)...);
	}

	iterator insert_after(const_iterator _Where, const _Ty& _Val)
	{
		return _Where.node->next = _Insert_after(_Where, _Val);
	}

	iterator insert_after(const_iterator _Where, const size_type _Count, const _Ty& _Val)
	{
		_Nodeptr temp = _Where->next;
		for (int i = 0; i != _Count, ++i)
		{
			temp = _Insert_after(temp, _Val);
		}
		return _Where.node->next = temp;
	}

	template<typename Iter>
	iterator insert_after(const_iterator _Where, Iter begin, Iter end)
	{
		return _Where.node->next = _Construct_from_end(_Where.node, begin, end);
	}

	iterator insert_after(const_iterator _Where, _STD initializer_list<_Ty> il)
	{
		return insert_after(_Where, il.begin(), il.end());
	}

	template<typename..._Args>
	iterator emplace_after(const_iterator _Where, _Args&&..._args)
	{
		return _Where.node->next = _Insert_after(_Where, _STD forward(_args)...);
	}

	void assign(_STD initializer_list<_Ty> il)
	{
		assign(il.begin(), il.end());
	}

	template<typename Iter>
	void assign(Iter begin, Iter end)
	{
		_Clear_n(_Head);
		_Head = _Construct_from_end(nullptr, begin, end);
	}

	void assign(size_type _Count, const _Ty& _Val)
	{
		_Clear_n(_Head);
		insert_after(_Head, _Count, _Val);
	}

	void resize(size_type _Newsize)
	{
		size_type _Cursize = _Size();
		if (_Cursize < _Newsize)
		{
			_Nodeptr temp = nullptr;
			for (int i = _Newsize - _Cursize; 0<i; --i)
			{
				temp = _BuyNode(temp);
			}
			_Before_end->next = temp;
		}
		else if (_Newsize<_Cursize)
		{
			_Nodeptr temp = _Head;
			for (; 0 < _Newsize; --_Newsize)
				temp = temp->next;
			_Clear_n(temp);
		}
	}

	bool empty()const
	{
		return _Head;
	}

	void pop_front()
	{
		if (_Head)
		{
			_Nodeptr temp = _Head->next->next;
			_Freenode(_Head);
			_Head = temp;
		}
	}

	iterator before_begin()noexcept
	{
		return _Head;
	}

	const_iterator before_begin() const noexcept
	{
		return _Head;
	}

	const_iterator cbefore_begin()const noexcept
	{
		return _Head;
	}

	iterator erase_after(const_iterator _Where)
	{
		if (_Where.node == nullptr)
		{
			return nullptr;
		}
		if (_Where.node->next)
		{
			_Nodeptr temp = _Where.node->next->next;
			_Freenode(_Where.node->next);
			return _Where.node->next = temp;
		}
		return nullptr;
	}

	iterator erase_after(const_iterator _First, const_iterator _Last)//earse elements between _First and _Last
	{
		if (!_First.node)
			return nullptr;
		if (_First.node == _Head&&_Last.node == nullptr)
		{
			clear();
		}
		else
		{
			_Nodeptr temp = _First.node->next;
			_Nodeptr next;
			for (; temp != _Last.node; temp = next)
			{
				next = temp->next;
				_Freenode(temp);
			}
			_First.node->next = _Last.node;
		}
		return _Last;
	}

	void clear()
	{
		_Nodeptr temp = _Head;
		_Nodeptr next;
		_Head = nullptr;
		for (; temp != nullptr; temp = next)
		{
			next = temp->next;
			_Freenode(temp);
		}
	}

	void swap(forward_list& _Right)
	{
		if (this != &_Right)
		{
			_Nodeptr temp = _Head;
			_Head = _Right._GetHead();
			_Right._GetHead() = _Head;
		}
	}

	void remove(const _Ty& _Val)
	{
		if (!_GetHead())
			return;
		_Nodeptr temp = before_begin();// before_begin() is inline function
		_Nodeptr _End = end();
		for (_Nodeptr next = temp->next; next != end();)
		{
			if (next->val == _Val)
			{
				if ((&next->val == &_Val))
				{
					_End = temp;
					temp = next;
					next = next->next
				}
				else
					next = erase_after(temp);
			}
			else
				temp = next;
		}
		if (_End != end())
			erase_after(_End);
	}

	template<typename _Pr>
	void remove_if(_Pr _Pred)
	{
		if (!_GetHead())
			return;
		_Nodeptr temp = before_begin();
		for (_Nodeptr next = begin(); next != end();)
			if (_Pred(next->val))
				next = erase_after(temp);
			else
			{
				temp = next;
				next = next->next;
			}
	}

	void unique()
	{
		if (_Nodeptr _First = begin())
		{
			_Nodeptr _After = _First->next;
			for (; _After != end(); )
				if (_First->val == _After->val)
					_After = erase_after(_First);
				else
				{
					_First = _First->next;
					_After = _After->next;
				}
		}
	}

	template<typename _Pr>
	void unique(_Pr _Pred)
	{
		if (_Nodeptr _First = begin())
		{
			_Nodeptr _After = _First->next;
			for (; _After != end(); )
				if (_Pred(_First->val, _After->val))
					_After = erase_after(_First);
				else
				{
					_First = _First->next;
					_After = _After->next;
				}
		}
	}

	void sort()
	{
		_Sort(before_begin().node, end(), less);
	}

	template<typename _Pr>
	void sort(_Pr _Pred)
	{
		_Sort(before_begin().node, end(), _Pred);
	}

	template<typename _Pr>
	void _Sort(_Nodeptr _Before_first, _Nodeptr _Last, _Pr _Pred)
	{
		size_type _Size = 0;
		for (_Nodeptr t = _Before_first->next; t != _Last; ++_Size)
			t = t->next;

		if (_Size < 2)
			return;

		_Nodeptr _Mid = _jump(_Before_first, 1 + _Size / 2);
		_Sort(_Before_first, _Mid);
		_Nodeptr _First = _jump(_Before_first);

		_Nodeptr _Before_mid = _jump(_Before_first, _Size / 2);
		_Sort(_Before_mid, _Last);
		_Mid = _jump(_Before_mid);

		for (;;)
		{
			if (_Pred(_Mid->next, _First->next))
			{
				splice_after(_Before_first, *this, _Before_mid);
				_Before_first = _Before_first->next;
				_Mid = _jump(_Before_mid);
				if (_Mid == _Last)
					return;
			}
			else
			{
				_Before_first = _Before_first->next;
				_First = _First->next;
				if (_First == _Mid)
					return;
			}
		}
	}

	void merge(forward_list& _Right)
	{
		merge(_Right, less);
	}

	void merge(forward_list&& _Right)
	{
		merge(_Right, less);
	}

	template<typename _Pr>
	void merge(forward_list& _Right, _Pr _Pred)
	{
		_Nodeptr _L = begin();
		_Nodeptr _R = _Right.begin();
		if (_Pred(_R->val, _L->val))
		{
			_Head = _R;
			_R = _R->next;
		}
		else
		{
			_Head = _L;
			_L = _L->next;
		}
		_Nodeptr temp = _Head
			for (; _L&&_R; temp = temp->next)
			{
				if (_Pred(_R->val, _L->val))
				{
					temp->next = _R;
					_R = _R->next;
				}
				else
				{
					temp->next = _L;
					_L = _L->next;
				}
			}
		if (_L)
			temp->next = _L;
		else
			temp->next = _R;

		_Right._GetHead() = nullptr;
	}

	template<typename _Pr>
	void merge(forward_list&& _Right, _Pr _Pred)
	{
		merge(_Right, _Pred);
	}

	void _Splice_after(_Nodeptr _Where,
		forward_list& _Right, _Nodeptr _First, _Nodeptr _Last)
	{
		if (_First == _Last || _First->next == _Last)
			return;
		_Nodeptr _begin = _First->next;
		if (_First->next == _Right.begin())
			_Right._GetHead() = _Last;
		else
			_First->next = _Last;
		_Nodeptr _end = _begin;
		for (; _end->next != _Last;)
			_end = _end->next;
		_end->next = _Where->next;
		_Where->next = _begin;
	}

	void splice_after(const_iterator _Where, forward_list& _Right)
	{
		_Splice_after(_Where.node, _Right, _Right.before_begin().node, _Right.end().node);
	}

	void splice_after(const_iterator _Where, forward_list&& _Right)
	{
		_Splice_after(_Where.node, _Right, _Right.before_begin().node, _Right.end().node);
	}

	void splice_after(const_iterator _Where, forward_list& _Right, const_iterator _First)
	{
		_Splice_after(_Where.node, _Right, _First.node, _Right.end().node);
	}

	void splice_after(const_iterator _Where, forward_list&& _Right, const_iterator _First)
	{
		_Splice_after(_Where.node, _Right, _First.node, _Right.end().node);
	}

	void splice_after(const_iterator _Where, forward_list& _Right, const_iterator _First, const_iterator _Last)
	{
		_Splice_after(_Where.node, _Right, _First.node, _Last.node);
	}

	void splice_after(const_iterator _Where, forward_list&& _Right, const_iterator _First, const_iterator _Last)
	{
		_Splice_after(_Where.node, _Right, _First.node, _Last.node);
	}

	void reverse() noexcept
	{
		if (empty())
			return;
		const_iterator _Newfirst(_Before_end());
		for (; begin() != _Newfirst;)
			_Splice_after(_Newfirst, *this, before_begin(), ++begin());
	}
};