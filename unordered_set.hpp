#ifndef CONTAINERS_UNORDERED_SET_HPP
#define	CONTAINERS_UNORDERED_SET_HPP
#include"unordered_base.hpp"
#include<xhash>
#include<vector>
#include<initializer_list>
#include<stdexcept>
#include<memory>
#include<utility>

/*                                      set_const_iterator                                  */
template<typename Key>
class set_const_iterator
{
public:
	using key_type = Key;
	using value_type = key_type;
	using node_type = uset_node<key_type>;
	using nodeptr = node_type * ;

	set_const_iterator() = default;

	set_const_iterator(nodeptr p) :np(p)
	{   }

	set_const_iterator(const set_const_iterator& Right) :np(Right.np)
	{   }

	set_const_iterator& operator=(const set_const_iterator& Right)
	{
		np = Right.np;
		return *this;
	}

	bool operator==(const set_const_iterator& Right) const
	{
		return (np == Right.np);
	}

	bool operator!=(const set_const_iterator& Right) const
	{
		return (np != Right.np);
	}

	set_const_iterator& operator++()
	{
		if (np == nullptr)
			throw STD out_of_range { "set_const_iterator operator++() out_of_range" };
		np = np->next;
		return *this;
	}

	set_const_iterator operator++(int)
	{
		if (np == nullptr)
			throw STD out_of_range { "set_const_iterator operator++(int) out_of_range" };
		set_const_iterator tmp (*this);
		np = np->next;
		return tmp;
	}

	set_const_iterator& operator--()
	{
		if (np == nullptr)
			throw STD out_of_range { "set_const_iterator operator--() out_of_range" };
		np = np->prev;
		return *this;
	}

	set_const_iterator operator--(int)
	{
		if (np == nullptr)
			throw STD out_of_range { "set_const_iterator operator--(int) out_of_range" };
		set_const_iterator tmp (*this);
		np = np->prev;
		return tmp;
	}

	const value_type& operator*() const
	{
		if (np == nullptr)
			throw STD out_of_range { "set_const_iterator operator--(int) out_of_range" };
		return np->getkey();
	}

	const node_type* operator->() const
	{
		return np;
	}

private:
	nodeptr np;
};

/*                                    set_iterator                                    */

template<typename Key>
class set_iterator
{
public:
	using key_type = Key;
	using value_type = key_type;
	using node_type = uset_node<key_type>;
	using nodeptr = node_type * ;

	set_iterator() = default;

	set_iterator(nodeptr p) :np(p)
	{   }

	set_iterator(const set_iterator& Right) :np(Right.np)
	{   }

	set_iterator& operator=(const set_iterator& Right)
	{
		np = Right.np;
		return *this;
	}

	bool operator==(const set_iterator& Right) const
	{
		return (np == Right.np);
	}

	bool operator!=(const set_iterator& Right) const
	{
		return (np != Right.np);
	}

	set_iterator& operator++()
	{
		if (np == nullptr)
			throw STD out_of_range { "set_iterator operator++() out_of_range" };
		np = np->next;
		return *this;
	}

	set_iterator operator++(int)
	{
		if (np == nullptr)
			throw STD out_of_range { "set_iterator operator++(int) out_of_range" };
		set_iterator tmp (*this);
		np = np->next;
		return tmp;
	}

	set_iterator& operator--()
	{
		if (np == nullptr)
			throw STD out_of_range { "set_iterator operator--() out_of_range" };
		np = np->prev;
		return *this;
	}

	set_iterator operator--(int)
	{
		if (np == nullptr)
			throw STD out_of_range { "set_iterator operator--(int) out_of_range" };
		set_iterator tmp (*this);
		np = np->prev;
		return tmp;
	}

	value_type& operator*() const
	{
		if (np == nullptr)
			throw STD out_of_range { "set_iterator operator*() out_of_range" };
		return np->getkey();
	}

	nodeptr operator->() const
	{
		return np;
	}

private:
	nodeptr np = nullptr;
};

/*                                  unordered_set_base                                */

