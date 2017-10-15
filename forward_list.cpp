#include<memory>
#include<initializer_list>
#include<utility>


template<typename _Ty>
class forward_list
{

	struct _Node
	{
		_Node* next = nullptr;
		_Ty val;

		template<typename..._Args>
		_Node(_Node* n, _Args&&..._args) :next(n),val(std::forward<_Args>(_args)...)
		{   }

	};

	using value_type = _Ty;
	using iterator = _Ty*;
	using const_iterator = const _Ty *;
	using size_type = unsigned;
	using difference_type = int;
	using allocator_type = std::allocator<_Node>;
	using _Node_pointer = _Node*;
	using const_Node_pointer = const _Node_pointer;
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
		_Node_pointer Rp = _Right.begin();
		if(Rp):
		{
			_Node_pointer temp =_BuyNode(nullptr, Rp->val);
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
		_Head = std::move(_Right._GetHead());
	}

	template<typename Iter>
	forward_list(Iter begin, Iter end)
	{
		_Head = _Construct_from_end(nullptr, begin, end);
	}

	forward_list(std::initializer_list<_Ty> il):forward_list(il.begin(),il.end())
	{   }

	forward_list& operator=(const forward_list& _Right)
	{
		_Node_pointer Rp = _Right.begin();
		if (Rp) :
		{
			_Node_pointer temp = _BuyNode(nullptr, Rp->val);
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
		_Head = std::move(_Right._GetHead());
		return *this;
	}

	forward_list& operator=(std::initializer_list<_Ty> il)
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
		_Node_pointer temp = nullptr;
		for (int i = 0; i != _Size; ++i)
			temp = _BuyNode(temp, &_Val);
		_Head = temp;
	}

	template<typename..._Args>
	_Node_pointer _BuyNode(_Node_pointer addr,_Args&&..._args)
	{
		return new _Node(addr,std::forward(_args)...);
	}

	void _Clear_n(_Node_pointer _Where)
	{
		if (_Where->next)
			_Clear_n(_Where->next);
		delete _Where;
	}

	template<typename Iter>
	_Node_pointer _Construct_from_end(_Node_pointer _Where,Iter begin, Iter end)
	{
		//assuem Iter support operator -- and begin is always less than end
		_Node_pointer temp = _Where->next;
		for (Iter it = end; it > begin, --it)
		{
			temp = _Insert_after(temp, *it);
		}
		return temp;
	}

	size_type _Size()const
	{
		size_type _Counter = 0;
		for (_Node_pointer temp = _Head; temp; ++_Counter)
			temp = temp->next;
		return _Counter;
	}

	_Node_pointer _Before_end()
	{
		_Node_poninter temp = _Head;
		if(temp)
			for( ;temp->next;temp=temp->next)
			{   }
		return temp;
	}

/************************************************************************************************/
public:

	_Node_pointer& _GetHead()const
	{
		return _Head;
	}

	_Node_pointer begin()const
	{
		return _Head->next;
	}

	const _Node_pointer	cbegin()const
	{
		return _Head->next;
	}

	_Node_pointer end()const
	{
		return nullptr;
	}

	const _Node_pointer cend()const
	{
		return nullptr;
	}

	void push_front(const _Ty& _Val)
	{
		_Node_pointer p = new _Node;
		p->val = _Val;
		p->next = _Head;
		_Head = p;
	}

	template<typename..._Args>
	auto emplace_front(_Args&&..._args)
	{
		_Head = _BuyNode(nullptr, std::forward(_args)...);
	}

	template<typename..._Args>
	_Node_pointer _Insert_after(_Node_pointer _Where, _Args&&..._args)
	{
		return _BuyNode(_Where->next, std::forward(_args)...);
	}

	_Node_pointer insert_after(const _Node_pointer _Where, const _Ty& _Val)
	{
		return _Where->next=_Insert_after(_Where, _Val);
	}

	_Node_pointer insert_after(const _Node_pointer _Where, const size_type _Count, const _Ty& _Val)
	{
		_Node_pointer temp = _Where->next;
		for (int i = 0; i != _Count, ++i)
		{
			temp = _Insert_after(temp, _Val);
		}
		return _Where->next=temp;
	}

	template<typename Iter>
	_Node_pointer insert_after(const _Node_pointer _Where, Iter begin, Iter end)
	{
		return _Where->next= _Construct_from_end(_Where, begin, end);
	}

	_Node_pointer insert_after(const _Node_pointer _Where, std::initializer_list<_Ty> il)
	{
		return insert_after(_Where, il.begin(), il.end());
	}

	template<typename..._Args>
	_Node_pointer emplace_after(const _Node_pointer _Where, _Args&&..._args)
	{
		return _Where->next = _Insert_after(_Where, std::forward(_args)...);
	}

	void assign(std::initializer_list<_Ty> il)
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
			_Node_pointer temp = nullptr;
			for (int i = _Newsize - _Cursize;0<i; --i)
			{
				temp = _BuyNode(temp);
			}
			_Before_end->next = temp;
		}
		else if(_Newsize<_Cursize)
		{
			_Node_pointer temp = _Head;
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
			_Node_pointer temp = _Head->next->next;
			delete _Head;
			_Head = temp;
		}
	}

	_Node_pointer before_begin()const
	{
		return _Head;
	}

	_Node_pointer cbefore_begin()const
	{
		return _Head;
	}

	_Node_pointer earse_after(const_Node_pointer _Where)
	{
		if (_Where == nullptr)
		{
			return nullptr;
		}
		if (_Where->next)
		{
			_Node_pointer temp = _Where->next->next;
			delete _Where->next;
			return _Where->next = temp;
		}
		return nullptr;
	}

	_Node_pointer earse_after(const_Node_pointer _First, const_Node_pointer _Last)//earse elements between _First and _Last
	{
		if (!_First)
			return nullptr;
		if (_First == _Head&&_Last == nullptr)
		{
			clear();
		}
		else
		{
			_Node_pointer temp = _First->next;
			for (; temp!=_Last;)
			{
				_Node_pointer next = temp->next;
				delete temp;
				temp = next;
			}
			_First->next = _Last;
		}
		return _Last;
	}

};