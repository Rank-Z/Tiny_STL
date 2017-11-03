



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

	bool operator==(const iteartor& it) const { return node == x.node; }
	bool operator!=(const iterator& it) const { return node != x.node; }


};

template<typename _Ty>
struct _list_const_iterator
{

};