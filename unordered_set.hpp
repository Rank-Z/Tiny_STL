#include<xhash>
#include<utility>
#include<vector>
#include<memory>
#include<initializer_list>
#define _STD ::std::
#define _Member_Data_Start_
#define _Member_Data_End_

template<typename Key>
struct uset_node
{
	const Key key = Key();
	uset_node* next = nullptr;
	uset_node* prev = nullptr;

	uset_node(const Key& k,uset_node* n,uset_node* p):key(k),next(n),prev(p)
	{   }
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

_Member_Data_Start_
		uset_node* np = nullptr;
_Member_Data_End_

	uset_const_iterator() = default;

	uset_const_iterator(uset_node* p) :np(p)
	{   }

	uset_const_iterator(const uset_const_iterator<Key>& _Right) :np(_Right.np)
	{   }

	operator bool() const
	{
		return np != nulllptr;
	}

	uset_const_iterator& operator=(const uset_const_iterator& _Right)
	{
		np = _Right.np;
		return *this;
	}

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
		np = np->prev;
		return *this;
	}

	uset_const_iterator operator--(int)
	{
		uset_const_iterator tmp(*this);
		np = np->prev;
		return tmp;
	}

	const_reference operator*() const
	{
		//TODO:expcetion
		return np->key;
	}

	const node_type& operator->() const
	{
		return np;
	}
};

template<typename Key>
struct uset_iterator
{
	using value_type = Key;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;
	using size_type = unsigned;
	using difference_type = int;
	using node_type = uset_node<Key>;

_Member_Data_Start_
		node_type* np = nullptr;
_Member_Data_End_

	uset_iterator() = default;

	uset_iterator(node_type* p) :np(p)
	{   }

	uset_iterator(const uset_iterator& _Right) :np(_Right.np)
	{   }

	operator uset_const_iterator<Key>() const
	{
		return np;
	}

	operator bool() const
	{
		return np != nullptr;
	}

	uset_iterator& operator=(const uset_iterator& _Right)
	{
		np = _Right.np;
		return *this;
	}

	bool operator==(const uset_iterator& _Right) const
	{
		return np == _Right.np;
	}

	bool operator!=(const uset_iterator& _Right) const
	{
		return np != _Right.np;
	}

	const_reference operator * ()
	{
		//TODO:expcetion
		return np->key;
	}

	const_reference operator*() const
	{
		//TODO:expcetion
		return np->key;
	}

	uset_iterator& operator++()
	{
		//TODO:expcetion
		np = np->next;
		return *this;
	}

	uset_iterator operator++(int)
	{
		//TODO:expcetion
		uset_iterator tmp(np);
		np = np->next;
		return tmp;
	}

	uset_iterator& operator--()
	{
		np = np->prev;
		return *this;
	}

	uset_iterator operator--(int)
	{
		uset_iterator tmp(*this);
		np = np->prev;
		return tmp;
	}

	node_type& operator->()
	{
		return np;
	}

	const node_type& operator->() const
	{
		return np;
	}
};

template<	typename Key,
	typename Hash = _STD hash<Key>,
	typename Pred = _STD equal_to<Key>,
	typename Alloc = _STD allocator<uset_node<Key>>>
	class unordered_set
{
public:
	using key_type = Key;
	using value_type = key_type;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using node = uset_node<Key>;
	using nodeptr = node*;
	using pointer = typename _STD allocator_traits<Alloc>::pointer;				//just value_type*
	using const_pointer = typename _STD allocator_traits<Alloc>::const_pointer;	//just const value_type*
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator = uset_iterator<Key>;
	using const_iterator = uset_const_iterator<Key>;
	using local_iterator = iterator;
	using const_local_iterator = const_iterator;

private:
_Member_Data_Start_
	_STD vector<nodeptr> _buckets;
	size_t _size=0;
	Hash _hash;
	key_equal _eql;
	allocator_type _alloc;
	size_type _head;
	nodeptr _last=nullptr;
	float _load_factor = 1.0;
_Member_Data_End_

