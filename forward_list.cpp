#include<memory>



template<typename Ty,typename Alloc=std::allocator<Ty>>
class foward_list
{


	using value_type = Ty;
	using iterator = Ty*;
	using const_iterator = const Ty *;
	using size_type = unsigned;
	using difference_type = int;

	using allocator_type = Alloc;

private:
	iterator _First = nullptr;
	allocator_type alloc;
	struct _Node
	{
		_Node* next = nullptr;
		value_type val;
	} _HeadNode;

public:

	forward_list()
	{   }

	foward_list(size_type _Size, const Alloc& _Alloc)
	{

	}

	forward_list(size_type _Size, const Ty& _Val)
	{

	}








	push_front(Ty v)
	{

	}


};