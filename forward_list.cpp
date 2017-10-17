#include<memory>
#include<initializer_list>
#include<utility>
#define _STD ::std::

template<typename _Ty>
class forward_list
{

	struct _Node
	{
		_Node* next = nullptr;
		_Ty val;

		template<typename..._Args>
		_Node(_Node* n, _Args&&..._args) :next(n),val(_STD forward<_Args>(_args)...)
		{   }

	};

	using value_type = _Ty;
	using iterator = _Ty*;
	using const_iterator = const _Ty *;
	using size_type = unsigned;
	using difference_type = int;
	using allocator_type = _STD allocator<_Node>;
	using _Nodeptr = _Node*;
	using const_Nodeptr = const _Nodeptr;
private:
	_Node* _Head = nullptr;

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
		if(Rp):
		{
			_Nodeptr temp =_BuyNode(nullptr, Rp->val);
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

	forward_list(_STD initializer_list<_Ty> il):forward_list(il.begin(),il.end())
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

	forward_list& operator=(forward_list&& _Right)
	{
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
	_Nodeptr _BuyNode(_Nodeptr addr,_Args&&..._args)
	{
		return new _Node(addr,_STD forward(_args)...);
	}

	void _Clear_n(_Nodeptr _Where)
	{
		if (_Where->next)
			_Clear_n(_Where->next);
		_Freenode(_Where);
	}

	template<typename Iter>
	_Nodeptr _Construct_from_end(_Nodeptr _Where,Iter begin, Iter end)
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
		if(temp)
			for( ;temp->next;temp=temp->next)
			{   }
		return temp;
	}

	void _Freenode(_Nodeptr _Where)
	{
		delete _Where;
	}

/************************************************************************************************/
public:

	_Nodeptr& _GetHead()
	{
		return _Head;
	}

	_Nodeptr begin()const
	{
		return _Head;
	}

	const _Nodeptr	cbegin()const
	{
		return _Head;
	}

	_Nodeptr end()const
	{
		return nullptr;
	}

	const _Nodeptr cend()const
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
	auto emplace_front(_Args&&..._args)
	{
		_Head = _BuyNode(nullptr, _STD forward(_args)...);
	}

	template<typename..._Args>
	_Nodeptr _Insert_after(_Nodeptr _Where, _Args&&..._args)
	{
		return _BuyNode(_Where->next,_STD forward(_args)...);
	}

	_Nodeptr insert_after(const _Nodeptr _Where, const _Ty& _Val)
	{
		return _Where->next=_Insert_after(_Where, _Val);
	}

	_Nodeptr insert_after(const _Nodeptr _Where, const size_type _Count, const _Ty& _Val)
	{
		_Nodeptr temp = _Where->next;
		for (int i = 0; i != _Count, ++i)
		{
			temp = _Insert_after(temp, _Val);
		}
		return _Where->next=temp;
	}

	template<typename Iter>
	_Nodeptr insert_after(const _Nodeptr _Where, Iter begin, Iter end)
	{
		return _Where->next= _Construct_from_end(_Where, begin, end);
	}

	_Nodeptr insert_after(const _Nodeptr _Where, _STD initializer_list<_Ty> il)
	{
		return insert_after(_Where, il.begin(), il.end());
	}

	template<typename..._Args>
	_Nodeptr emplace_after(const _Nodeptr _Where, _Args&&..._args)
	{
		return _Where->next = _Insert_after(_Where, _STD forward(_args)...);
	}

	void assign(_STD initializer_list<_Ty> il)
	{
		assign(il.begin(), il.end());
	}

	template<typename Iter>
	void assign(Iter begin, Iter end)
	{
		_Clear_n(_Head);
		_Head = _Construct_from_end(nullptr,begin, end);
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
			for (int i = _Newsize - _Cursize;0<i; --i)
			{
				temp = _BuyNode(temp);
			}
			_Before_end->next = temp;
		}
		else if(_Newsize<_Cursize)
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

	_Nodeptr before_begin()const
	{
		return new _Node(_Head);
	}

	const_Nodeptr cbefore_begin()const
	{
		return new _Node(_Head);
	}

	_Nodeptr erase_after(const_Nodeptr _Where)
	{
		if (_Where == nullptr)
		{
			return nullptr;
		}
		if (_Where->next)
		{
			_Nodeptr temp = _Where->next->next;
			_Freenode(_Where->next);
			return _Where->next = temp;
		}
		return nullptr;
	}

	_Nodeptr erase_after(const_Nodeptr _First, const_Nodeptr _Last)//earse elements between _First and _Last
	{
		if (!_First)
			return nullptr;
		if (_First == _Head&&_Last == nullptr)
		{
			clear();
		}
		else
		{
			_Nodeptr temp = _First->next;
			_Nodeptr next;
			for (; temp!=_Last;temp=next)
			{
				next = temp->next;
				_Freenode(temp);
			}
			_First->next = _Last;
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
			_Nodeptr temp=_Head;
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
			for ( ; _After != end(); )
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
				if (_Pred(_First->val,_After->val))
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

	}

	template<typename _Pr>
	void sort(_Pr _Pred)
	{

	}

	void merge(forward_list& _Right)
	{
		_Nodeptr _L = begin();
		_Nodeptr _R = _Right.begin();
		if (_R->val < _L->val)
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
		for ( ;_L&&_R;temp=temp->next)
		{
			if (_R->val < _L-val)
			{
				temp->next = _R;
				_R = _R->next;
			}
			else
			{
				temp->next= _L;
				_L = _L->next;  
			}
		}
		if (_L)
			temp->next = _L;
		else
			temp->next = _R;

		_Right._GetHead() = nullptr;
	}

	void merge(forward_list&& _Right)
	{
		merge(_Right);
	}

	template<typename _Pr>
	void merge(forward_list& _Right, _Pr _Pred)
	{
		_Nodeptr _L = begin();
		_Nodeptr _R = _Right.begin();
		if (_Pred(_R->val,_L->val))
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
				if (_Pred(_R->val,_L->val))
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

	void _Splice_after(const_Nodeptr _Where,
		forward_list& _Right, const_Nodeptr _First, const_Nodeptr _Last)
	{
		if (_First == _Last || _First->next==_Last||)
			return;
		_Nodeptr _begin = _First->next;
		if (_First->next == _Right.begin())
			_Right._GetHead() = _Last;
		else
			_First->next = _Last;
		_Nodeptr _end = _begin;
		for (; _end->next != _Last;)
			end = end->next;
		end->next = _Where->next;
		_Where->next = _begin;
	}

	void splice_after(const_Nodeptr _Where, forward_list& _Right)
	{
		_Splice_after(_Where, _Right, _Right.before_begin(), _Right.end());
	}

	void splice_after(const_Nodeptr _Where, forward_list&& _Right)
	{
		_Splice_after(_Where, _Right, _Right.before_begin(), _Right.end());
	}

	void splice_after(const_Nodeptr _Where, forward_list& _Right, const_Nodeptr _First)
	{
		_Splice_after(_Where, _Right, _First, _Right.end());
	}

	void splice_after(const_Nodeptr _Where, forward_list&& _Right, const_Nodeptr _First)
	{
		_Splice_after(_Where, _Right, _First, _Right.end());
	}

	void splice_after(const_Nodeptr _Where, forward_list& _Right, const_Nodeptr _First, const_Nodeptr _Last)
	{
		_Splice_after(_Where, _Right, _First, _Last);
	}

	void splice_after(const_Nodeptr _Where, forward_list&& _Right, const_Nodeptr _First, const_Nodeptr _Last)
	{
		_Splice_after(_Where, _Right, _First, _Last);
	}

};