	size_t _makehash(const key_type& key) const
	{
		return _hash(key)%_buckets.size();
	}

	nodeptr _buynode(const Key& key,nodeptr n,nodeptr p)
	{
		return _alloc.construct(_alloc.allocate(1), _STD forward<Key>(key),n,p);
	}

	nodeptr _insert(const Key& key)
	{
		if ((nodeptr np=_findkey(key))!=nullptr)
			return np;

		if ((_size + 1) / _buckets.size() >= _load_factor)
			_rehash(_size * 2);

		return _insert_withoutcheck(key);
	}

	nodeptr _insert_withoutcheck(const Key& key)
	{
		size_type hv = _makehash(key);

		if (_size == 0)
			_head = hv;

		if (_buckets[hv])
		{
			_buckets[hv] = _buynode(key, _buckets[hv], _buckets[hv]->prev);
			_buckets[hv]->next->prev = _buckets[hv];
			_buckets[hv]->prev->next = _buckets[hv];
			return _buckets[hv];
		}
		else
		{
			_buckets[hv] = _buynode(key, nullptr, _last);

			if (_last)
				_last->next = _buckets[hv];
			else
				_head = hv;

			return _last = _buckets[hv];
		}
		++_size;
	}

	void _insert_node(nodeptr np)
	{
		size_type hv = _makehash(np->key);

		if (_size == 0)
			_head = hv;

		if (_buckets[hv])
		{
			np->next = _buckets[hv];
			np->prev = _buckets[hv]->prev;
			_buckets[hv] =np;
			_buckets[hv]->next->prev = _buckets[hv];
			_buckets[hv]->prev->next = _buckets[hv];
			return _buckets[hv];
		}
		else
		{
			_buckets[hv] = np;
			np->prev = _last;

			if (_last)
				_last->next = _buckets[hv];
			else
				_head = hv;

			return _last = _buckets[hv];
		}
		++_size;
	}

	bool _delkey(const Key& key)
	{
		if ((nodeptr np = _findkey(key))!=nullptr)
		{
			_delnode(np);
			return true;
		}
		else
			return false;
	}

	nodeptr _findkey(const Key& key) const
	{
		size_type hv = _makehash(key);
		nodeptr np = _buckets[hv];
		for (; np&&_makehash(np->key) == hv;)
			if (np->key == key)
				return np;
		return nullptr; 
	}

	void _delnode(nodeptr where)
	{
		if (where->prev)
		{
			where->prev->next = where->next;
		}
		if (where->next)
		{
			where->next->prev = where->prev;
		}
		if (where == _buckets[_head])
		{
			if (where->next)
				_head = _makehash(where->next->key);
		}
		if (_last == where)
			_last = where->prev;
		_freenode(where);
		--_size;
	}

	void _freenode(nodeptr where)
	{
		_alloc.destroy(where);
		_alloc.deallocate(where, 1);
	}

	void _rehash(size_type newbc)
	{
		if (newbc <= _buckets.size())
			return;

		_STD vector<nodeptr> vec(_buckets);
		_buckets.resize(newbc);
		_buckets.clear();
		_size = 0;
		nodeptr np = vec[_head];
		_last = nullptr;
		for (; np;)
		{
			_insert_node(np);
			np = np->next;
		}
	}

public:
	unordered_set():_buckets(4),_size(0),_hash(Hash()),_eql(Pred()),_alloc(allocator_type())
	{   }

	explicit unordered_set(size_type n,
		const hasher& hf = hasher(),
		const key_equal& eq = key_equal(),
		const allocator_type& alloc = allocator_type())
		:_buckets(n),_size(0),_hash(hf),_eql(eq),_alloc(allocator_type())
	{   }

	explicit unordered_set(const allocator_type& alloc)
		:_buckets(4),_size(0),_hash(Hash()),_eql(Pred()),_alloc(alloc)
	{   }

