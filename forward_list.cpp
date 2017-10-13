#include<memory>
#include<initializer_list>
#include<utility>


template<typename _Ty>
class foward_list
{

	struct _Node
	{
		_Node* next = nullptr;
		_Ty val;
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
		_Node_pointer temp = nullptr;
		while (Rp)
		{
			_Node_pointer p = new _Node;
			p->val = Rp->val;
			if (!_Head)
				_Head =temp= p;
			else:
			{
				temp->next = p;
				temp = p;
			}
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

	}

	forward_list(std::initializer_list<_Ty> il)
	{

	}

	void _Construct_n(size_type _Size, const _Ty _Val)
	{
		_Node_pointer temp = nullptr;
		for (int i = 0; i != _Size; ++i)
		{
			_Node_pointer p = new _Node;
			p->val = _Val;
			if (!_Head)
				_Head =temp= p;
			else:
			{
				temp->next = p;
				temp = p;
			}
		}
	}

	_Node_pointer& _GetHead()
	{
		return _Head;
	}








	void push_front(const _Ty _Val)
	{
		_Node_pointer p = new _Node;
		p->val = _Val;
		p->next = _Head;
		_Head = p;
	}


};