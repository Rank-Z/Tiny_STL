#ifndef RB_TREE_HPP
#define RB_TREE_HPP
template<typename Key>
class RB_node
{
public:

	RB_node(const Key&key , RB_node* p = nullptr , RB_node* l = nullptr , RB_node* r = nullptr)
		:k(key) , par(p) , left(l) , right(r)
	{   }




	Key k;
	bool isred;
	RB_node* par;
	RB_node* left;
	RB_node* right;
};


template<typename Key>
class RB_Tree
{
public:
	using key_type = Key;
	using nodetype = RB_node<key_type>;
	using nodeptr = nodetype * ;

	RB_Tree& insert(key_type key)
	{
		nodeptr parent = root;
		nodeptr np = new nodetype (key);

		if (root != nullptr)
		{
			for (;;)
			{
				if (key < parent->k)
				{
					if (parent->left)
						parent = parent->left;
					else
						break;
				}
				else
				{
					if (parent->right)
						parent = parent->right;
					else
						break;
				}
			}

			np->par = parent;
			if (key < parent->k)
				parent->left = np;
			else
				parent->right = np;
		}
		else
		{
			root = np;
			np->par = nullptr;
		}
		np->isred = true;
		np->left = np->right = nullptr;
		insert_fixup(np);

		return *this;
	}

	nodeptr find(const key_type& key) const
	{
		nodeptr now = root;
		for (; now;)
		{
			if (key == now->k)
				break;
			else if (key < now->k)
				now = now->left;
			else
				now = now->right;
		}
		return now;
	}

	RB_Tree& deletekey(const key_type& key)
	{
		nodeptr np = find(key);
		if (np)
			erase(np);

		return *this;
	}

	RB_Tree& erase(nodeptr z)
	{
		nodeptr x;
		nodeptr y = z;
		bool y_original_color = y->isred;
		if (z->left == nullptr)
		{
			x = z->right;
			transplant(z , z->right);
		}
		else if (z->right == nullptr)
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
		return *this;
	}

private:

	void insert_fixup(nodeptr np)
	{
		for (; np->par&&np->par->isred;)
		{

			nodeptr parbro;
			if (np->par == np->par->par->left)
			{
				parbro = np->par->par->right;

				if (parbro&&parbro->isred)
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
				if (parbro&&parbro->isred)
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
		}//for end
		root->isred = false;
	}

	void leftrotate(nodeptr np)
	{
		nodeptr oth = np->right;
		np->right = oth->left;

		if (oth->left != nullptr)
			oth->left->par = np;

		oth->par = np->par;
		if (np == root)
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

		if (oth->right != nullptr)
			oth->right->par = np;

		oth->par = np->par;
		if (np == root)
			root = oth;
		else if (np == np->par->left)
			np->par->left = oth;
		else
			np->par->right = oth;

		oth->right = np;
		np->par = oth;
	}

	void transplant(nodeptr& x , nodeptr& y)
	{
		if (x->par == nullptr)
			root = y;
		else if (x == x->par->left)
			x->par->left = y;
		else
			x->par->right = y;

		if (y)
			y->par = x->par;
	}

	void delete_fixup(nodeptr& x)
	{
		for (; x != root && (x == nullptr || x->isred == false);)
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
				if ((bro->left == nullptr || bro->left->isred == false) && (bro->right == nullptr || bro->right->isred == false))
				{
					bro->isred = true;
					x = x->par;
				}
				else
				{
					if (bro->right == nullptr || bro->right->isred == false)
					{
						if (bro->left)
							bro->left->isred = false;
						bro->isred = true;
						rightrotate(bro);
						bro = x->par->right;
					}
					bro->isred = x->par->isred;
					x->par->isred = false;
					if (bro->right)
						bro->right->isred = false;
					leftrotate(x->par);
					break;
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
				if ((bro->right == nullptr || bro->right->isred == false) && (bro->left == nullptr || bro->left->isred == false))
				{
					bro->isred = true;
					x = x->par;
				}
				else
				{
					if ((bro->left == nullptr) || (bro->left->isred == false))
					{
						if (bro->right)
							bro->right->isred = false;
						bro->isred = true;
						leftrotate(bro);
						bro = x->par->left;
					}
					bro->isred = x->par->isred;
					x->par->isred = false;
					if (bro->left)
						bro->left->isred = false;
					rightrotate(x->par);
					break;
				}
			}
		}
		if (x)
			x->isred = false;
	}

	nodeptr minimum(nodeptr np)
	{
		for (;;)
		{
			if (np->left)
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

	nodeptr root = nullptr;
};



#endif // !RB_TREE_HPP
