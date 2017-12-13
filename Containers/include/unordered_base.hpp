#include<xhash>
#include<utility>
#include<vector>
#include<memory>
#include<initializer_list>
#include<exception>
#define _STD ::std::
#define _Member_Data_Start_
#define _Member_Data_End_

template<typename Key, typename T>
class umap_node
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const Key, T>;

	umap_node() = default;

	umap_node(const Key& key, const T& value, const umap_node* n = nullptr, const umap_node* p = nullptr) :data(key, value), next(n), prev(p)
	{   }

	umap_node(const _STD pair<Key, T> pr, const umap_node* n = nullptr, const umap_node* p = nullptr) :data(pr), next(n), prev(p)
	{   }

	key_type& getkey() const
	{
		return data.first;
	}

	mapped_type& getmapped() const
	{
		return data.second;
	}

	value_type& getpair() const
	{
		return data;
	}

private:
	_STD pair<const Key, T> data;
	umap_node* next = nullptr;
	umap_node* prev = nullptr;



};

//template<typename nodetype>
//class uo_iterator_base
//{
//	using reference = nodetype::value_type;
//
//	nodetype* np = nullptr;
//
//	uo_iterator_base(void* p) :np(static_cast<nodetype*>(p));
//
//	uo_iterator_base& operator=(const uo_iterator_base& Right)
//	{
//		np = Right.np;
//	}
//
//	bool operator==(const uo_iterator_base& Right) const
//	{
//		return (np == Right.np);
//	}
//
//	bool operator!=(const uo_iterator_base& Right) const
//	{
//		return (np != Right.np);
//	}
//
//	uo_iterator_base& operator++()
//	{
//		if (np == nullptr)
//			throw _STD out_of_range("iterator");
//		np = np->next;
//		return *this;
//	}
//
//	uo_iterator_base operator++(int)
//	{
//		uo_iterator_base ret = *this;
//		++(*this);
//		return ret;
//	}
//
//	uo_iterator_base& operator--()
//	{
//		if (np == nullptr)
//			throw _STD out_of_range("iterator");
//		np = np->prev;
//		return *this;
//	}
//
//	uo_iterator_base operator--(int)
//	{
//		uo_iterator_base ret = *this;
//		--(*this);
//		return ret;
//	}
//};

