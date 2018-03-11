#ifndef UNORDERED_MAP_HPP
#define UNORDERED_MAP_HPP
#include"unordered_base.hpp"
#include<xhash>
#include<utility>
#include<vector>
#include<memory>
#include<initializer_list>
#include<exception>

/*                                        map_const_iterator                          */
template<typename Key , typename T>
class map_const_iterator
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const Key , T>;
	using node_type = umap_node<Key , T>;
	using nodeptr = node_type * ;

	map_const_iterator () = default;

	map_const_iterator (nodeptr p) :np (p)
	{   }

	map_const_iterator (const map_const_iterator& Right) :np (Right.np)
	{   }

	map_const_iterator& operator=(const map_const_iterator& Right)
	{
		np = Right.np;
		return *this;
	}

	bool operator ==(const map_const_iterator& Right)
	{
		return (np == Right.np);
	}

	bool operator!=(const map_const_iterator& Right)
	{
		return (np != Right.np);
	}

	map_const_iterator& operator++()
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_const_iterator operator++() out_of_range" };
		np = np->next;
		return *this;
	}

	map_const_iterator operator++(int)
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_const_iterator operator++(int) out_of_range" };
		map_const_iterator tmp (*this);
		np = np->next;
		return tmp;
	}

	map_const_iterator& operator--()
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_const_iterator operator--() out_of_range" };
		np = np->prev;
		return *this;
	}

	map_const_iterator operator--(int)
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_const_iterator operator--(int) out_of_range" };
		map_const_iterator tmp (*this);
		np = np->prev;
		return tmp;
	}

	const value_type& operator*() const
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_const_iterator operator--(int) out_of_range" };
		return np->data;
	}

	const node_type* operator->() const
	{
		return np;
	}

private:
	nodeptr np = nullptr;
};

/*                            map_iterator                                      */

template<typename Key , typename T>
class map_iterator
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const Key , T>;
	using node_type = umap_node<Key , T>;
	using nodeptr = node_type * ;

	map_iterator () = default;

	map_iterator (nodeptr p) :np (p)
	{   }

	map_iterator (const map_iterator& Right) :np (Right.np)
	{   }

	map_iterator& operator=(const map_iterator& Right)
	{
		np = Right.np;
		return *this;
	}

	bool operator==(const map_iterator& Right) const
	{
		return (np == Right.np);
	}

	bool operator!=(const map_iterator& Right) const
	{
		return (np != Right.np);
	}

	map_iterator& operator++()
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_iterator operator++() out_of_range" };
		np = np->next;
		return *this;
	}

	map_iterator operator++(int)
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_iterator operator++(int) out_of_range" };
		map_iterator tmp (*this);
		np = np->next;
		return tmp;
	}

	map_iterator& operator--()
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_iterator operator--() out_of_range" };
		np = np->prev;
		return *this;
	}

	map_iterator operator--(int)
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_iterator operator--(int) out_of_range" };
		map_iterator tmp (*this);
		np = np->prev;
		return tmp;
	}

	value_type& operator*() const
	{
		if (np == nullptr)
			throw _STD out_of_range { "map_iterator operator*() out_of_range" };
		return np->data;
	}

	nodeptr operator->() const
	{
		return np;
	}

private:
	nodeptr np = nullptr;
};

/*                                      unordered_map_base                             */

template<typename Key ,
	typename T ,
	typename Hash ,
	typename Pred ,
	typename Alloc>
	class unordered_map_base :public unordered_base<umap_node<Key , T> , Hash , Pred , Alloc>
{
protected:
	using nodetype = umap_node<Key , T>;
	using nodeptr = nodetype * ;
	using value_type = typename nodetype::value_type;
	using mapped_type = typename nodetype::mapped_type;
	using key_type = typename nodetype::key_type;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using iterator = map_iterator<key_type , mapped_type>;
	using const_iterator = map_const_iterator<key_type , mapped_type>;

	using unordered_base::unordered_base;

	nodeptr _buynode (const key_type& key , const mapped_type& val , nodeptr n = nullptr , nodeptr p = nullptr)
	{
		nodeptr ret = _alloc.allocate (1);
		_alloc.construct (ret , key , val , n , p);
		return ret;
	}

