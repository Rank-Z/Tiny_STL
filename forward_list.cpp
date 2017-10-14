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

		_Node(_Node* n, _Ty v)
		{
			next = n;
			val = v;
		}
	};
	using value_type = _Ty;
	using iterator = _Ty*;
	using const_iterator = const _Ty *;
	using size_type = unsigned;
	using difference_type = int;
	using allocator_type = std::allocator<_Node>;
	using _Node_pointer = _Node*;

private:
	_Node* _Head = nullptr;

public:

	forward_list()
	{   }

	forward_list(size_type _Size, const _Ty _Val)
	{
		_Construc_n(_Size, _Val);
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
		if (begin == end)
			return;
		_Head = new _Node(nullptr, *begin);
		_Node_pointer temp = _Head;
		for (++begin; begin != end; ++begin)
		{
			temp->next = new _Node(nullptr, *begin);
			temp = temp->next;
		}
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
	}

/*********************************************************************************/

	void _Construct_n(size_type _Size, const _Ty _Val)
	{
		_Node_pointer temp = nullptr;
		for (int i = 0; i != _Size; ++i)
			temp = _BuyNode(temp, _Val);
		_Head = temp;
	}
	template<typename..._Args>
	_Node_pointer _BuyNode(_Node_pointer addr,_Args..._args)
	{
		return new _Node(addr,std::forward(_args)...);
	}

	_Node_pointer& _GetHead()const
	{
		return _Head;
	}

/*********************************************************************************/
	
	_Node_pointer begin()const
	{
		return _Head;
	}

	const _Node_pointer	cbegin()const
	{
		return _Head;
	}

	_Node_pointer end()const
	{
		return nullptr;
	}

	template<typename..._Args>
	_Node_pointer _Insert_after(_Node_pointer _Where, _Args&&..._args)
	{
		return _Where->next=_BuyNode(_Where->next, std::forward(_args)...);
	}

	const _Node_pointer cend()const
	{
		return nullptr;
	}

	void push_front(const _Ty _Val)
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

	_Node_pointer insert_after(const _Node_pointer _Where, const _Ty _Val)
	{
		return _Insert_after(_Where, _Val);
	}

	_Node_pointer insert_after(const _Node_pointer _Where, const size_type _Count, const _Ty _Val)
	{
		_Node_pointer temp = _Where->next;
		for (int i = 0; i != _Count, ++i)
		{
			temp = _Insert_after(temp, _Val);
		}
		return temp;
	}

	template<typename Iter>
	_Node_pointer insert_after(const _Node_pointer _Where, Iter begin, Iter end)
	{
		//assuem Iter support operator -- and begin is always less than end 
		_Node_pointer temp = _Where->next;
		for (Iter it = end; p >= begin; --it)
		{
			temp = _Insert_after(temp, *it);
		}
		return temp;
	}

	_Node_pointer insert_after(const _Node_pointer _Where,const std::initializer_list<_Ty> il)
	{
		return insert_after(_Where, il.begin(), il.end());
	}
};