	unordered_set(size_type n,const allocator_type& alloc)
		:_buckets(n),_size(0),_hash(Hash()),_eql(Pred()),_alloc(alloc)
	{   }

	unordered_set(size_type n,const hasher& hf,const allocator_type& alloc)
		:_buckete(4),_size(0),_hash(hf),_eql(Pred()),_alloc(alloc)
	{   }

	template<typename Iter>
	unordered_set(	Iter first, Iter last,
					size_type n,
					const hasher&hf=hasher(),
					const key_equal& eq=key_equal(),
					const allocator_type& alloc=allocator_type())
		:_buckets(n),_hash(hf),_eql(eq),_alloc(alloc)
	{
		for (; first != last; ++first)
			_insert(*first);
	}

	template<typename Iter>
	unordered_set(Iter first, Iter last, size_type n, const allocator_type& alloc)
		:_buckets(n), _hash(Hash()), _eql(Pred()), _alloc(alloc)
	{
		for (; first != last; ++first)
			_insert(*first);
	}

	template<typename Iter>
	unordered_set(Iter first, Iter last, size_type n, const hasher& hf, const allocator_type& alloc)
		:_buckets(n), _hash(hf), _eql(Pred()), _alloc(alloc)
	{
		for (; first != last; ++first)
			_insert(*first);
	}

	unordered_set(const unordered_set& _Right)
		:_buckets(_Right._buckets.size()), _hash(_Right._hash), _eql(_Right._eql), _alloc(_Right._alloc)
	{
		for (iterator it=_Right.begin(); it; ++it)
			_insert(*it);
	}

	unordered_set(const unordered_set& _Right, const allocator_type& alloc)
		:_buckets(_Right._buckets.size()), _hash(_Right._hash), _eql(_Right._eql), _alloc(alloc)
	{
		for (iterator it = _Right.begin(); it; ++it)
			_insert(*it);
	}

	unordered_set(const unordered_set&& _Right)
		:_buckets(_STD move(_Right._buckets)),_size(_Right._size),_hash(_STD move(_Right._hash)),
		_eql(_STD move(_Right._eql)), _alloc(_STD move(_Right._alloc))
	{   }

	unordered_set(const unordered_set&& _Right,const allocator_type& alloc)
		:_buckets(_STD move(_Right._buckets)), _size(_Right._size), _hash(_STD move(_Right._hash)),
		_eql(_STD move(_Right._eql)), _alloc(_STD move(alloc))
	{   }

	unordered_set(_STD initializer_list<key_type> il, size_type n, const hasher&hf = hasher(),
		const key_equal&eq = key_equal(), const allocator_type& alloc = allocator_type())
		:_buckets(n), _hash(hf), _eql(eq), _alloc(alloc)
	{
		for (auto&t : il)
			_insert(t);
	}

	unordered_set(_STD initializer_list<key_type> il, size_type n, const allocator_type& alloc)
		:_buckets(n), _hash(Hash()), _eql(Pred()), _alloc(alloc)
	{
		for (auto&t : il)
			_insert(t);
	}

	unordered_set(_STD initializer_list<key_type> il, size_type n, const hasher& hf, const allocator_type& alloc)
		:_buckets(n), _hash(hf), _eql(Pred()), _alloc(alloc)
	{
		for (auto&t : il)
			_insert(t);
	}

	~unordered_set()
	{
		clear();
	}

	unordered_set& operator=(const unordered_set& _Right)
	{
		clear();
		_buckets.resize(_Right._buckets.size());
		_hash = _Right._hash;
		_eql = _Right._eql;
		_alloc = _Right._alloc;
		for (iterator it = _Right.begin(); it != _Right.end(); ++it)
			_insert(*it);

		return *this;
	}

	unordered_set& operator=(const unordered_set&& _Right)
	{
		clear();
		_buckets = _STD move(_Right._buckets);
		_hash = _STD move(_Right._hash);
		_eql = _STD move(_Right._eql);
		_alloc = _STD move(_Right._alloc);
		_size = _Right._size;
		_head = _Right._head;
		_last = _Right._last;

		_Right.clear();

		return *this;
	}