template<typename nodetype,
	typename Hash,
	typename Pred,
	typename Alloc>
	class unordered_base
{
	using nodeptr = nodetype * ;
	using size_type = size_t;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using key_type = typename nodetype::Key;
	using value_type = typename nodetype::value_type;

protected:
	size_type _makehash(const key_type& key) const
	{
		return _hash(key) % _buckets.size();
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

	void _insert_node(nodeptr np)
	{
		size_type hv = _makehash(np->key);

		if (_size == 0)
			_head = hv;

		if (_buckets[hv])
		{
			np->next = _buckets[hv];
			np->prev = _buckets[hv]->prev;
			_buckets[hv] = np;
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

	void _insert_after(nodeptr where, nodeptr which)
	{
		which->next = where->next;
		which->prev = where;
		where->next = which;
		if (which->next)
			which->next->prev = which;
		++_size;
	}

	nodeptr _findkey(const key_type& key) const
	{
		size_type hv = _makehash(key);
		nodeptr np = _buckets[hv];
		for (; np&&_makehash(np->key) == hv;)
			if (_eql(np->key, key))
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
		size_t hv = _makehash(where->key);
		if (_buckets[hv] == where)
		{
			if (where->next)
			{
				if (hv == _makehash(where->next->key))
					_buckets[hv] = where->next;
				else
					_buckets[hv] = nullptr;
			}
			else
				_buckets[hv] = nullptr;
		}
		_freenode(where);
		--_size;
	}

	void _freenode(nodeptr where)
	{
		_alloc.destroy(where);
		_alloc.deallocate(where, 1);
	}

	void _clear() noexcept
	{
		for (nodeptr np = _buckets[_head]; np;)
		{
			nodeptr tmp = np->next;
			_freenode(np);
			np = tmp;
		}
		_buckets.clear();
		_size = 0;
		_last = nullptr;
	}

	size_type _bucket_size(size_type n) const
	{
		if (n >= _buckets.size() || !_buckets[n])
			return 0;

		nodeptr np = _buckets[n]->next;
		size_type ret = 1;
		for (; np&&n == _makehash(np->key); np = np->next)
			++ret;
		return ret;
	}

	float _get_load_factor() const noexcept
	{
		return static_cast<float>(_size) / _buckets.size();
	}

	void _max_load_factor(float newlf)
	{
		if (newlf == _load_factor || newlf <= 0)
			return;

		if (newlf < load_factor())
			_rehash(_size / newlf);
		_load_factor = newlf;
	}

	void _reserve(size_type n)
	{
		if (n >= _buckets.size()*_load_factor)
			_rehash(n / _load_factor);
	}


	_STD vector<nodeptr> _buckets;
	size_type _size = 0;
	Hash _hash;
	key_equal _eql;
	allocator_type _alloc;
	size_type _head = 0;
	nodeptr _last = nullptr;
	float _load_factor = 1.0;
};


template<typename Key,
	typename T,
	typename Hash,
	typename Pred,
	typename Alloc>
	class unordered_map_base :public unordered_base<umap_node<Key, T>, Hash, Pred, Alloc>
{
	using nodetype = umap_node<Key, T>;
	using nodeptr = nodetype * ;
	using value_type = typename nodetype::value_type;
	using map_type = typename nodetype::map_type;
	using key_type = typename nodetype::key_type;
protected:
	nodeptr _buynode(const key_type& key, const map_type& val, nodeptr n, nodeptr p)
	{
		nodeptr ret = _alloc.allocate(1);
		_alloc.construct(ret, _STD forward<key_type>(key), _STD forward<map_type>(val), n, p);
		return np;
	}

	nodeptr _insert(const value_type& pv)
	{
		if ((nodeptr np = _findkey(pv.first)) != nullptr)
			return np;

		if (((_size + 1) / _buckets.size()) >= _load_factor)
			_rehash(_size * 2);

		return _insert_withoutcheck(pv);
	}

	nodeptr _insert_withoutcheck(const value_type& pv)
	{
		size_type hv = _makehash(pv.first);
		++_size;
		if (_size == 0)
			_head = hv;

		if (_buckets[hv])
		{
			_buckets[hv] = _buynode(pv.first, pv.second, _buckets[hv], _buckets[hv]->prev);
			_buckets[hv]->next->prev = _buckets[hv];
			_buckets[hv]->prev->next = _buckets[hv];
			return _buckets[hv];
		}
		else
		{
			_buckets[hv] = _buynode(pv.first, pv.second, nullptr, _last);

			if (_last)
				_last->next = _buckets[hv];
			else
				_head = hv;

			return _last = _buckets[hv];
		}
	}

	nodeptr _insert_multiable(const value_type& pv)
	{

	}





};
/*                                        map_iterator                          */
template<typename Key, typename T>
class map_const_iterator
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const Key, T>;
	using node_type = umap_node<Key, T>;
	using nodeptr = node_type * ;

	map_const_iterator() = default;

	map_const_iterator(nodeptr p) :np(p)
	{   }

	map_const_iterator(const map_const_iterator& Right) :np(Right.np)
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
			throw _STD out_of_range{ "map_const_iterator operator++() out_of_range" };
		np = np->next;
		return *this;
	}

	map_const_iterator operator++(int)
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_const_iterator operator++(int) out_of_range" };
		map_const_iterator tmp(*this);
		np = np->next;
		return tmp;
	}

	map_const_iterator& operator--()
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_const_iterator operator--() out_of_range" };
		np = np->prev;
		return *this;
	}

	map_const_iterator operator--(int)
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_const_iterator operator--(int) out_of_range" };
		map_const_iterator tmp(*this);
		np = np->prev;
		return tmp;
	}

	const value_type& operator*() const
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_const_iterator operator--(int) out_of_range" };
		return np->data;
	}

	const node_type& operator->() const
	{
		return *np;
	}
private:
	nodeptr np = nullptr;
};


template<typename Key, typename T>
class map_iterator
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = _STD pair<const Key, T>;
	using node_type = umap_node<Key, T>;
	using nodeptr = node_type * ;

	map_iterator() = default;

	map_iterator(nodeptr p) :np(p)
	{   }

	map_iterator(const map_iterator& Right) :np(Right.np)
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
			throw _STD out_of_range{ "map_iterator operator++() out_of_range" };
		np = np->next;
		return *this;
	}

	map_iterator operator++(int)
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_iterator operator++(int) out_of_range" };
		map_iterator tmp(*this);
		np = np->next;
		return tmp;
	}

	map_iterator& operator--()
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_iterator operator--() out_of_range" };
		np = np->prev;
		return *this;
	}

	map_iterator operator--(int)
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_iterator operator--(int) out_of_range" };
		map_iterator tmp(*this);
		np = np->prev;
		return tmp;
	}

	value_type operator*() const
	{
		if (np == nullptr)
			throw _STD out_of_range{ "map_iterator operator*() out_of_range" };
		return np->data;
	}

	node_type& operator->() const
	{
		return *np;
	}

	nodeptr np = nullptr;
};




