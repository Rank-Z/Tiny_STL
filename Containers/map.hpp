#ifndef  CONTAINERS_MAP_HPP
#define	 CONTAINERS_MAP_HPP

#include"RB_Tree.hpp"
#include<functional>
#include<memory>
#include<stdexcept>
#include<algorithm>
#include<utility>
#include<iterator>
#define _STD ::std::

template<typename Key , typename T>
class map_node
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const key_type , mapped_type>;

	map_node(value_type* v = nullptr , map_node* p = nullptr , map_node* l = nullptr , map_node* r = nullptr)
		:vp(v) , par(p) , left(l) , right(r) , isred(false)
	{   }

	key_type& getkey()
	{
		return (*vp).first;
	}

	mapped_type& getmapped()
	{
		return (*vp).second;
	}

	value_type& getvalue()
	{
		return *vp;
	}


	value_type * vp;
	bool isred;
	map_node* par;
	map_node* left;
	map_node* right;
};

template<typename Key ,
	typename T ,
	typename Compare ,
	typename Allocator>
	class map_const_iterator
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const key_type , mapped_type>;

private:
	using nodetype = map_node<key_type , mapped_type>;
	using nodeptr = nodetype * ;
	using treetype = RB_Tree<nodetype , map_const_iterator , Compare , Allocator>;
	using treeptr = treetype * ;
	friend class treetype;

public:
	map_const_iterator() = default;

	map_const_iterator(nodeptr n , treeptr t)
		:node(n) , tree(t)
	{   }

	map_const_iterator(const map_const_iterator& right)
		:node(right.node) , tree(right.tree)
	{   }

	map_const_iterator& operator=(const map_const_iterator& right)
	{
		node = right.node;
		tree = right.tree;
		return *this;
	}

	const value_type& operator*() const
	{
		if (tree->getnil() == node)
			throw _STD out_of_range { "map_iterator out_of_range!" };
		return node->getvalue();
	}

	map_const_iterator& operator++()
	{
		if (node == tree->getnil())
			throw _STD out_of_range { "map_iterator out_of_range!" };
		node = tree->successor(node);
		return *this;
	}

	map_const_iterator operator++(int)
	{
		map_const_iterator ret(*this);
		++(*this);
		return ret;
	}

	map_const_iterator& operator--()
	{
		if (node == tree->getnil())
		{
			if (tree->empty())
				throw _STD out_of_range { "map_iterator out_of_range!" };
			node = tree->last();
		}
		else
			node = tree->predecessor(node);

		return *this;
	}

	map_const_iterator operator--(int)
	{
		map_const_iterator ret(*this);
		--(*this);
		return ret;
	}

	bool operator==(const map_const_iterator& right) const
	{
		return ((node == right.node) && (tree == right.tree));
	}

	bool operator!=(const map_const_iterator& right) const
	{
		return !(*this == right);
	}

	nodeptr operator->() const
	{
		return node;
	}

protected:
	nodeptr node;
	treeptr tree;
};


template<typename Key ,
	typename T ,
	typename Compare ,
	typename Allocator>
	class map_iterator :public map_const_iterator<Key , T , Compare , Allocator>
{
public:
	using map_const_iterator::map_const_iterator;

	value_type& operator*()
	{
		if (tree->getnil() == node)
			throw _STD out_of_range { "map_iterator out_of_range!" };
		return node->getvalue();
	}
};


template<typename Key ,
	typename T ,
	typename Compare = _STD less<Key> ,
	typename Allocator = _STD allocator<_STD pair<const Key , T>>>
	class map
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const key_type , mapped_type>;
	using size_type = unsigned;
	using difference_type = int;
	using key_compare = Compare;
	using allocator_type = Allocator;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename _STD allocator_traits<Allocator>::pointer;
	using const_pointer = typename _STD allocator_traits<Allocator>::const_pointer;
	using iterator = map_iterator<key_type , mapped_type , key_compare , allocator_type>;
	using const_iterator = map_const_iterator<key_type , mapped_type , key_compare , allocator_type>;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
	using node_type = map_node<key_type , mapped_type>;//since C++ 17

private:
	using treetype = RB_Tree<map_node<key_type , mapped_type> , iterator , Compare , Allocator>;
	using nodeptr = node_type * ;