	unordered_set& operator=(_STD initializer_list<key_type> il)
	{
		clear();
		for (_STD initializer_list<key_type>::iterator it = il.begin(); il != il.end(); ++it)
			_insert(*it);
		return *this;
	}

	bool empty() const
	{
		return (_size == 0);
	}

	size_type size() const
	{
		return _size;
	}

	size_type max_size() const
	{
		return 536870911;
	}

	iterator begin() noexcept
	{
		return _buckets[_head];
	}

	const_iterator begin() const noexcept
	{
		retunr _buckets[_head];
	}

	const_iterator cbegin() const noexcept
	{
		return _buckets[_head];
	}

	local_iterator begin(size_type n)
	{
		if(n>=_buckets.size())
			return nullptr
		else
			return _buckets[n];
	}

	const_local_iterator begin(size_type n) const
	{
		if (n >= _buckets.size())
			return nullptr
		else
			return _buckets[n];
	}
	
	const_local_iterator cbegin(size_type n) const
	{
		if (n >= _buckets.size())
			return nullptr
		else
			return _buckets[n];
	}

	iterator end() noexcept
	{
		return nullptr;
	}

	const_iterator end() const noexcept
	{
		return nullptr;
	}

	const_iterator cend() const noexcept
	{
		return nullptr;
	}

	local_iterator end(size_type n)
	{
		if (n >= _buckets.size())
			return nullptr;
		
		if (!_buckets[n])
			return nullptr;

		nodeptr np = _buckets[n];
		for (; np->next&&n == _makehash(np->next->key); np = np->next);
		return np->next;
	}

	const_local_iterator end(size_type n) const
	{
		return static_cast<const_local_iterator>(end(n));
	}

	const_local_iterator cend(size_type n) const
	{
		return end(n);
	}

	iterator find(const Key& key)
	{
		return _findkey(key);
	}

	const_iterator find(const Key& key) const
	{
		return _findkey(key);
	}

	size_type count(const Key& key) const
	{
		return _findkey(key) != nullptr;
	}

	_STD pair<iterator, iterator> equal_range(const Key& key)
	{
		iterator ret = _findkey(key);
		return _STD make_pair(ret, ret);
	}
	
	_STD pair<const_iterator, const_iterator> equal_range(const Key& key) const
	{
		const_iterator ret = _findkey(key);
		return _STD make_pair(ret, ret);
	}

	template<typename ...Args>
	_STD pair<iterator, bool> emplace(Args&&...agrs)
	{
		Key k(Args&&...args);
		if ((nodeptr np = _findkey(k)) != nullptr)
			return _STD make_pair(static_cast<iterator>(np), false);
		else
			return _STD make_pair(static_cast<iterator>(_insert(k)), true);
	}

	template <class... Args>
	iterator emplace_hint(const_iterator hint, Args&&... args)
	{
		Key k(Args..args);
		if (*hint == k)
			return hint;

		return _insert(k);
	}

	_STD pair<iterator, bool> insert(const Key& key)
	{
		if (iterator it (_findkey(key)))
			return _STD make_pair(it, false);

		return _STD make_pair(_insert(key), true);
	}

	_STD pair<iterator, bool> insert(Key&& key)
	{
		if (iterator it(_findkey(key)))
			return _STD make_pair(it, false);

		return _STD make_pair(_insert(_STD forward<Key>(key)), false);
	}

	iterator insert(const_iterator hint,const Key& key)
	{
		if (*hint == key)
			return hint;

		return _insert(key);
	}

	iterator insert(const_iterator hint, Key&& key)
	{
		if (*hint == key)
			return hint;

		return _insert(_STD forward<Key>(key));
	}

	template<typename Iter>
	void insert(Iter first, Iter last)
	{
		for (; first != last; ++first)
			_insert(*first);
	}

