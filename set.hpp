#ifndef SET_HPP
#define SET_HPP

#include"RB_Tree.hpp"
#include<memory>
#include<functional>
#include<stdexcept>
#include<iterator>
#include<algorithm>
#include<utility>
#define _STD ::std::

template<typename Key>
class set_node
{
public:
	using key_type = Key;
	using value_type = key_type;

	set_node(value_type* k = nullptr , set_node* p = nullptr , set_node* l = nullptr , set_node* r = nullptr)
		:kp(k) , par(p) , left(l) , right(r) , isred(false)
	{   }

	key_type& getkey()
	{
		return *vp;
	}

	value_type& getvalue()
	{
		return *vp;
	}

	value_type* vp;
	bool isred;
	set_node* par;
	set_node* left;
	set_node* right;
};

template<typename Key ,
	typename Compare ,
	typename Allocator>
	class set_iterator
{
public:
	using key_type = Key;
	using value_type = key_type;

private:

	using nodetype = set_node<key_type>;
	using nodeptr = nodetype * ;
	using treetype = RB_Tree<nodetype , set_iterator , Compare , Allocator>;
	using treeptr = treetype * ;
	friend class treetype;
public:
	set_iterator() = default;

	set_iterator(nodeptr n , treeptr t)
		:node(n) , tree(t)
	{   }

	set_iterator(const set_iterator& right)
		:node(right.node) , tree(right.tree)
	{   }

	set_iterator& operator=(const set_iterator& right)
	{
		node = right.node;
		tree = right.tree;
	}

	const value_type& operator*() const
	{
		if (tree->getnil() == node)
			throw _STD out_of_range { "set_iterator out_of_range!" };
		return node->getvalue();
	}

	set_iterator& operator++()
	{
		if (node == tree->getnil())
			throw _STD out_of_range { "set_iterator out_of_range!" };
		node = tree->successor(node);
		return *this;
	}

	set_iterator operator++(int)
	{
		set_iterator ret(*this);
		++(*this);
		return ret;
	}

	set_iterator& operator--()
	{
		if (node == tree->getnil())
		{
			if (tree->empty())
				throw _STD out_of_range { "set_iterator out_of_range!" };
			node = tree->last();
		}
		else
			node = tree->predecessor(node);

		return *this;
	}

	set_iterator operator--(int)
	{
		set_iterator ret(*this);
		--(*this);
		return ret;
	}

	bool operator==(const set_iterator& right) const
	{
		return ((node == right.node) && (tree == right.tree));
	}

	bool operator!=(const set_iterator& right) const
	{
		return !(*this == right);
	}

	nodeptr operator->() const
	{
		return node;
	}

private:
	nodeptr node;
	treeptr tree;

};

template<typename Key ,
	typename Compare = _STD less<Key> ,
	typename Allocator = _STD allocator<Key>>
	class set
{
public:
	using key_type = Key;
	using value_type = Key;
	using size_type = unsigned int;
	using difference_type = int;
	using key_compare = Compare;
	using value_compare = Compare;
	using allocator_type = Allocator;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename _STD allocator_traits<Allocator>::pointer;
	using const_pointer = typename _STD allocator_traits<Allocator>::const_pointer;
	using iterator = set_iterator<value_type , Compare , Allocator>;
	using const_iterator = iterator;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
	using node_type = set_node<key_type>;//since C++ 17

private:
	using treetype = RB_Tree<set_node<key_type> , iterator , Compare , Allocator>;
	using nodeptr = node_type * ;
public:

	set() :set(Compare())
	{   }

	explicit set(const Compare& comp , const Allocator& alloc = Allocator())
		:_tree(comp , alloc)
	{   }

	explicit set(const Allocator& alloc)
		:_tree(alloc)
	{   }

