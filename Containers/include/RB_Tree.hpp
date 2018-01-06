#ifndef CONTAINERS_REDBLACKTREE_HPP
#define CONTAINERS_REDBLACKTREE_HPP

#include<functional>
#include<memory>
#include<utility>
#define _STD ::std::

template<typename Key>
class RB_node
{
public:
	using key_type = Key;
	using value_type = key_type;

	RB_node(const Key&key , RB_node* p = nullptr , RB_node* l = nullptr , RB_node* r = nullptr)
		:k(key) , par(p) , left(l) , right(r)
	{   }

	RB_node()
		:k(Key()) , par(nullptr) , left(nullptr) , right(nullptr) , isred(false)
	{   }

	Key& getkey()
	{
		return k;
	}

	Key k;
	bool isred;
	RB_node* par;
	RB_node* left;
	RB_node* right;
};//TODO:delete




template<typename nodetype ,
	typename Itertype ,
	typename Compare ,
	typename Allocator>
	class RB_Tree
{
public:
	using Key = typename nodetype::key_type;
	using key_type = Key;
	using value_type = typename nodetype::value_type;
	using nodeptr = nodetype * ;
	using allocator_type = Allocator;
	using iterator = Itertype;
	using key_compare = Compare;
	using ketptr = key_type * ;

	RB_Tree()
	{
		init()
	}

	RB_Tree(const Compare& comp,const Allocator& alloc=Allocator())
		:_compare(comp),_alloc(alloc)
	{
		init()
	}

	RB_Tree(const Allocator& alloc)
		:_alloc(alloc)
	{
		init();
	}

	RB_Tree(const RB_Tree& right,const Allocator& alloc)
		:_compare(right.key_comp()),_alloc(alloc)
	{
		init();
		nodeptr np = right.first();
		for (; np != right.getroot(); np = right.successor(np))
			insert_multiable(np->getkey());
	}

	RB_Tree(const RB_Tree& right)
		:RB_Tree(right,
			_STD allocator_traits<Allocator>::select_on_container_copy_construction(right.get_allocator()))
	{   }

	RB_Tree(RB_Tree&& right)
		:_compare(_STD move(right.key_comp())),_alloc(_STD move(right.get_allocator())),
		nilnode(_STD move(*(right.getnil())))
	{
		init();
	}

	RB_Tree(RB_Tree&& right , const Allocator& alloc)
		:RB_Tree(right)
	{
		_alloc = alloc;
	}

	RB_Tree& operator=(const RB_Tree& right)
	{
		clear();
		nodeptr np = right.first();
		for (; np != right.getroot(); np = right.successor(np))
			insert_multiable(np->getkey());
		return *this;
	}

	RB_Tree& operator=(RB_Tree&& right)
	{
		clear();
		nilnode = _STD move(right.nilnode);
		nil = right.nil;
		root = right.root;
		_compare = _STD move(right._compare);
		_alloc = _STD move(right._alloc);
		
		return *this;
	}

	void swap(RB_Tree& right)
	{

	}

	~RB_Tree()
	{
		clear();
	}

	nodeptr getroot()
	{
		return root;
	}

	nodeptr getnil()
	{
		return nil;
	}

	bool empty()
	{
		return (root == nil);
	}

	iterator begin() const
	{
		nodeptr np = first();
		if (np == nullptr)
			np = nil;

		iterator it(np , this);
		return it;
	}

	iterator end() const
	{
		iterator it(nil , this);
		return it;
	}

	int back_height() const
	{
		nodeptr np = root;
		int back_count = 0;
		for (; np != nil;)
		{
			if (!np->isred)
				++back_count;
			np = np->left();
		}
		return back_count;
	}

	void insert_multiable(const key_type& key)
	{
		nodeptr np = buynode(key);
		insert(np);
	}

	_STD pair<iterator,bool> insert_unique(const key_type& key)
	{
		nodeptr np = find(key);
		bool ret = false;
		if (np == nullptr)
		{
			np = buynode(key);
			insert(np);
			ret = true;
		}

		return _STD make_pair(make_iter(np) , ret);
	}

	_STD pair<iterator , bool> insert_unique(key_type&& key)
	{
		nodeptr np = find(key);
		bool ret = false;
		if (np == nullptr)
		{
			np = buynode_rvalue(key);
			insert(np);
			ret = true;
		}

		return _STD make_pair(make_iter(np) , ret);
	}