	void insert(_STD initializer_list<key_type> il)
	{
		for (auto&t : il)
			_insert(t);
	}

	iterator erase(const_iterator position)
	{
		iterator ret(position.np->next);
		_delnode(position.np);
		return ret;
	}

	size_type erase(const Key& key)
	{
		return _delkey(key);
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		nodeptr np = first.np;
		np->prev->next = last.np;
		for (; first != last; ++first)
			_freenode(first.np);
		return last.np;
	}

	void clear() noexcept
	{
		for (auto&t : _buckets)
		{
			if (t)
			{
				for (nodeptr np = t; np;)
				{
					nodeptr tmp = np;
					np = np->next;
					delete tmp;
				}
			}
			t = nullptr;
		}
		_size = 0;
		_last = nullptr;
	}

	void swap(unordered_set& _Right)
	{
		_buckets.swap(_Right._buckets);

		_size = _size^_Right._size;
		_Right._size = _size^_Right._size;
		_size = _size^_Right._size;

		Hash thf = _hash;
		_hash = _Right._hash;
		_Right._hash = thf;

		key_equal teq = _eql;
		_eql = _Right._eql;
		_Right._eql = teq;

		_head = _head^_Right._head;
		_Right._head = _head^_Right._head;
		_head = _head^_Right._head;

		allocator_type talloc = _alloc;
		_alloc = _Right._alloc;
		_Right._alloc = talloc;

		nodeptr tnp = _last;
		_last = _Right._last;
		_Right._last = _last;
	}

	size_type bucket_count() const noexcept
	{
		return _buckets.size();
	}

	size_type max_bucket_count() const noexcept
	{
		return 536870911;
	}

	size_type bucket_size(size_type n) const
	{
		if (n >= _buckets.size() || !_buckets[n])
			return 0;

		nodeptr np = _buckets[n]->next;
		size_type ret = 1;
		for (; np&&n == _makehash(np->key); np = np->next)
			++ret;
		return ret;
	}

	size_type bucket(const Key& key) const
	{
		return _makehash(key);
	}

	float load_factor() const noexcept
	{
		return static_cast<float>(_size) / _buckets.size();
	}

	float max_load_factor() const noexcept
	{
		return _load_factor;
	}

	void max_load_factor(float newlf)
	{
		if (newlf == _load_factor || newlf <= 0)
			return;

		if (newlf < load_factor())
			rehash(_size / newlf);
		_load_factor = newlf;
	}

	void rehash(size_type n)
	{
		_rehash(n);
	}

	void reserve(size_type n)
	{
		if (n >= _buckets.size()*_load_factor)
			_rehash(n / _load_factor);
	}

	hasher hash_function() const
	{
		return _hash;
	}

	key_equal key_eq() const
	{
		return _eql;
	}

	allocator_type get_allocator() const noexcept
	{
		return _alloc;
	}
};

template<typename Key,
	typename Hash,
	typename Pred,
	typename Alloc>
	bool operator ==(const unordered_set<Key, Hash, Pred, Alloc>& _Left,
		const unordered_set<Key, Hash, Pred, Alloc>& _Right)
{
	if (_Left.size() != _Right.size())
		return false;

	for (unordered_set<Key, Hash, Pred, Alloc>::const_iterator it = _Left.begin(); it != _Left.end(); ++it)
		if (_Right.find(*it) == nullptr)
			return false;

	return true;
}

template<typename Key,
	typename Hash,
	typename Pred,
	typename Alloc>
	bool operator !=(const unordered_set<Key, Hash, Pred, Alloc>& _Left,
		const unordered_set<Key, Hash, Pred, Alloc>& _Right)
{
	return !(_Left == _Right);
}

template<typename Key,
	typename Hash,
	typename Pred,
	typename Alloc>
	void swap(unordered_set<Key, Hash, Pred, Alloc>& _Left,
		unordered_set<Key, Hash, Pred, Alloc>& _Right)
{
	_Left.swap(_Right);
}