	nodeptr _insert (const value_type& pv)
	{
		if (nodeptr np = _findkey (pv.first))
			return np;

		if (((_size + 1) / _buckets.size ()) >= _load_factor)
			_rehash (_size * 2);

		return _insert_withoutcheck (pv);
	}

	nodeptr _insert (const key_type& key)
	{
		mapped_type mp;
		_STD pair<key_type , mapped_type> (key , mp) tmp;
		return _insert (_STD move(tmp));
	}

	nodeptr _insert(value_type&& val)//withoutcheck
	{
		nodeptr np = _alloc.allocate(1) :
			_alloc.construct(np , _STD forward<value_type>(val));
		_insert_node(np);
		return np;
	}

	nodeptr _insert_withoutcheck (const value_type& pv)
	{
		size_type hv = _makehash (pv.first);
		++_size;
		if (_size == 0)
			_head = hv;

		if (_buckets [hv])
		{
			_buckets [hv] = _buynode (pv.first , pv.second , _buckets [hv] , _buckets [hv]->prev);
			_buckets [hv]->next->prev = _buckets [hv];
			_buckets [hv]->prev->next = _buckets [hv];
			return _buckets [hv];
		}
		else
		{
			_buckets [hv] = _buynode (pv.first , pv.second , nullptr , _last);

			if (_last)
				_last->next = _buckets [hv];
			else
				_head = hv;

			return _last = _buckets [hv];
		}
	}

	nodeptr _insert_multiable (const value_type& pv)
	{
		if (((_size + 1) / _buckets.size ()) >= _load_factor)
			_rehash (_size * 2);

		return _insert_withoutcheck(pv);
	}

	iterator _erase(const_iterator position)
	{
		iterator ret(position.np->next);
		_delnode(position.np);
		return ret;
	}

	iterator _erase(const_iterator first , const_iterator last)
	{
		nodeptr np = first.np;
		np->prev->next = last.np;
		for (; first != last; ++first)
			_freenode(first.np);
		return last.np;
	}
};


/*                                           unordered_map                                        */

template<typename Key ,
	typename T ,
	typename Hash = _STD hash<Key> ,
	typename Pred = _STD equal_to<Key> ,
	typename Alloc = _STD allocator<umap_node<Key , T>>>
	class unordered_map :public unordered_map_base<Key , T , Hash , Pred , Alloc>
{
public:

	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const Key , T>;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename _STD allocator_traits<Alloc>::pointer;				//just value_type*
	using const_pointer = typename _STD allocator_traits<Alloc>::const_pointer;	//just const value_type*
	using iterator = map_iterator<Key , T>;
	using const_iterator = map_const_iterator<Key , T>;
	using local_iterator = iterator;
	using const_local_iterator = const_iterator;
	using difference_type = ptrdiff_t;
	using size_type = unsigned;


private:
	using nodetype = umap_node<Key , T>;
	using nodeptr = nodetype * ;

public:

	unordered_map () :unordered_map (4)
	{   }

	explicit unordered_map (size_type n ,
		const hasher&hf = hasher () ,
		const key_equal& eql = key_equal() ,
		const allocator_type& alloc = allocator_type ())
		:unordered_map_base(n , hf , eql , alloc)
	{   }

	explicit unordered_map (const allocator_type& alloc)
		:unordered_map_base(4 , hasher() , key_equal() , alloc)
	{   }

	unordered_map (size_type n , const allocator_type& alloc)
		:unordered_map_base(n , hasher() , key_equal() , alloc)
	{   }

