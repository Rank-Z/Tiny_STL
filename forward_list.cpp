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

public:

	forward_list()
	{   }











	push_front(Ty v)
	{

	}


};