#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
template<class _keyType,
	class _valueType,
	class _compare = std::less<_keyType>,
	class _equal = std::equal_to<_keyType>>
	class AVL
{
private:
	class _node
	{
	public:
		_keyType key;
		_valueType value;
		_node* left, * right, * parent;
		size_t height;
		_node(_keyType key = 0, _valueType value = 0);
		~_node();
	};
	_node* root;

	void rebalancingInsert(_node*);
	void rotate_left(_node*);
	void rotate_right(_node*);
	void transplant(_node*, _node*);
	_node* _nodeDelete(_node*);
	void rebalancingDelete(_node*);
	void printLayers();
	void printPreorder(_node*);
	void printInorder(_node*);
	void printPostorder(_node*);
	void print_node(_node*);

	void doubleRotateLeft_Right(_node*);
	void doubleRotateRight_Left(_node*);
public:
	AVL(std::vector<std::pair<_keyType, _valueType>> arr = std::vector<std::pair<_keyType, _valueType>>());
	~AVL();

	_node* find(_keyType);
	void insert(_keyType, _valueType);
	void printTree(const short&);
	void construct(const std::vector<std::pair<_keyType, _valueType>>&);
	_node* min(_node*);
	_node* max(_node*);
	void erase(const _keyType&);
	void clear();
	_node* successor(_node*);
	_node* predecessor(_node*);
	_node* getRoot();

	_valueType& operator[](const _keyType);
};

template<class _keyType, class _valueType, class _compare, class _equal>
inline AVL<_keyType, _valueType, _compare, _equal>::_node::_node(_keyType key, _valueType value)
{
	this->key = key;
	this->value = value;

	this->left = this->right = this->parent = nullptr;
	this->height = 0;
}