public:
	class value_compare
	{
		friend class map<key_type , mapped_type , key_compare , allocator_type>;
	public:
		bool operator()(const value_type& left , const value_type& right) const
		{
			return comp(left.first , right.first);
		}
	protected:
		value_compare(Compare c)
			:comp(c)
		{   }

		Compare comp;
	};

	map()
		:map(Compare())
	{   }

	explicit map(const Compare& comp , const Allocator& alloc = Allocator())
		:_tree(comp , alloc)
	{   }

	explicit map(const Allocator& alloc)
		:map(Compare() , alloc)
	{   }

	template<typename Iter>
	map(Iter first , Iter last , const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		: _tree(comp , alloc)
	{
		_insert_range(first , last);
	}

	template<typename Iter>
	map(Iter first , Iter last , const Allocator& alloc)
		: map(first , last , Compare() , alloc)
	{   }

	map(const map& right)
		:_tree(right._tree)
	{   }

	map(const map& right , const Allocator& alloc)
		:_tree(right._tree , alloc)
	{   }

	map(map&& right)
		:_tree(_STD move(right._tree))
	{   }

	map(map&& right , const Allocator& alloc)
		:_tree(_STD move(right._tree) , alloc)
	{   }

	map(_STD initializer_list<value_type> il , const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		:map(il.begin() , il.end() , comp , alloc)
	{   }

	map(_STD initializer_list<value_type> il , const Allocator& alloc)
		:map(il.begin() , il.end() , alloc)
	{   }

	~map()
	{   }

	map& operator=(const map& right)
	{
		clear();
		_tree = right._tree;
		return *this;
	}

	map& operator=(map&& right)
		noexcept(_STD allocator_traits<Allocator>::is_always_equal::value&&_STD is_nothrow_move_assignable<Compare>::value)//since C++ 17
	{
		clear();
		_tree = _STD move(right._tree);
		return *this;
	}

	map& operator=(_STD initializer_list<value_type> il)
	{
		clear();
		_insert_range(il.begin() , il.end());
		return *this;
	}

	allocator_type& get_allocator() const
	{
		return _tree.get_allocator();
	}

	mapped_type& at(const key_type& key)
	{
		iterator it(find(key));
		if (it == end())
			throw _STD out_of_range { "map at() out_of_range" };

		return (*it).second;
	}

	const mapped_type& at(const key_type& key) const
	{
		const_iterator it(find(key));
		if (it == cend())
			throw _STD out_of_range { "map at() out_of_range" };

		return (*it).second;
	}

	mapped_type& operator[](const key_type& key)
	{
		return insert(_STD make_pair(key , T())).first->second;
	}

	mapped_type& operator[](key_type&& key)
	{
		return insert(_STD make_pair(_STD move(key) , T())).first->second;
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
		_tree.clear();
	}

	_STD pair<iterator , bool> insert(const value_type& val)
	{
		return _tree.map_insert_unique(val);
	}

	_STD pair<iterator , bool> insert(value_type&& val)
	{
		return _tree.map_insert_unique(_STD move(val));
	}

	iterator insert(const_iterator hint , const value_type& val)
	{
		if ((!_tree.key_comp()(*hint , val)) && (!_tree.key_comp()(val , *hint)))
			return find(val.first);
		else
			return insert(val);
	}

	iterator insert(const_iterator hint , value_type&& val)
	{
		if ((!_tree.key_comp()(*hint , val)) && (!_tree.key_comp()(val , *hint)))
			return find(val.first);
		else
			return insert(val);
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

	void swap(map& right)
		//noexcept(_STD allocator_traits<Allocator>::is_always_equal::value&&_STD is_nothrow_swappable<Compare>::value)//since C++17
	{
		_STD swap(_tree , right._tree);
	}

	size_type count(const key_type& key)
	{
		return (find(key) != end());
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
			_tree.map_insert_unique(*first);
	}

	treetype _tree;
};

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator==(const map<Key , T , Compare , Allocator>& left , const map<Key , T , Compare , Allocator>& right)
{
	if (left.size() != right.size())
		return false;

	auto lit = left.cbegin();
	auto rit = right.cbegin();
	for (; lit != left.cend();)
	{
		if (*lit == *rit)
		{
			++lit;
			++rit;
		}
		else
			return false;
	}
	return true;
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator!=(const map<Key , T , Compare , Allocator>& left , const map<Key , T , Compare , Allocator>& right)
{
	return !(left == right);
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator<(const map<Key , T , Compare , Allocator>& left , const map<Key , T , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(left.begin() , left.end() , right.begin() , right.end() , left.key_comp());
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator>(const map<Key , T , Compare , Allocator>& left , const map<Key , T , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(right.begin() , right.end() , left.begin() , left.end());
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator>=(const map<Key , T , Compare , Allocator>& left , const map<Key , T , Compare , Allocator>& right)
{
	return !(left < right);
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator<=(const map<Key , T , Compare , Allocator>& left , const map<Key , T , Compare , Allocator>& right)
{
	return !(left > right);
}

template<typename Key , typename T , typename Compare , typename Allocator>
void swap(map<Key , T , Compare , Allocator>& left ,
	map<Key , T , Compare , Allocator>& right) noexcept(noexcept(left.swap(right)))//since C++ 17
{
	left.swap(right);
}



template<typename Key ,
	typename T ,
	typename Compare = _STD less<Key> ,
	typename Allocator = _STD allocator<_STD pair<const Key , T> >
>class multimap
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const key_type , mapped_type>;
	using size_type = unsigned;
	using difference_type = int;
	using key_compare = Compare;
	using allocator_type = Allocator;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename _STD allocator_traits<Allocator>::pointer;
	using const_pointer = typename _STD allocator_traits<Allocator>::const_pointer;
	using iterator = map_iterator<key_type , mapped_type , key_compare , allocator_type>;
	using const_iterator = map_const_iterator<key_type , mapped_type , key_compare , allocator_type>;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
	using node_type = map_node<key_type , mapped_type>;//since C++ 17

private:
	using treetype = RB_Tree<map_node<key_type , mapped_type> , iterator , Compare , Allocator>;
	using nodeptr = node_type * ;

public:
	class value_compare
	{
		friend class multimap<key_type , mapped_type , key_compare , allocator_type>;
	public:
		bool operator()(const value_type& left , const value_type& right) const
		{
			return comp(left.first , right.first);
		}
	protected:
		value_compare(Compare c)
			:comp(c)
		{   }

		Compare comp;
	};

	multimap()
		:multimap(Compare())
	{   }

	explicit multimap(const Compare& comp , const Allocator& alloc = Allocator())
		:_tree(comp , alloc)
	{   }

	explicit multimap(const Allocator& alloc)
		:multimap(Compare() , alloc)
	{   }

	template<typename Iter>
	multimap(Iter first , Iter last , const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		: _tree(comp , alloc)
	{
		_insert_range(first , last);
	}

	template<typename Iter>
	multimap(Iter first , Iter last , const Allocator& alloc)
		: multimap(first , last , Compare() , alloc)
	{   }

	multimap(const multimap& right)
		:_tree(right._tree)
	{   }

	multimap(const multimap& right , const Allocator& alloc)
		:_tree(right._tree , alloc)
	{   }

	multimap(multimap&& right)
		:_tree(_STD move(right._tree))
	{   }

	multimap(multimap&& right , const Allocator& alloc)
		:_tree(_STD move(right._tree) , alloc)
	{   }

	multimap(_STD initializer_list<value_type> il , const Compare& comp = Compare() , const Allocator& alloc = Allocator())
		:multimap(il.begin() , il.end() , comp , alloc)
	{   }

	multimap(_STD initializer_list<value_type> il , const Allocator& alloc)
		:multimap(il.begin() , il.end() , alloc)
	{   }

	~multimap()
	{   }

	multimap& operator=(const multimap& right)
	{
		clear();
		_tree = right._tree;
		return *this;
	}

	multimap& operator=(multimap&& right)
		noexcept(_STD allocator_traits<Allocator>::is_always_equal::value&&_STD is_nothrow_move_assignable<Compare>::value)//since C++ 17
	{
		clear();
		_tree = _STD move(right._tree);
		return *this;
	}

	multimap& operator=(_STD initializer_list<value_type> il)
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
		_tree.clear();
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
	iterator emplace_hint(const_iterator , Args&&...args)
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

	void swap(multimap& right)
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

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator==(const multimap<Key , T , Compare , Allocator>& left , const multimap<Key , T , Compare , Allocator>& right)
{
	if (left.size() != right.size())
		return false;

	auto lit = left.cbegin();
	auto rit = right.cbegin();
	for (; lit != left.cend();)
	{
		if (*lit == *rit)
		{
			++lit;
			++rit;
		}
		else
			return false;
	}
	return true;
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator!=(const multimap<Key , T , Compare , Allocator>& left , const multimap<Key , T , Compare , Allocator>& right)
{
	return !(left == right);
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator<(const multimap<Key , T , Compare , Allocator>& left , const multimap<Key , T , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(left.begin() , left.end() , right.begin() , right.end() , left.key_comp());
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator>(const multimap<Key , T , Compare , Allocator>& left , const multimap<Key , T , Compare , Allocator>& right)
{
	return _STD lexicographical_compare(right.begin() , right.end() , left.begin() , left.end());
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator>=(const multimap<Key , T , Compare , Allocator>& left , const multimap<Key , T , Compare , Allocator>& right)
{
	return !(left < right);
}

template<typename Key , typename T , typename Compare , typename Allocator>
bool operator<=(const multimap<Key , T , Compare , Allocator>& left , const multimap<Key , T , Compare , Allocator>& right)
{
	return !(left > right);
}

template<typename Key , typename T , typename Compare , typename Allocator>
void swap(multimap<Key , T , Compare , Allocator>& left ,
	multimap<Key , T , Compare , Allocator>& right) noexcept(noexcept(left.swap(right)))//since C++ 17
{
	left.swap(right);
}

#endif // ! CONTAINERS_MAP_HPP
