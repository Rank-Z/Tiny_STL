#ifndef CONTAINERS_UNORDERED_BASE_HPP
#define CONTAINERS_UNORDERED_BASE_HPP
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

	umap_node () = default;

	umap_node(const Key& key, const T& value, umap_node* n = nullptr, umap_node* p = nullptr) :data(key, value), next(n), prev(p)
	{   }

	umap_node(const _STD pair<Key, T>& pr, umap_node* n = nullptr, umap_node* p = nullptr) :data(pr), next(n), prev(p)
	{   }

	const key_type& getkey() 
	{
		return data.first;
	}

	mapped_type& getmapped() 
	{
		return data.second;
	}

	value_type& getpair()
	{
		return data;
	}

	_STD pair<const Key, T> data;
	umap_node* next = nullptr;
	umap_node* prev = nullptr;
};

template<typename nodetype,
	typename Hash,
	typename Pred,
	typename Alloc>
	class unordered_base
{
protected:
	using nodeptr = nodetype * ;
	using size_type = size_t;
	using hasher = Hash;
	using key_equal = Pred;
	using allocator_type = Alloc;
	using key_type = typename nodetype::key_type;
	using value_type = typename nodetype::value_type;

	unordered_base ()
		:_buckets (4,nullptr),_hash(hasher()),_eql(key_equal()),_alloc(allocator_type())
	{   }

	unordered_base (size_type n ,const hasher& hf ,const key_equal& eql ,const allocator_type& alloc)
		:_buckets (n > 4 ? n : 4 , nullptr) , _hash (hf) , _eql (eql) , _alloc (alloc)
	{   }

	unordered_base(_STD vector<nodeptr>&& vn,hasher&& hf,key_equal&& eql,allocator_type&& alloc)
		:_buckets(_STD move(vn)),_hash(_STD move(hf)),_eql(_STD move(eql)),_alloc(_STD move(alloc))
	{   }

	unordered_base (_STD vector<nodeptr>&& vn , hasher&& hf , key_equal&& eql ,const allocator_type& alloc)
		:_buckets (_STD move (vn)) , _hash (_STD move (hf)) , _eql (_STD move (eql)) , _alloc (alloc)
	{   }

	size_type _makehash(const key_type& key) const
	{
		return _hash(key) % _buckets.size();
	}

	void _rehash(size_type newbc)
	{
		if (newbc <= _buckets.size())
			return;
		if (_size == 0)
		{
			_buckets.resize (newbc);
			return;
		}

		_STD vector<nodeptr> vec(_buckets);
		_buckets.clear ();
		_buckets.resize(newbc);
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
		size_type hv = _makehash(np->getkey());

		if (_size == 0)
			_head = hv;

		if (_buckets[hv])
		{
			np->next = _buckets[hv];
			np->prev = _buckets[hv]->prev;
			_buckets[hv] = np;
			_buckets[hv]->next->prev = _buckets[hv];
			_buckets[hv]->prev->next = _buckets[hv];
		}
		else
		{
			_buckets[hv] = np;
			np->prev = _last;

			if (_last)
				_last->next = _buckets[hv];
			else
				_head = hv;
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
		for (; np&&_makehash(np->getkey()) == hv;)
			if (_eql(np->getkey(), key))
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
				_head = _makehash(where->next->getkey());
		}
		if (_last == where)
			_last = where->prev;
		size_t hv = _makehash(where->getkey());
		if (_buckets[hv] == where)
		{
			if (where->next)
			{
				if (hv == _makehash(where->next->getkey()))
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
		_buckets.resize (_size);
		_size = 0;
		_last = nullptr;
	}

	size_type _bucket_count() const
	{
		return _buckets.size();
	}

	size_type _max_bucket_count() const
	{
		return 357913941;
	}

	size_type _bucket_size(size_type n) const
	{
		if (n >= _buckets.size() || !_buckets[n])
			return 0;

		nodeptr np = _buckets[n]->next;
		size_type ret = 1;
		for (; np&&n == _makehash(np->getkey()); np = np->next)
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

		if (newlf < _load_factor)
			_rehash(_size / newlf);
		_load_factor = newlf;
	}

	void _reserve(size_type n)
	{
		if (n >= _buckets.size()*_load_factor)
			_rehash(n / _load_factor);
	}

	hasher _hash_function() const
	{
		return _hash;
	}

	key_equal _key_eq() const
	{
		return _eql;
	}

	allocator_type _get_allocator() const noexcept
	{
		return _alloc;
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



#endif //CONTAINERS_UNORDERED_BASE_HPP