template<typename Key ,
	typename Hash,
	typename Pred ,
	typename Alloc>
	class unordered_set_base :public unordered_base<uset_node<Key> , Hash , Pred , Alloc>
{
protected:
	using nodetype = uset_node<Key>;
	using nodeptr = nodetype * ;
	using key_type = typename nodetype::key_type;
	using value_type = typename nodetype::value_type;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using iterator = set_iterator<key_type>;
	using const_iterator = set_const_iterator<key_type>;

	using unordered_base::unordered_base;

	nodeptr _buynode(const key_type& key , nodeptr n , nodeptr p)
	{
		nodeptr ret = _alloc.allocate(1);
		_alloc.construct(ret , key , n , p);
		return ret;
	}

	nodeptr _insert(const value_type& key)
	{
		if ((nodeptr np = _findkey(key)) != nullptr)
			return np;

		if ((_size + 1) / _buckets.size() >= _load_factor)
			_rehash(_size * 2);

		return _insert_withoutcheck(key);
	}

	nodeptr _insert_withoutcheck(const value_type& key)
	{
		size_type hv = _makehash(key);
		++_size;

		if (_size == 0)
			_head = hv;

		if (_buckets [hv])
		{
			_buckets [hv] = _buynode(key , _buckets [hv] , _buckets [hv]->prev);
			_buckets [hv]->next->prev = _buckets [hv];
			_buckets [hv]->prev->next = _buckets [hv];
			return _buckets [hv];
		}
		else
		{
			_buckets [hv] = _buynode(key , nullptr , _last);

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


/*                                       unordered_set                        */

template<typename Key ,
	typename Hash=std::hash<Key>,
	typename Pred = STD equal_to<Key> ,
	typename Alloc = STD allocator<uset_node<Key>>>
	class unordered_set :public unordered_set_base<Key , Hash , Pred , Alloc>
{
public:
	using key_type = Key;
	using value_type = key_type;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using pointer = typename STD allocator_traits<Alloc>::pointer;				//just value_type*
	using const_pointer = typename STD allocator_traits<Alloc>::const_pointer;	//just const value_type*
	using reference = value_type & ;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator = set_iterator<key_type>;
	using const_iterator = set_const_iterator<key_type>;
	using local_iterator = iterator;
	using const_local_iterator = const_iterator;

private:
	using nodetype = uset_node<key_type>;
	using nodeptr = nodetype * ;

public:
	unordered_set () :unordered_set (4)
	{   }

	explicit unordered_set (size_type n ,
		const hasher&hf = hasher () ,
		const key_equal& eql = key_equal() ,
		const allocator_type& alloc = allocator_type ())
		:unordered_set_base(n , hf , eql , alloc)
	{   }

	explicit unordered_set (const allocator_type& alloc)
		:unordered_set_base(4 , hasher() , key_equal() , alloc)
	{   }

	unordered_set (size_type n , const allocator_type& alloc)
		:unordered_set_base(n , hasher() , key_equal() , alloc)
	{   }

	unordered_set (size_type n , const hasher& hf , const allocator_type& alloc)
		:unordered_set_base(n , hf , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_set (Iter first , Iter last ,
		size_type n = 4 ,
		const hasher&hf = hasher () ,
		const key_equal& eql = key_equal () ,
		const allocator_type& alloc = allocator_type ())
		: unordered_set_base (n , hf , eql , alloc)
	{
		for (; first != last; ++first)
			_insert (*first);
	}

	template<typename Iter>
	unordered_set(Iter first , Iter last , size_type n , const allocator_type& alloc)
		:unordered_set(first , last , n , hasher() , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_set(Iter first , Iter last , size_type n , const hasher& hf , const allocator_type& alloc)
		: unordered_set(first , last , n , hf , key_equal() , alloc)
	{   }

	unordered_set (const unordered_set& Right)
		:unordered_set (Right , Right.get_allocator ())
	{   }

	unordered_set (const unordered_set&Right , const allocator_type& alloc)
		: unordered_set_base (Right.bucket_count() , Right.hash_fuction() , Right.key_eql() , alloc)
	{
		for (const_iterator it = Right.begin (); it != Right.end (); ++it)
			_insert (*it);
	}

	unordered_set (unordered_set&& Right)
		:unordered_set_base(STD move (Right._buckets) , STD move (Right._hash) , STD move (Right._eql) ,
			STD move (Right._alloc))
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_set (unordered_set&& Right , const allocator_type& alloc)
		:unordered_set_base (STD move (Right._buckets) , STD move (Right._hash) , STD move (Right._eql) ,
			alloc)
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_set (STD initializer_list<value_type> il ,
		size_type n = 4 ,
		const hasher&hf = hasher () ,
		const key_equal& eql = key_equal () ,
		const allocator_type& alloc = allocator_type ())
		: unordered_set_base (n , hf , eql , alloc)
	{
		for (STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert (*it);
	}

	unordered_set (STD initializer_list<value_type> il ,
		size_type n ,
		const allocator_type& alloc)
		: unordered_set_base (n , hasher() , key_equal() , alloc)
	{
		for (STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert (*it);
	}

	unordered_set (STD initializer_list<value_type> il ,
		size_type n ,
		const hasher&hf ,
		const allocator_type& alloc)
		: unordered_set_base (n , hf , key_equal() , alloc)
	{
		for (STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert (*it);
	}

	unordered_set& operator=(const unordered_set& Right)
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

	unordered_set& operator=(unordered_set&& Right)
	{
		clear ();
		_buckets = STD move (_Right._buckets);
		_hash = STD move (_Right._hash);
		_eql = STD move (_Right._eql);
		_alloc = STD move (_Right._alloc);
		_size = _Right._size;
		_head = _Right._head;
		_last = _Right._last;

		return *this;
	}

	unordered_set& operator=(STD initializer_list<value_type> il)
	{
		clear ();
		for (STD initializer_list<key_type>::iterator it = il.begin (); il != il.end (); ++it)
			_insert (*it);
		return *this;
	}

	~unordered_set()
	{
		clear();
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

	STD pair<iterator , iterator> equal_range(const key_type& key)
	{
		iterator ret (_findkey (key));
		iterator ret2(ret);
		++ret2;
		return STD make_pair(static_cast<iterator>(ret) , static_cast<iterator>(ret2));
	}

	STD pair<const_iterator , const_iterator> equal_range(const key_type& key) const
	{
		const_iterator ret (_findkey (key));
		const_iterator ret2(ret);
		++ret2;
		return STD make_pair(static_cast<const_iterator>(ret) , static_cast<const_iterator>(ret));
	}

	template<typename ...Args>
	STD pair<iterator , bool> emplace(Args&&...args)
	{
		key_type k(STD forward<Args>(args)...);
		if ((nodeptr np = _findkey(k)) != nullptr)
			return STD make_pair(static_cast<iterator>(np) , false);
		else
			return STD make_pair(static_cast<iterator>(_insert(k)) , true);
	}

	template <class... Args>
	iterator emplace_hint(const_iterator hint , Args&&... args)
	{
		Key k(Args..args);
		if (_eql(*hint , k))
			return hint;

		return _insert(k);
	}

	STD pair<iterator , bool> insert(const key_type& key)
	{
		if (iterator it(_findkey(key)))
			return STD make_pair(it , false);

		return STD make_pair(_insert(key) , true);
	}

	STD pair<iterator , bool> insert(key_type&& key)
	{
		if (iterator it(_findkey(key)))
			return STD make_pair(it , false);

		return STD make_pair(_insert(STD forward<key_type>(key)) , false);
	}

	iterator insert(const_iterator hint , const key_type& key)
	{
		if (_eql(*hint , key))
			return hint;

		return _insert(key);
	}

	iterator insert(const_iterator hint , key_type&& key)
	{
		if (_eql(*hint , key))
			return hint;

		return _insert(STD forward<key_type>(key));
	}

	template<typename Iter>
	void insert(Iter first , Iter last)
	{
		for (; first != last; ++first)
			_insert(*first);
	}

	void insert(STD initializer_list<value_type> il)
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

	void swap(unordered_set& Right)
	{
		STD swap(_buckets , Right._buckets);
		STD swap(_size , Right._size);
		STD swap(_hash , Right._hash);
		STD swap(_eql , Right._eql);
		STD swap(_alloc , Right._alloc);
		STD swap(_head , Right._head);
		STD swap(_last , Right._last);
		STD swap(_load_factor , Right._load_factor);
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

/*                                                  unordered_multiset                      */

template<typename Key ,
	typename Hash=std::hash<Key>,
	typename Pred = STD equal_to<Key> ,
	typename Alloc = STD allocator<uset_node<Key , T>>>
	class unordered_multiset :public unordered_set_base<Key , Hash , Pred , Alloc>
{
public:
	using key_type = Key;
	using value_type = key_type;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using pointer = typename STD allocator_traits<Alloc>::pointer;				//just value_type*
	using const_pointer = typename STD allocator_traits<Alloc>::const_pointer;	//just const value_type*
	using reference = value_type & ;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator = set_iterator<key_type>;
	using const_iterator = set_const_iterator<key_type>;
	using local_iterator = iterator;
	using const_local_iterator = const_iterator;

private:
	using nodetype = uset_node<key_type>;
	using nodeptr = nodetype * ;


public:
	unordered_multiset()
		:unordered_multiset(4)
	{   }

	explicit unordered_multiset(size_type n ,
		const hasher& hf = hasher() ,
		const key_equal& eql = key_equal() ,
		const allocator_type& alloc = allocator_type())
		:unordered_set_base(n , hf , eql , alloc)
	{   }

	explicit unordered_multiset(const allocator_type& alloc)
		:unordered_set_base(4 , hasher() , key_equal() , alloc)
	{   }

	unordered_multiset(size_type n , const allocator_type& alloc)
		:unordered_set_base(n , hasher() , key_equal() , alloc)
	{   }

	unordered_multiset(size_type n , const hasher& hf , const allocator_type& alloc)
		:unordered_set_base(n , hf , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_multiset(Iter first , Iter last ,
		size_type n = 4 ,
		const hasher& hf = hasher() ,
		const key_equal& eql = key_equal() ,
		const allocator_type& alloc = allocator_type())
		: unordered_set_base(n , hf , eql , alloc)
	{
		for (; first != last; ++first)
			_insert_multiable(*first);
	}

	template<typename Iter>
	unordered_multiset(Iter first , Iter last , size_type n , const allocator_type& alloc)
		:unordered_multiset(first , last , n , hasher() , key_equal() , alloc)
	{   }

	template<typename Iter>
	unordered_multiset(Iter first , Iter last , size_type n , const hasher& hf , const allocator_type& alloc)
		: unordered_multiset(first , last , n , hf , key_equal() , alloc)
	{
		for (; first != last; ++first)
			_insert_multiable(*first);
	}

	unordered_multiset(const unordered_multiset& Right)
		:unordered_multiset(Right , Right.get_allocator())
	{   }

	unordered_multiset(const unordered_multiset& Right , const allocator_type& alloc)
		: unordered_set_base (Right.bucket_count() , Right.hash_fuction() , Right.key_eql() , alloc)
	{
		for (const_iterator it = Right.begin(); it != Right.end(); ++it)
			_insert_multiable(*it);
	}

	unordered_multiset(unordered_multiset&& Right)
		:unordered_set_base(STD move (Right._buckets) , STD move (Right._hash) , STD move (Right._eql) ,
			STD move (Right._alloc))
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_multiset (unordered_multiset&& Right , const allocator_type& alloc)
		:unordered_set_base (STD move (Right._buckets) , STD move (Right._hash) , STD move (Right._eql) ,
			alloc)
	{
		_size = Right.size ();
		_last = Right._last;
		_load_factor = Right.load_factor ();
	}

	unordered_multiset(STD initializer_list<value_type> il , size_type n , const hasher& hf = hasher() ,
		const key_equal& eql = key_equal() , const allocator_type& alloc = allocator_type())
		:unordered_set_base(n , hf , eql , alloc)
	{
		for (STD initializer_list<value_type>::const_iterator it = il.begin(); it != il.end(); ++it)
			_insert_multiable(*it);
	}

	unordered_multiset(STD initializer_list<value_type> il ,
		size_type n ,
		const allocator_type& alloc)
		: unordered_set_base (n , hasher() , key_equal() , alloc)
	{
		for (STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert_multiable (*it);
	}

	unordered_multiset (STD initializer_list<value_type> il ,
		size_type n ,
		const hasher&hf ,
		const allocator_type& alloc)
		: unordered_set_base (n , hf , key_equal() , alloc)
	{
		for (STD initializer_list<value_type>::const_iterator it = il.begin (); it != il.end (); ++it)
			_insert_multiable (*it);
	}

	unordered_multiset& operator=(const unordered_multiset& Right)
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

	unordered_multiset& operator=(unordered_multiset&& Right)
	{
		clear ();
		_buckets = STD move (_Right._buckets);
		_hash = STD move (_Right._hash);
		_eql = STD move (_Right._eql);
		_alloc = STD move (_Right._alloc);
		_size = _Right._size;
		_head = _Right._head;
		_last = _Right._last;

		return *this;
	}

	unordered_multiset& operator=(STD initializer_list<value_type> il)
	{
		clear ();
		for (STD initializer_list<key_type>::iterator it = il.begin (); il != il.end (); ++it)
			_insert_multiable (*it);
		return *this;
	}

	~unordered_multiset()
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

	STD pair<iterator , iterator> equal_range(const key_type& key)
	{
		iterator ret1 = find(key);
		iterator ret2(ret1);
		size_type num = count(key);
		for (; num; --num)
			++ret2;
		return STD make_pair(ret1 , ret2);
	}

	STD pair<const_iterator , const_iterator> equal_range(const key_type& key) const
	{
		const_iterator ret1 = find(key);
		const_iterator ret2(ret1);
		size_type num = count(key);
		for (; num; --num)
			++ret2;
		return STD make_pair(ret1 , ret2);
	}

	template<typename ...Args>
	iterator emplace(Args&&...args)
	{
		nodeptr np = new nodeptype<key_type> (STD forward<Args>(args)...);
		nodeptr fp = _find(np->getkey());
		if (fp == nullptr)
			_insert_node(np);
		else
			_insert_after(fp , np);
		return np;
	}

	template<typename ...Args>
	iterator emplace_hint(const_iterator hint , Args&&...args)
	{
		nodeptr np = new nodeptype<key_type> (STD forward<Args>(args)...);
		if (_eql((*hint) , np->getkey()))
			_insert_after(hint.np , np);
		else
		{
			if (fp == nullptr)
				_insert_node(np);
			else
				_insert_after(fp , np);
		}
		return np;
	}

	iterator insert(const value_type& val)
	{
		return _insert_multiable(val);
	}

	iterator insert(value_type&& val)
	{
		return emplace(STD forward<value_type>(val));
	}

	iterator insert(const_iterator hint , const value_type& val)
	{
		return emplace_hint(hint , val);
	}

	iterator insert(const_iterator hint , value_type&& val)
	{
		return emplace_hint(hint , val);
	}

	template<typename Iter>
	void insert(Iter first , Iter last)
	{
		for (; first != last;)
			_insert_multiable(*first);
	}

	void insert(STD initializer_list<value_type> il)
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
			STD pair<iterator , iterator> itp = equal_range(key);
			erase(itp.first , itp.second);
		}
		return ret;
	}

	void clear () noexcept
	{
		_clear ();
	}

	void swap(unordered_set& Right)
	{
		STD swap(_buckets , Right._buckets);
		STD swap(_size , Right._size);
		STD swap(_hash , Right._hash);
		STD swap(_eql , Right._eql);
		STD swap(_alloc , Right._alloc);
		STD swap(_head , Right._head);
		STD swap(_last , Right._last);
		STD swap(_load_factor , Right._load_factor);
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

#endif //CONTAINERS_UNORDERED_SET_HPP