	template<typename Iter>
	set(Iter first , Iter last ,
		const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		: _tree(comp , alloc)
	{
		_insert_range(first , last);
	}

	template<typename Iter>
	set(Iter first , Iter last ,
		const Allocator& alloc)
		: set(first , last , Compare() , alloc)
	{   }

	set(const set& right)
		:_tree(right._tree)
	{   }

	set(const set& right , const Allocator& alloc)
		:_tree(right._tree , alloc)
	{   }

	set(set&& right)
		:_tree(_STD move(right._tree))
	{   }

	set(set&& right , const Allocator& alloc)
		:_tree(_STD move(right._tree) , alloc)
	{   }

	set(_STD initializer_list<value_type> il ,
		const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		:_tree(comp , alloc)
	{
		_insert_range(il.begin() , il.end());
	}

	set(_STD initializer_list<value_type> il , const Allocator& alloc)
		:set(il , Compare() , alloc)
	{   }

	~set()
	{   }

	set& operator=(const set& right)
	{
		clear();
		_tree = right._tree;
		return *this;
	}

	set& operator=(set&& right)
		noexcept(_STD allocator_traits<Allocator>::is_always_equal::value&&_STD is_nothrow_move_assignable<Compare>::value)//since C++ 17
	{
		clear();
		_tree = _STD move(right._tree);
		return *this;
	}

	set& operator=(_STD initializer_list<value_type> il)
	{
		clear();
		_insert_range(il.begin() , il.end());
		return *this;
	}

	allocator_type& get_allocator() const
	{
		return _tree.get_allocator();
	}

	iterator begin() noexcept
	{
		return _tree.begin();
	}

	const_iterator begin() const noexcept
	{
		return _tree.begin();
	}

	const_iterator cbegin() const noexcept
	{
		return _tree.begin();
	}

	iterator end() noexcept
	{
		return _tree.end();
	}

	const_iterator end() const noexcept
	{
		return _tree.end();
	}

	const_iterator cend() const noexcept
	{
		return _tree.end();
	}

	reverse_iterator rbegin() noexcept
	{
		return _tree.end();
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return _tree.end();
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return _tree.end();
	}

	reverse_iterator rend() noexcept
	{
		return _tree.begin();
	}

	const_reverse_iterator rend() const noexcept
	{
		return _tree.begin();
	}

	const_reverse_iterator crend() const noexcept
	{
		return _tree.begin();
	}

	bool empty() const noexcept
	{
		return _tree.empty();
	}

	size_type size() const noexcept
	{
		return _tree.size();
	}

	size_type max_size() const noexcept
	{
		return 357913941;
	}

	void clear()
	{
		_tree.clear()
	}

	_STD pair<iterator , bool> insert(const value_type& value)
	{
		return _tree.set_insert_unique(value);
	}

	_STD pair<iterator , bool> insert(value_type&& value)
	{
		return _tree.set_insert_unique(_STD move(value));
	}

	iterator insert(const_iterator hint , const value_type& value)
	{
		if ((!_tree.key_comp()(*hint , value)) && (!_tree.key_comp()(value , *hint)))
			return hint;
		else
			return insert(value);
	}

	iterator insert(const_iterator hint , value_type&& value)
	{
		if ((!_tree.key_comp()(*hint , value)) && (!_tree.key_comp()(value , *hint)))
			return hint;
		else
			return insert(_STD move(value));
	}

	template<typename Iter>
	void insert(Iter first , Iter last)
	{
		for (; first != last; ++first)
			insert(*first);
	}

	void insert(_STD initializer_list<value_type> il)
	{
		insert(il.begin() , il.end());
	}

	template<typename...Args>
	_STD pair<iterator , bool> emplace(Args&&...args)
	{
		value_type* vp = _tree.get_allocator().allocate(1);
		_tree.get_allocator().construct(vp , _STD forward<Args>(args)...);
		_STD pair<iterator , bool> ret(_tree.set_insert_unique(_STD move(*vp)));
		if (ret.second == false)
			_tree.get_allocator().deallocate(vp , 1);

		return ret;
	}

	template<typename...Args>
	iterator emplace_hint(const_iterator hint , Args&&...args)
	{
		value_type* vp = _tree.get_allocator().allocate(1);
		_tree.get_allocator().construct(vp , _STD forward<Args>(args)...);

		if ((!_tree.key_comp()(*hint , *vp)) && (!_tree.key_comp()(*vp , *hint)))
		{
			_tree.get_allocator().deallocate(vp , 1);
			return hint;
		}

		_STD pair<iterator , bool> ret(_tree.set_insert_unique(_STD move(*vp)));
		if (ret.second == false)
			_tree.get_allocator().deallocate(vp , 1);
			
		return ret.first;
	}

	iterator erase(iterator pos)//since C++17 argument type is iterator ,const_iterator before
	{
		return _tree.erase(pos);
	}

	iterator erase(const_iterator first , const_iterator last)
	{
		return _tree.erase(first , last);
	}

	size_type erase(const key_type& key)
	{
		return _tree.deletekey(key);
	}

	void swap(set& right)
		//noexcept(_STD allocator_traits<Allocator>::is_always_equal::value&&_STD is_nothrow_swappable<Compare>::value)//since C++17
	{
		_STD swap(_tree , right._tree);
	}

	size_type count(const key_type& key) const
	{
		if (_tree.find(key) == nullptr)
			return 0;
		else
			return 1;
	}

	iterator find(const key_type& key)
	{
		return _tree.make_iter(_tree.find(key));
	}

	const_iterator find(const key_type& key) const
	{
		return _tree.make_iter(_tree.find(key));
	}

	_STD pair<iterator , iterator> equal_range(const key_type& key)
	{
		return _STD make_pair(lower_bound(key) , upper_bound(key));
	}

	_STD pair<const_iterator , const_iterator> equal_range(const key_type& key) const
	{
		return _STD make_pair(lower_bound(key) , upper_bound(key));
	}

	iterator lower_bound(const key_type& key)
	{
		return _tree.lower_bound(key);
	}

	const_iterator lower_bound(const key_type& key) const
	{
		return _tree.lower_bound(key);
	}

	iterator upper_bound(const key_type& key)
	{
		return _tree.upper_bound(key);
	}

	const_iterator upper_bound(const key_type& key) const
	{
		return _tree.upper_bound(key);
	}

	key_compare key_comp() const
	{
		return _tree.key_comp();
	}

	value_compare value_comp() const
	{
		return _tree.key_comp();
	}

private:

	template<typename Iter>
	void _insert_range(Iter first , Iter last)
	{
		for (; first != last; ++first)
			_tree.set_insert_unique(*first);
	}

	treetype _tree;
};

template<typename Key , typename Compare , typename Allocator>
bool operator==(const set<Key , Compare , Allocator>& left ,
	const set<Key , Compare , Allocator>& right)
{
	if (left.size() != right.size())
		return false;
	auto lit = left.cbegin();
	auto rit = right.cbegin();
	for (; lit != left.cend();)
	{
		if (*lit==*rit)
		{
			++lit;
			++rit;
		}
		else
			return false;
	}
	return true;
}

template<typename Key , typename Compare , typename Allocator>
bool operator!=(const set<Key , Compare , Allocator>& left ,
	const set<Key , Compare , Allocator>& right)
{
	return !(left == right);
}

template<typename Key , typename Compare , typename Allocator>
bool operator<(const set<Key , Compare , Allocator>& left ,
	const set<Key , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(left.begin() , left.end() , right.begin() , right.end() , left.key_comp());
}

template<typename Key , typename Compare , typename Allocator>
bool operator>(const set<Key , Compare , Allocator>& left ,
	const set<Key , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(right.begin() , right.end() , left.begin() , left.end() , left.key_comp());
}

template<typename Key , typename Compare , typename Allocator>
bool operator<=(const set<Key , Compare , Allocator>& left ,
	const set<Key , Compare , Allocator>& right)
{
	return !(left > right);
}

template<typename Key , typename Compare , typename Allocator>
bool operator>=(const set<Key , Compare , Allocator>& left ,
	const set<Key , Compare , Allocator>& right)
{
	return !(left < right);
}

template<typename Key , typename Compare , typename Allocator>
void swap(set<Key , Compare , Allocator>& left ,
	set<Key , Compare , Allocator>& right) noexcept(noexcept(left.swap(right)))//since C++ 17
{
	left.swap(right);
}


template<typename Key ,
	typename Compare = _STD less<Key> ,
	typename Allocator = _STD allocator<Key>>
	class multiset
{
public:
	using key_type = Key;
	using value_type = Key;
	using size_type = unsigned int;
	using difference_type = int;
	using key_compare = Compare;
	using value_compare = Compare;
	using allocator_type = Allocator;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename _STD allocator_traits<Allocator>::pointer;
	using const_pointer = typename _STD allocator_traits<Allocator>::const_pointer;
	using iterator = set_iterator<value_type , Compare , Allocator>;
	using const_iterator = iterator;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
	using node_type = set<key_type>;//since C++17

private:
	using treetype = RB_Tree<set_node<key_type> , iterator , Compare , Allocator>;
	using nodeptr = node_type * ;

public:
	multiset() :multiset(Compare())
	{   }

	explicit multiset(const Compare& comp , const Allocator& alloc = Allocator())
		:_tree(comp , alloc)
	{   }

	explicit multiset(const Allocator& alloc)
		:_tree(alloc)
	{   }

	template<typename Iter>
	multiset(Iter first , Iter last , const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		: _tree(comp , alloc)
	{
		_insert_range(first , last);
	}

	template<typename Iter>
	multiset(Iter first , Iter last , const Allocator& alloc)
		: multiset(first , last , Compare() , alloc)
	{   }

	multiset(const multiset& right)
		:_tree(right._tree)
	{   }

	multiset(const multiset& right , const Allocator& alloc)
		:_tree(right._tree , alloc)
	{   }

	multiset(multiset&& right)
		:_tree(_STD move(right._tree))
	{   }

	multiset(multiset&& right , const Allocator& alloc)
		:_tree(_STD move(right._tree) , alloc)
	{   }

	multiset(_STD  initializer_list<key_type> il , const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		:_tree(comp , alloc)
	{
		_insert_range(il.begin() , il.end());
	}

	multiset(_STD initializer_list<key_type> il , const Allocator& alloc)
		:multiset(il , Compare() , alloc)
	{   }

	~multiset()
	{   }

	multiset& operator=(const multiset& right)
	{
		clear();
		_tree = right._tree;
		return *this;
	}

	multiset& operator=(multiset&& right)
		noexcept(_STD allocator_traits<Allocator>::is_always_equal::value&&_STD is_nothrow_move_assignable<Compare>::value)
	{
		clear();
		_tree = _STD move(right._tree);
		return *this;
	}

	multiset(_STD initializer_list<value_type> il)
	{
		clear();
		_insert_range(il.begin() , il.end());
		return *this;
	}

	allocator_type get_allocator() const
	{
		return _tree.get_allocator();
	}

	iterator begin() noexcept
	{
		return _tree.begin();
	}

	const_iterator begin() const noexcept
	{
		return _tree.begin();
	}

	const_iterator cbegin() const noexcept
	{
		return _tree.begin();
	}

	iterator end() noexcept
	{
		return _tree.end();
	}

	const_iterator end() const noexcept
	{
		return _tree.end();
	}

	const_iterator cend() const noexcept
	{
		return _tree.end();
	}

	reverse_iterator rbegin() noexcept
	{
		return _tree.end();
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return _tree.end();
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return _tree.end();
	}

	reverse_iterator rend() noexcept
	{
		return _tree.begin();
	}

	const_reverse_iterator rend() const noexcept
	{
		return _tree.begin();
	}

	const_reverse_iterator crend() const noexcept
	{
		return _tree.begin();
	}

	bool empty() const noexcept
	{
		return _tree.empty();
	}

	size_type size() const noexcept
	{
		return _tree.size();
	}

	size_type max_size() const noexcept
	{
		return 357913941;
	}

	void clear()
	{
		_tree.clear()
	}

	iterator insert(const value_type& value)
	{
		return _tree.insert_multiable(value);
	}

	iterator insert(value_type&& value)
	{
		return _tree.insert_multiable(_STD move(value));
	}

	iterator insert(const_iterator hint , const value_type& value)
	{
		return _tree.insert_multiable(value);
	}

	iterator insert(const_iterator hint , value_type&& value)
	{
		return _tree.insert_multiable(_STD move(value));
	}

	template<typename Iter>
	void insert(Iter first , Iter last)
	{
		_insert_range(first , last);
	}

	void insert(_STD initializer_list<value_type> il)
	{
		_insert_range(il.begin() , il.end());
	}

	template<typename...Args>
	iterator emplace(Args&&...args)
	{
		value_type* vp = _tree.get_allocator().allocate(1);
		_tree.get_allocator().construct(vp , _STD forward<Args>(args)...);
		return _tree.insert_multiable(_STD move(*vp));
	}

	template<typename...Args>
	iterator emplace_hint(const_iterator hint , Args&&...args)
	{
		return emplace(_STD forward<Args>(args)...);
	}

	iterator erase(iterator pos)//since C++17 argument type is iterator ,const_iterator before
	{
		return _tree.erase(pos);
	}

	iterator erase(const_iterator first , const_iterator last)
	{
		return _tree.erase(first , last);
	}

	size_type erase(const key_type& key)
	{
		iterator it(_tree.make_iter(_tree.find(key)));
		int c = _tree.count(key);
		for (int i = 0; i != c; ++i)
		{
			iterator temp = it;
			++it;
			erase(temp);
		}
		return c;
	}

	void swap(multiset& right)
		//noexcept(_STD allocator_traits<Allocator>::is_always_equal::value&&_STD is_nothrow_swappable<Compare>::value)//since C++17
	{
		_STD swap(_tree , right._tree);
	}

	size_type count(const key_type& key) const
	{
		return _tree.count(key);
	}

	iterator find(const key_type& key)
	{
		return _tree.make_iter(_tree.find(key));
	}

	const_iterator find(const key_type& key) const
	{
		return _tree.make_iter(_tree.find(key));
	}

	_STD pair<iterator , iterator> equal_range(const key_type& key)
	{
		return _STD make_pair(lower_bound(key) , upper_bound(key));
	}

	_STD pair<const_iterator , const_iterator> equal_range(const key_type& key) const
	{
		return _STD make_pair(lower_bound(key) , upper_bound(key));
	}

	iterator lower_bound(const key_type& key)
	{
		return _tree.lower_bound(key);
	}

	const_iterator lower_bound(const key_type& key) const
	{
		return _tree.lower_bound(key);
	}

	iterator upper_bound(const key_type& key)
	{
		return _tree.upper_bound_multiable(key);
	}

	const_iterator upper_bound(const key_type& key) const
	{
		return _tree.upper_bound_multiable(key);
	}

	key_compare key_comp() const
	{
		return _tree.key_comp();
	}

	value_compare value_comp() const
	{
		return _tree.key_comp();
	}

private:

	template<typename Iter>
	void _insert_range(Iter first , Iter last)
	{
		for (; first != last; ++first)
			_tree.insert_multiable(*first);
	}

	treetype _tree;
};


template<typename Key , typename Compare , typename Allocator>
bool operator==(const multiset<Key , Compare , Allocator>& left ,
	const multiset<Key , Compare , Allocator>& right)
{
	if (left.size() != right.size())
		return false;
	auto lit = left.cbegin();
	auto rit = right.cbegin();
	for (; lit != left.cend();)
	{
		if (*lit==*rit)
		{
			++lit;
			++rit;
		}
		else
			return false;
	}
	return true;
}

template<typename Key , typename Compare , typename Allocator>
bool operator!=(const multiset<Key , Compare , Allocator>& left ,
	const multiset<Key , Compare , Allocator>& right)
{
	return !(left == right);
}

template<typename Key , typename Compare , typename Allocator>
bool operator<(const multiset<Key , Compare , Allocator>& left ,
	const multiset<Key , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(left.begin() , left.end() , right.begin() , right.end() , left.key_comp());
}

template<typename Key , typename Compare , typename Allocator>
bool operator>(const multiset<Key , Compare , Allocator>& left ,
	const multiset<Key , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(right.begin() , right.end() , left.begin() , left.end() , left.key_comp());
}

template<typename Key , typename Compare , typename Allocator>
bool operator<=(const multiset<Key , Compare , Allocator>& left ,
	const multiset<Key , Compare , Allocator>& right)
{
	return !(left > right);
}

template<typename Key , typename Compare , typename Allocator>
bool operator>=(const multiset<Key , Compare , Allocator>& left ,
	const multiset<Key , Compare , Allocator>& right)
{
	return !(left < right);
}

template<typename Key , typename Compare , typename Allocator>
void swap(multiset<Key , Compare , Allocator>& left ,
	multiset<Key , Compare , Allocator>& right) noexcept(noexcept(left.swap(right)))
{
	left.swap(right);
}

#endif // ! SET_HPP