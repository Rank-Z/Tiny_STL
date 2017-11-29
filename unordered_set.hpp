#include<xhash>
#include<utility> //pair<  ,  >
//#include<type_traits>
#include<vector>
#include<memory>
#define _STD ::std::
#define _Member_Data_
#define _Member_Data_End_

template<typename Key>
struct uset_node
{
	Key key;
	uset_node* next = nullptr;
	uset_node* prev = nullptr;
	bool used = false;
};

template<typename Key>
struct uset_const_iterator
{
	using value_type = Key;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using size_type = unsigned;
	using difference_type = int;
	using node_type = uset_node<Key>;

_Member_Data_
		uset_node<Key>* np = nullptr;
_Member_Data_End_

		uset_const_iterator() = default;

	uset_const_iterator(void* p) :np(static_cast<node_type*>(p))
	{   }

	uset_const_iterator(const uset_const_iterator<Key>& _Right) :np(_Right.np)
	{   }

	bool operator ==(const uset_const_iterator<Key>& _Right) const
	{
		return (np == _Right.np);
	}

	bool operator !=(const uset_const_iterator<Key>& _Right) const
	{
		return (np != _Right.np);
	}

	uset_const_iterator& operator++()
	{
		//TODO:: exception
		np = np->next;
		return *this;
	}

	uset_const_iterator operator++(int)
	{
		//TODO:: exception
		uset_const_iterator tmp(*this);
		np = np->next;
		return tmp;
	}

	uset_const_iterator& operator--()
	{
		//TODO:: exception
		np = np->prev;
		return *this;
	}

	uset_const_iterator operator--(int)
	{
		//TODO:: exception
		uset_const_iterator tmp(*this);
		np = np->prev;
		return tmp;
	}

	const_reference operator*() const
	{
		return np->key;
	}
};

template<typename Key>
struct uset_iterator :public uset_const_iterator<Key>
{
	using value_type = Key;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using size_type = unsigned;
	using difference_type = int;
	using node_type = uset_node<Key>;

	uset_iterator() = default;

	uset_iterator(void* p) :np(static_cast<node_type*>(p))
	{   }

	uset_iterator(const uset_iterator& _Right) :np(_Right.np)
	{   }

};

template<	typename Key,
	typename Hash = _STD hash<Key>,
	typename Pred = _STD equal_to<Key>,
	typename Alloc = _STD allocator<Key>>
	class unordered_set
{
public:
	using key_type = Key;
	using value_type = key_type;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using node = uset_node<Key>;
	using pointer = typename _STD allocator_traits<Alloc>::pointer;				//just value_type*
	using const_pointer = typename _STD allocator_traits<Alloc>::const_pointer;	//just const value_type*
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	//using iterator=

private:
	_STD vector<node> _buckets;
	size_t _size;

public:

};