	void insert(nodeptr np)
	{
		nodeptr parent = nil;
		for (nodeptr temp = root; temp != nil;)
		{
			parent = temp;
			if (np->getkey() < temp->getkey())
				temp = temp->left;
			else
				temp = temp->right;
		}
		np->par = parent;
		if (parent == nil)
			root = np;
		else if (np->getkey() < parent->getkey())
			parent->left = np;
		else
			parent->right = np;
		np->right = nil;
		np->left = nil;
		np->isred = true;
		insert_fixup(np);

		++_size;
	}

	void quick_insert(nodeptr hint , nodeptr np)
	{

	}

	nodeptr find(const key_type& key) const
	{
		nodeptr now = root;
		for (; now;)
		{
			if ((!_compare(key , now->getkey()))&& (!_compare(now->getkey() , key)))
				break;
			else if (_compare(key,now->getkey()))
				now = now->left;
			else
				now = now->right;
		}
		if (now != nil)
			return now;
		else
			return nullptr;
	}

	int find_or_parent(const key_type& key,nodeptr& now) const
	{
		if (root == nil)
			return nullptr;

		now = root;
		for (;;)
		{
			if ((!_compare(key , now->getkey())) && (!_compare(now->getkey() , key)))
				return 0;
			else if (_compare(key , now->getkey()))
			{
				if (now->left == nil)
					return 1;
				else
					now = now->left;
			}
			else
			{
				if (now->right == nil)
					return -1;
				else
					now = now->right;
			}
		}
	}

	iterator lower_bound(const key_type& key) const
	{
		nodeptr np;
		switch (find_or_parent(key , np))
		{
		case 0:
		case 1:
			return make_iter(np);
		case -1:
			return ++make_iter(np);
		}
	}

	iterator upper_bound(const key_type& key) const
	{
		nodeptr np;
		switch (find_or_parent(key , np))
		{
		case 1:
			return make_iter(np);
		case 0:
		case -1:
			return ++make_iter(np);
		}
	}

	bool deletekey(const key_type& key)
	{
		nodeptr np = find(key);
		if (np)
		{
			erase(np);
			return true;
		}
		else
			return false;
	}

	iterator erase(iterator pos)
	{
		iterator ret(pos);
		++ret;
		erase(pos.node);
		return ret;
	}

	void erase(nodeptr z)
	{
		nodeptr x;
		nodeptr y = z;
		bool y_original_color = y->isred;
		if (z->left == nil)
		{
			x = z->right;
			transplant(z , z->right);
		}
		else if (z->right == nil)
		{
			x = z->left;
			transplant(z , z->left);
		}
		else
		{
			y = minimum(z->right);
			y_original_color = y->isred;
			x = y->right;
			if (y->par == z)
				x->par = y;
			else
			{
				transplant(y , y->right);
				y->right = z->right;
				y->right->par = y;
			}
			transplant(z , y);
			y->left = z->left;
			y->left->par = y;
			y->isred = z->isred;
		}
		if (y_original_color == false)
			delete_fixup(x);

		freenode(z);
		--_size;
	}

	void clear()
	{
		cleartree(root);
		root = nil;
	}

	nodeptr successor(nodeptr np) const
	{
		if (np->right != nil)
			return minimum(np->right);

		nodeptr parent = np->par;
		for (; parent != nil && np == parent->right;)
		{
			np = parent;
			parent = parent->par;
		}

			return parent;
	}

	nodeptr predecessor(nodeptr np) const
	{
		if (np->left != nil)
			return maxmum(np->left);

		nodeptr parent = np->par;
		for (; parent != nil && np == parent->left)
		{
			np = parent;
			parent = parent->par;
		}

			return parent;
	}

	nodeptr first() const
	{
			return minimum(root);
	}

	nodeptr last() const
	{
			return maxmum(root);
	}

	unsigned size() const noexcept
	{
		return _size;
	}

	Allocator& get_allocator()
	{
		return _alloc;
	}

	Compare& key_comp()
	{
		return _compare;
	}

	iterator make_iter(nodeptr np) const
	{
		iterator it(np , this);
		return it;
	}

private:

	template<typename ...Args>
	nodeptr buynode(Args&&...args)
	{
		nodeptr np = new nodetype;
		np->kp = _alloc.allocate(1);
		_alloc.construct(np->kp ,_STD forward<Args>(args)...);
		return np;
	}

	nodeptr buynode_rvalue(key_type&& key)
	{
		nodeptr np = new nodeptr;
		np->kp = &key;
		return np;
	}

