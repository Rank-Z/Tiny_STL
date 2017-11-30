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
	Key key;
	uset_node* next = nullptr;
	uset_node* prev = nullptr;

	uset_node(Key& k,uset_node* n,uset_node* p):key(k),next(n),prev(p)
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

	reference operator * ()
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
	using unordered_set = unordered_set<Key>;

private:
_Member_Data_Start_
	_STD vector<nodeptr> _buckets;
	size_t _size=0;
	Hash _hash;
	key_equal _eql;
	allocator_type _alloc;
	size_type _head;
	nodeptr _last=nullptr;
_Member_Data_End_

	size_t _makehash(key_type& key) const
	{
		return _hash(key)%_buckets.size();
	}

	nodeptr _buynode(Key& key,nodeptr n,nodeptr p)
	{
		return _alloc.construct(_alloc.allocate(1), _STD forward(key),n,p);
	}

	void _insert(Key& key)
	{
		size_type hv = _makehash(key);

		if (_size == 0)
			_head = hv;

		if (_buckets[hv])
		{
			_buckets[hv] = _buynode(key, _buckets[hv], _buckets[hv]->prev);
			_buckets[hv]->next->prev = _buckets[hv];
			_buckets[hv]->prev->next = _buckets[hv];
		}
		else
		{
			_buckets[hv] = _buynode(key, nullptr, _last);

			if (_last)
				_last->next = _buckets[hv];
			else
				_head = hv;

			_last = _buckets[hv];
		}
		++_size;
	}

	bool _delkey(Key& key)
	{
		if (nodeptr np = _findkey(key))
		{
			_delnode(np);
			return true;
		}
		else
			return false;
	}

	nodeptr _findkey(Key& key) const
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
		delete where;
	}

	void _rehash(size_type newsize)
	{

	}

	void _erase(iterator where)
	{

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

	unordered_set(_STD initializer_list<value_type> il, size_type n, const hasher&hf = hasher(),
		const key_equal&eq = key_equal(), const allocator_type& alloc = allocator_type())
		:_buckets(n), _hash(hf), _eql(eq), _alloc(alloc)
	{
		for (auto&t : il)
			_insert(t);
	}

	unordered_set(_STD initializer_list<value_type> il, size_type n, const allocator_type& alloc)
		:_buckets(n), _hash(Hash()), _eql(Pred()), _alloc(alloc)
	{
		for (auto&t : il)
			_insert(t);
	}

	unordered_set(_STD initializer_list<value_type> il, size_type n, const hasher& hf, const allocator_type& alloc)
		:_buckets(n), _hash(hf), _eql(Pred()), _alloc(alloc)
	{
		for (auto&t : il)
			_insert(t);
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
		return _buckets[n];
	}

	const_local_iterator begin(size_type n) const
	{
		return _buckets[n];
	}
	
	const_local_iterator cbegin(size_type n) const
	{
		return _buckets[n];
	}


	

	void clear()
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
};