template<class _keyType, class _valueType, class _compare, class _equal>
inline AVL<_keyType, _valueType, _compare, _equal>::_node::~_node()
{
	if (this->left)
		delete this->left;

	if (this->right)
		delete this->right;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::rebalancingInsert(_node* point)
{
	point = point->parent;
	while (point)
	{
		short factor = ((!point->right) ? 0 : point->right->height) -
			((!point->left) ? 0 : point->left->height);

		if (factor == 0)
		{
			return;
		}
		else if (factor == 1 || factor == -1)
		{
			++point->height;
			point = point->parent;
		}
		else
		{
			if (factor == -2)
			{
				_node* left = point->left;
				short left_factor = ((!left->right) ? 0 : left->right->height) -
					((!left->left) ? 0 : left->left->height);
				if (left_factor == -1)
				{
					--point->height;
					rotate_right(point);
					return;
				}
				else
				{
					--point->left->height;
					--point->height;
					++point->left->right->height;
					doubleRotateLeft_Right(point);
					return;
				}
			}
			else
			{
				_node* right = point->right;
				short right_factor = ((!right->right) ? 0 : right->right->height) -
					((!right->left) ? 0 : right->left->height);
				if (right_factor == 1)
				{
					--point->height;
					rotate_left(point);
					return;
				}
				else
				{
					--point->right->height;
					--point->height;
					++point->right->left->height;
					doubleRotateRight_Left(point);
					return;
				}
			}
		}
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
inline void AVL<_keyType, _valueType, _compare, _equal>::doubleRotateLeft_Right(_node* point)
{
	_node* leftright = point->left->right; // salvez stdr
	_node* left = point->left; // salvez st
	if (leftright->left)
	{
		left->right = leftright->left;
		left->right->parent = left;    //mut pe 2 la st
	}
	else
		left->right = nullptr;
	leftright->parent = point->parent; // pun pe stdr ca root
	if (!point->parent)
		this->root = leftright;
	else
	{
		if (point == point->parent->left)
			point->parent->left = leftright;
		else
			point->parent->right = leftright;  // leg pe stdr de parinte
	}
	if (leftright->right)
	{
		point->left = leftright->right;
		point->left->parent = point; // leg pe 3 la x
	}
	else
		point->left = nullptr;
	leftright->left = left;
	left->parent = leftright; // fac legatura st - stdr

	leftright->right = point;
	point->parent = leftright; // fac legatura x - stdr
}

template<class _keyType, class _valueType, class _compare, class _equal>
inline void AVL<_keyType, _valueType, _compare, _equal>::doubleRotateRight_Left(_node* point)
{
	_node* rightleft = point->right->left;
	_node* right = point->right;
	if (rightleft->right)
	{
		right->left = rightleft->right;
		right->left->parent = right;
	}
	else
		right->left = nullptr;
	rightleft->parent = point->parent;
	if (!point->parent)
		this->root = rightleft;
	else
	{
		if (point == point->parent->left)
			point->parent->left = rightleft;
		else
			point->parent->right = rightleft;
	}
	if (rightleft->left)
	{
		point->right = rightleft->left;
		point->right->parent = point;
	}
	else point->right = nullptr;
	rightleft->right = right;
	right->parent = rightleft;

	rightleft->left = point;
	point->parent = rightleft;
}
template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::rotate_left(_node* point)
{
	_node* right = point->right;
	point->right = right->left;
	if (right->left)
		right->left->parent = point;
	right->parent = point->parent;
	if (!point->parent)
		this->root = right;
	else
	{
		if (point == point->parent->left)
			point->parent->left = right;
		else
			point->parent->right = right;
	}
	right->left = point;
	point->parent = right;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::rotate_right(_node* point)
{
	_node* left = point->left;
	point->left = left->right;
	if (left->right)
		left->right->parent = point;
	left->parent = point->parent;
	if (!point->parent)
		this->root = left;
	else
	{
		if (point == point->parent->left)
			point->parent->left = left;
		else
			point->parent->right = left;
	}
	left->right = point;
	point->parent = left;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::transplant(_node* u, _node* v)
{
	if (!u->parent)
		this->root = v;
	else
	{
		if (u == u->parent->left)
			u->parent->left = v;
		else
			u->parent->right = v;
	}
	if (v)
		v->parent = u->parent;
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename AVL<_keyType, _valueType, _compare, _equal>::_node* AVL<_keyType, _valueType, _compare, _equal>::successor(_node* p)
{
	_node* y;
	if (p->right)
		y = min(p->right);
	else
	{
		y = p->parent;
		while (y && p == y->right)
		{
			p = y;
			y = y->parent;
		}
	}
	return y;
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename AVL<_keyType, _valueType, _compare, _equal>::_node* AVL<_keyType, _valueType, _compare, _equal>::_nodeDelete(_node* point)
{
	if (!point)
		return nullptr;
	if (!point->left)
	{
		transplant(point, point->right);

		_node* aux = point->parent;

		point->left = point->right = nullptr;
		delete point;
		return aux;

	}
	else
	{
		if (!point->right)
		{
			transplant(point, point->left);

			_node* aux = point->parent;
			point->left = point->right = nullptr;
			delete point;
			return aux;
		}
		else
		{

			_node* y = successor(point);
			_node* rebalancing_node;
			if (y->parent == point)
				rebalancing_node = y;
			else
				rebalancing_node = y->parent;

			y->height = point->height;

			_node* x = y->right;

			if (x && y->parent == point)
				x->parent = y;
			else
			{
				transplant(y, y->right);
				if (y->right)
				{
					y->right = point->right;
					y->right->parent = y;
				}
			}

			transplant(point, y);
			y->left = point->left;
			y->left->parent = y;

			point->left = point->right = nullptr;
			delete point;

			return rebalancing_node;
		}
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::rebalancingDelete(_node* point)
{
	while (point)
	{
		short factor = ((!point->right) ? 0 : point->right->height) -
			((!point->left) ? 0 : point->left->height);

		if (factor == 1 || factor == -1)
			return;
		else if (factor == 0)
		{
			--point->height;
			point = point->parent;
			continue;
		}
		else
		{
			if (factor == -2)
			{
				short left_factor = ((!point->left->right) ? 0 : point->left->right->height) -
					((!point->left->left) ? 0 : point->left->left->height);
				if (left_factor == 0)
				{
					--point->height;
					++point->left->height;
					rotate_right(point);
					return;
				}
				else if (left_factor == -1)
				{
					point->height -= 2;
					rotate_right(point);
					point = point->parent->parent;
					continue;
				}
				else if (left_factor == 1)
				{
					--point->left->height;
					point->height -= 2;
					++point->left->right->height;

					doubleRotateLeft_Right(point);

					point = point->parent->parent;
				}
			}
			else
			{
				short right_factor = ((!point->right->right) ? 0 : point->right->right->height) -
					((!point->right->left) ? 0 : point->right->left->height);
				if (right_factor == 0)
				{
					++point->right->height;
					--point->height;
					rotate_left(point);
					return;
				}
				else if (right_factor == 1)
				{
					point->height -= 2;
					rotate_left(point);
					point = point->parent->parent;
					continue;
				}
				else if (right_factor == -1)
				{
					--point->right->height;
					point->height -= 2;
					++point->right->left->height;

					doubleRotateRight_Left(point);

					point = point->parent->parent;
				}
			}
		}
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::printLayers()
{
	std::queue<_node*> queue;

	queue.push(this->root);
	int this_layer_cnt = 1;
	int next_layer_cnt = 0;
	while (!queue.empty())
	{
		if (this_layer_cnt == 0)
		{
			std::cout << '\n';
			this_layer_cnt = next_layer_cnt;
			next_layer_cnt = 0;
		}

		print_node(queue.front());
		this_layer_cnt--;

		if (queue.front()->left)
			++next_layer_cnt, queue.push(queue.front()->left);
		if (queue.front()->right)
			++next_layer_cnt, queue.push(queue.front()->right);

		queue.pop();
	}
	std::cout << '\n';
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::printPreorder(_node* point)
{
	if (point)
	{
		print_node(point);
		printPreorder(point->left);
		printPreorder(point->right);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::printInorder(_node* point)
{
	if (point)
	{
		printInorder(point->left);
		print_node(point);
		printInorder(point->right);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::printPostorder(_node* point)
{
	if (point)
	{
		printPostorder(point->left);
		printPostorder(point->right);
		print_node(point);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::print_node(_node* point)
{
	short factor = ((!point->right) ? 0 : point->right->height) -
		((!point->left) ? 0 : point->left->height);
	std::cout << point->key << "," << point->value << "(" << factor << ") ";
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename AVL<_keyType, _valueType, _compare, _equal>::_node* AVL<_keyType, _valueType, _compare, _equal>::predecessor(_node* root)
{
	_node* y;
	if (root->right)
		y = min(root->right);
	else
	{
		y = root->parent;
		while (y && root == y->left)
		{
			root = y;
			y = y->parent;
		}
	}
	return y;

}

template<class _keyType, class _valueType, class _compare, class _equal>
inline typename AVL<_keyType, _valueType, _compare, _equal>::_node* AVL<_keyType, _valueType, _compare, _equal>::getRoot()
{
	return this->root;
}

template<class _keyType, class _valueType, class _compare, class _equal>
inline _valueType& AVL<_keyType, _valueType, _compare, _equal>::operator[](const _keyType key)
{
	insert(key, 0);
	return find(key)->value;
}

template<class _keyType, class _valueType, class _compare, class _equal>
AVL<_keyType, _valueType, _compare, _equal>::AVL(std::vector<std::pair<_keyType, _valueType>> arr)
{
	this->root = nullptr;
	if (arr.size())
	{
		construct(arr);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
inline AVL<_keyType, _valueType, _compare, _equal>::~AVL()
{
	delete this->root;
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename AVL<_keyType, _valueType, _compare, _equal>::_node* AVL<_keyType, _valueType, _compare, _equal>::find(_keyType key)
{
	_node* p = this->root;
	while (p && !_equal()(p->key, key))
	{
		if (_compare()(key, p->key))
			p = p->left;
		else
			p = p->right;
	}
	return p;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::insert(_keyType key, _valueType value)
{
	if (!find(key))
	{
		_node* current_point = this->root;
		_node* current_parent = nullptr;
		while (current_point)
		{
			current_parent = current_point;
			if (_compare()(key, current_point->key))
				current_point = current_point->left;
			else
				current_point = current_point->right;
		}
		_node* point = new _node(key, value);
		point->height = 1;
		point->parent = current_parent;

		if (!current_parent)
		{
			this->root = point;
		}
		else
		{
			if (_compare()(point->key, current_parent->key))
				current_parent->left = point;
			else
				current_parent->right = point;
		}
		rebalancingInsert(point);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::printTree(const short& a)
{
	switch (a)
	{
	case 1:
		printPreorder(this->root);
		std::cout << '\n';
		break;
	case 2:
		printInorder(this->root);
		std::cout << '\n';
		break;
	case 3:
		printPostorder(this->root);
		std::cout << '\n';
		break;
	case 4:
		printLayers();
		break;
	default:
		return;
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::construct(const std::vector<std::pair<_keyType, _valueType>>& aux)
{
	for (auto& it : aux)
		insert(it.first, it.second);
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename AVL<_keyType, _valueType, _compare, _equal>::_node* AVL<_keyType, _valueType, _compare, _equal>::min(_node* p)
{
	_node* aux = p;
	while (p)
	{
		aux = p;
		p = p->left;
	}
	return aux;
}

template<class _keyType, class _valueType, class _compare, class _equal>
typename AVL<_keyType, _valueType, _compare, _equal>::_node* AVL<_keyType, _valueType, _compare, _equal>::max(_node* p)
{
	_node* aux = p;
	while (p)
	{
		aux = p;
		p = p->right;
	}
	return aux;
}

template<class _keyType, class _valueType, class _compare, class _equal>
void AVL<_keyType, _valueType, _compare, _equal>::erase(const _keyType& key)
{
	_node* aux = find(key);
	if (!aux)
		std::cout << "Could not find the specified value.\n";
	else
	{
		_node* help = _nodeDelete(aux);
		rebalancingDelete(help);
	}
}

template<class _keyType, class _valueType, class _compare, class _equal>
inline void AVL<_keyType, _valueType, _compare, _equal>::clear()
{
	if (this->root)
		delete this->root;
}