	void freenode(nodeptr np)
	{
		_alloc.deallocate(np->kp,1);
		delete np;
	}

	void insert_fixup(nodeptr np)
	{
		for (; np->par->isred;)
		{

			nodeptr parbro;
			if (np->par == np->par->par->left)
			{
				parbro = np->par->par->right;

				if (parbro->isred)
				{
					np->par->isred = false;
					parbro->isred = false;
					np->par->par->isred = true;
					np = np->par->par;
				}
				else
				{
					if (np == np->par->right)
					{
						np = np->par;
						leftrotate(np);
					}
					np->par->isred = false;
					np->par->par->isred = true;
					rightrotate(np->par->par);
				}
			}
			else
			{
				parbro = np->par->par->left;
				if (parbro->isred)
				{
					np->par->isred = false;
					parbro->isred = false;
					np->par->par->isred = true;
					np = np->par->par;
				}
				else
				{
					if (np == np->par->left)
					{
						np = np->par;
						rightrotate(np);
					}
					np->par->isred = false;
					np->par->par->isred = true;
					leftrotate(np->par->par);
				}
			}
		}
		root->isred = false;
	}

	void leftrotate(nodeptr np)
	{
		nodeptr oth = np->right;
		np->right = oth->left;

		if (oth->left != nil)
			oth->left->par = np;

		oth->par = np->par;
		if (np->par == nil)
			root = oth;
		else if (np == np->par->left)
			np->par->left = oth;
		else
			np->par->right = oth;

		oth->left = np;
		np->par = oth;
	}

	void rightrotate(nodeptr np)
	{
		nodeptr oth = np->left;
		np->left = oth->right;

		if (oth->right != nil)
			oth->right->par = np;

		oth->par = np->par;
		if (np->par == nil)
			root = oth;
		else if (np == np->par->left)
			np->par->left = oth;
		else
			np->par->right = oth;

		oth->right = np;
		np->par = oth;
	}

	void transplant(nodeptr x , nodeptr y)
	{
		if (x->par == nil)
			root = y;
		else if (x == x->par->left)
			x->par->left = y;
		else
			x->par->right = y;

		y->par = x->par;
	}

	void delete_fixup(nodeptr x)
	{
		for (; x != root && (x->isred == false);)
		{
			nodeptr bro;
			if (x == x->par->left)
			{
				bro = x->par->right;
				if (bro->isred)
				{
					bro->isred = false;
					x->par->isred = true;
					leftrotate(x->par);
					bro = x->par->right;
				}
				if ((bro->left->isred == false) && (bro->right->isred == false))
				{
					bro->isred = true;
					x = x->par;
				}
				else
				{
					if (bro->right->isred == false)
					{
						bro->left->isred = false;
						bro->isred = true;
						rightrotate(bro);
						bro = x->par->right;
					}
					bro->isred = x->par->isred;
					x->par->isred = false;
					bro->right->isred = false;
					leftrotate(x->par);
					x = root;
				}
			}
			else
			{
				bro = x->par->left;
				if (bro->isred)
				{
					bro->isred = false;
					x->par->isred = true;
					rightrotate(x->par);
					bro = x->par->left;
				}
				if ((bro->right->isred == false) && (bro->left->isred == false))
				{
					bro->isred = true;
					x = x->par;
				}
				else
				{
					if (bro->left->isred == false)
					{
						bro->right->isred = false;
						bro->isred = true;
						leftrotate(bro);
						bro = x->par->left;
					}
					bro->isred = x->par->isred;
					x->par->isred = false;
					bro->left->isred = false;
					rightrotate(x->par);
					x = root;
				}
			}
		}
		x->isred = false;
	}

	nodeptr minimum(nodeptr np)
	{
		for (;;)
		{
			if (np->left != nil)
				np = np->left;
			else
				return np;
		}
	}

	nodeptr maxmum(nodeptr np)
	{
		for (;;)
		{
			if (np->right)
				np = np->right;
			else
				return np;
		}
	}

	void cleartree(nodeptr np)
	{
		if (np == nil)
			return;

		cleartree(np->left);
		cleartree(np->right);
		freenode(np);
	}

	void init()
	{
		nil = &nilnode;
		root = nil;
		_size = 0;
	}


	nodeptr root;
	nodetype nilnode;
	nodeptr nil;
	allocator_type _alloc;
	Compare _compare;
	unsigned _size;
};


#endif //CONTAINERS_REDBLACKTREE_HPP