	unordered_map (size_type n , const hasher& hf , const allocator_type& alloc)
		:unordered_map_base(n , hf , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_map (Iter first , Iter last ,
		size_type n = 4 ,
		const hasher&hf = hasher () ,
		const key_equal& eql = key_equal () ,
		const allocator_type& alloc = allocator_type ())
		: unordered_map_base (n , hf , eql , alloc)
	{
		for (; first != last; ++first)
			_insert (*first);
	}

	template<typename Iter>
	unordered_map(Iter first , Iter last , size_type n , const allocator_type& alloc)
		:unordered_map(first , last , n , hasher() , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_map(Iter first , Iter last , size_type n , const hasher& hf , const allocator_type& alloc)
		: unordered_map(first , last , n , hf , key_equal() , alloc)
	{   }

	unordered_map (const unordered_map& Right)
		:unordered_map (Right , Right.get_allocator ())
	{   }

	unordered_map (const unordered_map&Right , const allocator_type& alloc)
		: unordered_map_base (Right.bucket_count() , Right.hash_fuction() , Right.key_eql() , alloc)
	{
		for (const_iterator it = Right.begin (); it != Right.end (); ++it)
			_insert (*it);
	}

	unordered_map (unordered_map&& Right)
		:unordered_map_base(_STD move (Right._buckets) , _STD move (Right._hash) , _STD move (Right._eql) ,
			_STD move (Right._alloc))
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_map (unordered_map&& Right , const allocator_type& alloc)
		:unordered_map_base (_STD move (Right._buckets) , _STD move (Right._hash) , _STD move (Right._eql) ,
			alloc)
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_map (_STD initializer_list<value_type> il ,
		size_type n = 4 ,
		const hasher&hf = hasher () ,
		const key_equal& eql = key_equal () ,
		const allocator_type& alloc = allocator_type ())
		: unordered_map_base (n , hf , eql , alloc)
	{
		for (_STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert (*it);
	}

	unordered_map (_STD initializer_list<value_type> il ,
		size_type n ,
		const allocator_type& alloc)
		: unordered_map_base (n , hasher() , key_equal() , alloc)
	{
		for (_STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert (*it);
	}

	unordered_map (_STD initializer_list<value_type> il ,
		size_type n ,
		const hasher&hf ,
		const allocator_type& alloc)
		: unordered_map_base (n , hf , key_equal() , alloc)
	{
		for (_STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert (*it);
	}

	unordered_map& operator=(const unordered_map& Right)
	{
		clear ();
		_buckets.resize (_Right._buckets.size ());
		_hash = _Right._hash;
		_eql = _Right._eql;
		_alloc = _Right._alloc;
		for (iterator it = _Right.begin (); it != _Right.end (); ++it)
			_insert (*it);

		return *this;
	}

	unordered_map& operator=(unordered_map&& Right)
	{
		clear ();
		_buckets = _STD move (_Right._buckets);
		_hash = _STD move (_Right._hash);
		_eql = _STD move (_Right._eql);
		_alloc = _STD move (_Right._alloc);
		_size = _Right._size;
		_head = _Right._head;
		_last = _Right._last;

		return *this;
	}

	unordered_map& operator=(_STD initializer_list<value_type> il)
	{
		clear ();
		for (_STD initializer_list<key_type>::iterator it = il.begin (); il != il.end (); ++it)
			_insert (*it);
		return *this;
	}

	~unordered_map ()
	{
		clear ();
	}

	bool empty () const noexcept
	{
		return (_size == 0);
	}

	size_type size () const noexcept
	{
		return _size;
	}

	size_type maxsize () const noexcept
	{
		return 357913941;
	}

	iterator begin () noexcept
	{
		return _buckets [_head];
	}

	const_iterator begin () const noexcept
	{
		return _buckets [_head];
	}

	const_iterator cbegin () const noexcept
	{
		return _buckets [_head];
	}

	local_iterator begin (size_type n)
	{
		if (n >= _buckets.size ())
			return nullptr
		else
			return _buckets [n];
	}

	const_local_iterator begin (size_type n) const
	{
		if (n >= _buckets.size ())
			return nullptr
		else
			return _buckets [n];
	}

	const_local_iterator cbegin (size_type n) const
	{
		if (n >= _buckets.size ())
			return nullptr
		else
			return _buckets [n];
	}

	iterator end () noexcept
	{
		return nullptr;
	}

	const_iterator end () const noexcept
	{
		return nullptr;
	}

	const_iterator cend () const noexcept
	{
		return nullptr;
	}

	local_iterator end (size_type n)
	{
		if (n >= _buckets.size ())
			return nullptr;

		if (!_buckets [n])
			return nullptr;

		nodeptr np = _buckets [n];
		for (; np->next && (n == _makehash (np->next->getkey())); np = np->next);
		return np->next;
	}

	const_local_iterator end (size_type n) const
	{
		return static_cast<const_local_iterator>(end (n));
	}

	const_local_iterator cend (size_type n) const
	{
		return static_cast<const_local_iterator>(end (n));
	}

	mapped_type& operator[](const key_type& k)
	{
		nodeptr np = _findkey (k);
		if (np == nullptr)
		{
			_insert (k);
			np = _findkey (k);
		}
		return np->getmapped ();
	}

	mapped_type& operator[](key_type&& k)
	{
		return *this [k];
	}

	mapped_type& at (const key_type& k)
	{
		nodeptr np = _findkey (k);
		if (np != nullptr)
			return np->getmapped ();

		throw _STD out_of_range { "unordered_map at() out_of_range" }
	}

	const mapped_type& at (const key_type& k) const
	{
		nodeptr np = _findkey (k);
		if (np != nullptr)
			return np->getmapped ();

		throw _STD out_of_range { "unordered_map at() out_of_range" }
	}

	iterator find (const key_type& key)
	{
		return _findkey (key);
	}

	const_iterator find (const key_type& key) const
	{
		return _findkey (key);
	}

	size_type count (const key_type& key) const
	{
		return _findkey (key) != nullptr;
	}

	_STD pair<iterator , iterator> equal_range (const key_type& key)
	{
		iterator ret (_findkey (key));
		iterator ret2(ret);
		++ret2;
		return _STD make_pair (static_cast<iterator>(ret) , static_cast<iterator>(ret2));
	}

	_STD pair<const_iterator , const_iterator> equal_range (const key_type& key) const
	{
		const_iterator ret (_findkey (key));
		const_iterator ret2(ret);
		++ret2;
		return _STD make_pair (static_cast<const_iterator>(ret) , static_cast<const_iterator>(ret2));
	}

	template<typename ...Args>
	_STD pair<iterator , bool> emplace (Args&&...args)
	{
		umap_node<key_type , mapped_type> tnode (_STD forward<Args>(args)...);
		nodeptr np = _findkey (tnode.getkey ());
		if (np != nullptr)
		{
			np->getmapped () = tnode.getmapped ();
			return _STD make_pair (static_cast<iterator>(np) , false);
		}
		else
		{
			return _STD make_pair (static_cast<iterator>(_insert (tnode)) , true);
		}
	}

	template<typename ...Args>
	iterator emplace_hint (const_iterator position , Args&&...args)
	{
		umap_node<key_type , mapped_type> tnode (_STD forward<Args> (args)...);
		if ((*position).first == tnode.getkey ())
		{
			(*position).second = tnode.getmapped ();
			return iterator (positon.np);
		}

		nodeptr np = _findkey (tnode.getkey ());
		if (np == nullptr)
			np = _insert (tnode);
		else
			np->getmapped () = tnode.getmapped ();

		return np;
	}

	_STD pair<iterator , bool> insert (const value_type& val)
	{
		nodeptr np = _findkey (val.first);
		if (np != nullptr)
			return _STD make_pair (static_cast<iterator>(np) , false);
		else
			return _STD make_pair (static_cast<iterator>(_insert(val) , false));
	}

	_STD pair<iterator , bool> insert(value_type&& val)
	{
		nodeptr np = _findkey(val.first);
		if (np != nullptr)
			return _STD make_pair (static_cast<iterator>(np) , false);
		else
			return _STD make_pair (static_cast<iterator>(_insert(val) , false));
	}

	iterator insert(const_iterator hint , const value_type& val)
	{
		if (_eql((*hint).first , val.first))
		{
			iterator ret(hint.np);
			return ret;
		}
		else
			return (insert(val).first);
	}

	iterator insert(const_iterator hint , value_type&& val)
	{
		if (_eql((*hint).first , val.first))
		{
			iterator ret(hint.np);
			return ret;
		}
		else
			return (insert(val).first);
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

	iterator erase(const_iterator position)
	{
		return _erase(position);
	}

	size_type erase(const key_type& key)
	{
		nodeptr np = _findkey(key);
		if (np == nullptr)
			return 0;
		else
		{
			_delnode(np);
			return 1;
		}
	}

	iterator erase(const_iterator first , const_iterator last)
	{
		return _erase(first , last);
	}

	void clear () noexcept
	{
		_clear ();
	}

	void swap(unordered_map& Right)
	{
		_STD swap(_buckets , Right._buckets);
		_STD swap(_size , Right._size);
		_STD swap(_hash , Right._hash);
		_STD swap(_eql , Right._eql);
		_STD swap(_alloc , Right._alloc);
		_STD swap(_head , Right._head);
		_STD swap(_last , Right._last);
		_STD swap(_load_factor , Right._load_factor);
	}

	size_type bucket_count() const noexcept
	{
		return _bucket_count();
	}

	size_type max_bucket_count() const noexcept
	{
		return _max_bucket_count();
	}

	size_type bucket_size(size_type n) const
	{
		return _bucket_size(n);
	}

	size_type bucket(const key_type& key) const
	{
		return _makehash(key);
	}

	float load_factor() const noexcept
	{
		return _get_load_factor();
	}

	float max_load_factor() const noexcept
	{
		return _load_factor;
	}

	void max_load_factor(float newlf)
	{
		_max_load_factor(newlf);
	}

	void rehash(size_type n)
	{
		_rehash(n);
	}

	void reserve(size_type n)
	{
		_reserve(n);
	}

	hasher hash_function() const
	{
		return _hash_function();
	}

	key_equal key_eq() const
	{
		return _key_eq();
	}

	allocator_type get_allocator() const
	{
		return _get_allocator();
	}

};

/*                                            unordered_multimap                   */

template<typename Key ,
	typename T ,
	typename Hash = _STD hash<Key> ,
	typename Pred = _STD equal_to<Key> ,
	typename Alloc = _STD allocator<umap_node<Key , T>>>
	class unordered_multimap :public unordered_map_base<Key , T , Hash , Pred , Alloc>
{
public:

	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const Key , T>;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename _STD allocator_traits<Alloc>::pointer;				//just value_type*
	using const_pointer = typename _STD allocator_traits<Alloc>::const_pointer;	//just const value_type*
	using iterator = map_iterator<Key , T>;
	using const_iterator = map_const_iterator<Key , T>;
	using local_iterator = iterator;
	using const_local_iterator = const_iterator;
	using difference_type = ptrdiff_t;
	using size_type = unsigned;


private:
	using nodetype = umap_node<Key , T>;
	using nodeptr = nodetype * ;


	unordered_multimap()
		:unordered_multimap(4)
	{   }

	explicit unordered_multimap(size_type n ,
		const hasher& hf = hasher() ,
		const key_equal& eql = key_equal() ,
		const allocator_type& alloc = allocator_type())
		:unordered_map_base(n , hf , eql , alloc)
	{   }

	explicit unordered_multimap(const allocator_type& alloc)
		:unordered_map_base(4 , hasher() , key_equal() , alloc)
	{   }

	unordered_multimap(size_type n , const allocator_type& alloc)
		:unordered_map_base(n , hasher() , key_equal() , alloc)
	{   }

	unordered_multimap(size_type n , const hasher& hf , const allocator_type& alloc)
		:unordered_map_base(n , hf , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_multimap(Iter first , Iter last ,
		size_type n = 4 ,
		const hasher& hf = hasher() ,
		const key_equal& eql = key_equal() ,
		const allocator_type& alloc = allocator_type())
		: unordered_map_base(n , hf , eql , alloc)
	{
		for (; first != last; ++first)
			_insert_multiable(*first);
	}

	template<typename Iter>
	unordered_multimap(Iter first , Iter last , size_type n , const allocator_type& alloc)
		:unordered_multimap(first , last , n , hasher() , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_multimap(Iter first , Iter last , size_type n , const hasher& hf , const allocator_type& alloc)
		: unordered_multimap(first , last , n , hf , key_equal() , alloc)
	{
		for (; first != last; ++first)
			_insert_multiable(*first);
	}

	unordered_multimap(const unordered_multimap& Right)
		:unordered_multimap(Right , Right.get_allocator())
	{   }

	unordered_multimap(const unordered_multimap& Right , const allocator_type& alloc)
		: unordered_map_base (Right.bucket_count() , Right.hash_fuction() , Right.key_eql() , alloc)
	{
		for (const_iterator it = Right.begin(); it != Right.end(); ++it)
			_insert_multiable(*it);
	}

	unordered_multimap(unordered_multimap&& Right)
		:unordered_map_base(_STD move (Right._buckets) , _STD move (Right._hash) , _STD move (Right._eql) ,
			_STD move (Right._alloc))
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_multimap (unordered_multimap&& Right , const allocator_type& alloc)
		:unordered_map_base (_STD move (Right._buckets) , _STD move (Right._hash) , _STD move (Right._eql) ,
			alloc)
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_multimap(_STD initializer_list<value_type> il , size_type n , const hasher& hf = hasher() ,
		const key_equal& eql = key_equal() , const allocator_type& alloc = allocator_type())
		:unordered_map_base(n , hf , eql , alloc)
	{
		for (_STD initializer_list<value_type>::const_iterator it = il.begin(); it != il.end(); ++it)
			_insert_multiable(*it);
	}

	unordered_multimap(_STD initializer_list<value_type> il ,
		size_type n ,
		const allocator_type& alloc)
		: unordered_map_base (n , hasher() , key_equal() , alloc)
	{
		for (_STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert_multiable (*it);
	}

	unordered_multimap (_STD initializer_list<value_type> il ,
		size_type n ,
		const hasher&hf,
		const allocator_type& alloc)
		: unordered_map_base (n , hf , key_equal() , alloc)
	{
		for (_STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert_multiable (*it);
	}

	unordered_multimap& operator=(const unordered_multimap& Right)
	{
		clear ();
		_buckets.resize (_Right._buckets.size ());
		_hash = _Right._hash;
		_eql = _Right._eql;
		_alloc = _Right._alloc;
		for (iterator it = _Right.begin (); it != _Right.end (); ++it)
			_insert_multiable (*it);

		return *this;
	}

	unordered_multimap& operator=(unordered_multimap&& Right)
	{
		clear ();
		_buckets = _STD move (_Right._buckets);
		_hash = _STD move (_Right._hash);
		_eql = _STD move (_Right._eql);
		_alloc = _STD move (_Right._alloc);
		_size = _Right._size;
		_head = _Right._head;
		_last = _Right._last;

		return *this;
	}

	unordered_multimap& operator=(_STD initializer_list<value_type> il)
	{
		clear ();
		for (_STD initializer_list<key_type>::iterator it = il.begin (); il != il.end (); ++it)
			_insert_multiable (*it);
		return *this;
	}

	~unordered_multimap()
	{
		clear();
	}

	bool empty() const noexcept
	{
		return (_size == 0);
	}

	size_type size () const noexcept
	{
		return _size;
	}

	size_type maxsize () const noexcept
	{
		return 357913941;
	}

	iterator begin () noexcept
	{
		return _buckets [_head];
	}

	const_iterator begin () const noexcept
	{
		return _buckets [_head];
	}

	const_iterator cbegin () const noexcept
	{
		return _buckets [_head];
	}

	local_iterator begin (size_type n)
	{
		if (n >= _buckets.size ())
			return nullptr
		else
			return _buckets [n];
	}

	const_local_iterator begin (size_type n) const
	{
		if (n >= _buckets.size ())
			return nullptr
		else
			return _buckets [n];
	}

	const_local_iterator cbegin (size_type n) const
	{
		if (n >= _buckets.size ())
			return nullptr
		else
			return _buckets [n];
	}

	iterator end () noexcept
	{
		return nullptr;
	}

	const_iterator end () const noexcept
	{
		return nullptr;
	}

	const_iterator cend () const noexcept
	{
		return nullptr;
	}

	local_iterator end (size_type n)
	{
		if (n >= _buckets.size ())
			return nullptr;

		if (!_buckets [n])
			return nullptr;

		nodeptr np = _buckets [n];
		for (; np->next && (n == _makehash (np->next->getkey())); np = np->next);
		return np->next;
	}

	const_local_iterator end (size_type n) const
	{
		return static_cast<const_local_iterator>(end (n));
	}

	const_local_iterator cend (size_type n) const
	{
		return static_cast<const_local_iterator>(end (n));
	}

	iterator find (const key_type& key)
	{
		return _findkey (key);
	}

	const_iterator find (const key_type& key) const
	{
		return _findkey (key);
	}

	size_type count(const key_type& key) const
	{
		nodeptr np = _findkey(key);
		if (np == nullptr)
			return 0;
		size_type ret = 1;
		for (np = np->next; np; np = np->next)
			if (_eql(key , np->getkey()))
				++ret;
			else
				break;

		return ret;
	}

	_STD pair<iterator , iterator> equal_range(const key_type& key)
	{
		iterator ret1 = find(key);
		iterator ret2(ret1);
		size_type num = count(key);
		for (; num; --num)
			++ret2;
		return _STD make_pair(ret1 , ret2);
	}

	_STD pair<const_iterator , const_iterator> equal_range(const key_type& key) const
	{
		const_iterator ret1 = find(key);
		const_iterator ret2(ret1);
		size_type num = count(key);
		for (; num; --num)
			++ret2;
		return _STD make_pair(ret1 , ret2);
	}

	template<typename ...Args>
	iterator emplace(Args&&...args)
	{
		umap_node<key_type , mapped_type> tnode(_STD forward<Args>(args)...);
		nodeptr newnode = &tnode;
		nodeptr np = _findkey(tnode.getkey());
		if (np == nullptr)
		{
			size_type hv = _makehash(tnode.getkey());
			if (_buckets [hv])
				_insert_after(_buckets [hv] , newnode);
			else
			{
				_insert_after(_last , newnode);
				_last = newnode;
				_buckets [hv] = newnode;
			}
		}
		else
			_insert_after(np , newnode);

		return newnode;
	}

	template<typename ...Args>
	iterator emplace_hint(const_iterator position , Args&&...args)
	{
		umap_node<key_type , mapped_type> tnode(_STD forward<Args>(args)...);
		nodeptr newnode = &tnode;
		if (_eql((*position).first , tnode.getkey()))
			_insert_after(position.np , newnode);
		else
		{
			nodeptr np = _findkey(tnode.getkey());
			if (np == nullptr)
			{
				size_type hv = _makehash(tnode.getkey());
				if (_buckets [hv])
					_insert_after(_buckets [hv] , newnode);
				else
				{
					_insert_after(_last , newnode);
					_last = newnode;
					_buckets [hv] = newnode;
				}
			}
			else
				_insert_after(np , newnode);
		}
		return newnode;
	}

	iterator insert(const value_type& val)
	{
		return _insert_multiable(val);
	}

	_STD iterator insert(value_type&& val)
	{
		return _insert(val);
	}

	iterator insert(const_iterator hint , const value_type& val)
	{
		if (_eql((*hint).first , val.first))
		{
			umap_node<key_type , mapped_type> tnode(val);
			nodeptr newnode = &tnode;
			_insert_after(hint.np , newnode);
			return newnode;
		}
		else
			return insert(val);
	}

	iterator insert(const_iterator hint , value_type&& val)
	{
		return insert(val);
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

	iterator erase(const_iterator position)
	{
		return _erase(position);
	}

	iterator erase(const_iterator first , const_iterator last)
	{
		return _erase(first , last);
	}

	size_type erase(const key_type& key)
	{
		size_type ret = count(key);
		if (ret != 0)
		{
			_STD pair<iterator , iterator> itp = equal_range(key);
			erase(itp.first , itp.second);
		}
		return ret;
	}

	void clear () noexcept
	{
		_clear ();
	}

	void swap(unordered_multimap& Right)
	{
		_STD swap(_buckets , Right._buckets);
		_STD swap(_size , Right._size);
		_STD swap(_hash , Right._hash);
		_STD swap(_eql , Right._eql);
		_STD swap(_alloc , Right._alloc);
		_STD swap(_head , Right._head);
		_STD swap(_last , Right._last);
		_STD swap(_load_factor , Right._load_factor);
	}

	size_type bucket_count() const noexcept
	{
		return _bucket_count();
	}

	size_type max_bucket_count() const noexcept
	{
		return _max_bucket_count();
	}

	size_type bucket_size(size_type n) const
	{
		return _bucket_size(n);
	}

	size_type bucket(const key_type& key) const
	{
		return _makehash(key);
	}

	float load_factor() const noexcept
	{
		return _get_load_factor();
	}

	float max_load_factor() const noexcept
	{
		return _load_factor;
	}

	void max_load_factor(float newlf)
	{
		_max_load_factor(newlf);
	}

	void rehash(size_type n)
	{
		_rehash(n);
	}

	void reserve(size_type n)
	{
		_reserve(n);
	}

	hasher hash_function() const
	{
		return _hash_function();
	}

	key_equal key_eq() const
	{
		return _key_eq();
	}

	allocator_type get_allocator() const
	{
		return _get_allocator();
	}
};

#endif //UNORDERED_MAP_HPP