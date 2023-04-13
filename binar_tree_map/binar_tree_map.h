#pragma once
#ifndef _TREE_
#define _TREE_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

//�������� ������ ��� ���� { ���� + ������ } � ����������� � ������� �� �����
template<class K, class D>
class tree
{
private:
	template<class K, class D>
	class Node
	{
	public:
		pair<K, D> cell; //������ ���� { ����, ������ }
		Node<K, D>* left;
		Node<K, D>* right;
		size_t height;
		Node(pair<K, D> cell = { K(), D() }, Node<K, D>* left = nullptr, Node<K, D>* right = nullptr)
		{
			this->cell = cell;
			this->left = left;
			this->right = right;
			this->height = 1;
		}
	};

	Node<K, D>* root;

	size_t Size;

	//������� �������� ������ �����
	size_t height(Node<K, D>* branch)
	{
		return branch ? branch->height : 0;
	}

	//���������� �������� ����� �������� ������������ �����
	int balance_factor(Node<K, D>* branch)
	{
		return height(branch->right) - height(branch->left);
	}

	//������� ��������� ������ �����
	void fix_height(Node<K, D>* branch)
	{
		size_t left_height = height(branch->left);
		size_t right_height = height(branch->right);
		branch->height = (left_height > right_height ? left_height : right_height) + 1;
	}

	//������ ������� ������ ���������� �����
	Node<K, D>* turn_right(Node<K, D>* branch)
	{
		Node<K, D>* new_root = branch->left;
		branch->left = new_root->right;
		new_root->right = branch;
		fix_height(branch);
		fix_height(new_root);
		return new_root;
	}

	//����� ������� ������ ���������� �����
	Node<K, D>* turn_left(Node<K, D>* branch)
	{
		Node<K, D>* new_root = branch->right;
		branch->right = new_root->left;
		new_root->left = branch;
		fix_height(branch);
		fix_height(new_root);
		return new_root;
	}

	//������������ ������������ ����� (��������� �������)
	Node<K, D>* balance_branch(Node<K, D>* branch)
	{
		fix_height(branch);
		if (balance_factor(branch) == 2)
		{
			if (balance_factor(branch->right) < 0) { branch->right = turn_right(branch->right); }
			return turn_left(branch);
		}
		if (balance_factor(branch) == -2)
		{
			if (balance_factor(branch->left) > 0) { branch->left = turn_left(branch->left); }
			return turn_right(branch);
		}
		return branch; //������������ �� �����
	}

	//������� �������� � �������������
	Node<K, D>* insert(Node<K, D>* branch, K key, D data)
	{
		if (!branch) { this->Size++; return new Node<K, D>(pair<K, D>(key, data)); }
		if (key == branch->cell.first) { return branch; }
		if (key < branch->cell.first) { branch->left = insert(branch->left, key, data); }
		else { branch->right = insert(branch->right, key, data); }
		return balance_branch(branch);
	}

	//�������� ����� ������ (���������� ��������� ����, ������� � �������� ������� �����; ��������� �������, ����������� ��� ������)
	void clear_branch(Node<K, D>* branch)
	{
		if (branch->left != nullptr)
		{
			clear_branch(branch->left);
		}
		if (branch->right != nullptr)
		{
			clear_branch(branch->right);
		}
		delete branch;
		this->Size--;
	}

	//��������� ������� ��� ������ ����� ������� � ������
	void show_elements(Node<K, D>* branch, size_t level)
	{
		std::string str;
		if (branch->right != nullptr) { show_elements(branch->right, ++level); }
		for (int i = 0; i < level; i++) { str += " "; }
		cout << str << branch->cell.first << "," << branch->cell.second << endl;
		if (branch->left != nullptr) { show_elements(branch->left, ++level); }
	}

	//��������� ������� ������������ ������ � ������������ �������, ������� � �������� �����
	vector<pair<K, D>> equivalent_vector(Node<K, D>* branch)
	{
		vector<pair<K, D>> vct, vctL, vctR;
		if (branch->left != nullptr) { vctL = equivalent_vector(branch->left); }   //������������ ����� �����, ���� ��� ����
		if (branch->right != nullptr) { vctR = equivalent_vector(branch->right); } //������������ ������ �����, ���� ��� ����
		if (!vctL.empty()) { vct = vctL; }
		vct.push_back(branch->cell);				//����������� � ����:
		if (!vctR.empty())							//"������ ����� ����� -> ������ ������� ���� -> ������ ������ �����"
		{
			for (int i = 0; i < vctR.size(); i++)
			{
				vct.push_back(vctR[i]);
			}
		}
		return vct;
	}

	//���������� ���� � ����������� ������ � ���������� ����� (���������)
	Node<K, D>* find_min(Node<K, D>* branch)
	{
		if (branch == nullptr) { return nullptr; }
		if (branch->left == nullptr) { return branch; }
		else { find_min(branch->left); }
	}

	//�������� ���� � ����������� ������ � �������� ����� (���������)
	Node<K, D>* remove_min(Node<K, D>* branch)
	{
		if (branch == nullptr) { return nullptr; }
		if (branch->left == nullptr) { return branch->right; }
		branch->left = remove_min(branch->left);
		return balance_branch(branch); //�������� ������������
	}

	//��������� ������� �������� �������� �� ����� � ���������� �����
	Node<K, D>* remove_at_branch(Node<K, D>* branch, K key)
	{
		if (key < branch->cell.first) { branch->left = remove_at_branch(branch->left, key); }
		else
		{
			if (key > branch->cell.first) { branch->right = remove_at_branch(branch->right, key); }
			else
			{
				Node<K, D>* temp = branch;
				while (temp != nullptr)
				{
					if (key == temp->cell.first) { break; }
					if (key > temp->cell.first) { temp = temp->right; }
					else { temp = temp->left; }
				}
				Node<K, D>* left_block = temp->left;
				Node<K, D>* right_block = temp->right;
				delete temp;
				if (left_block == nullptr && right_block == nullptr) { return nullptr; }
				Node<K, D>* minimum = find_min(right_block);
				minimum->right = remove_min(right_block);
				minimum->left = left_block;
				return balance_branch(minimum); //�������� ������������
			}
		}
		return balance_branch(branch);
	}

public:

	//����������� ������� ������
	tree();

	//�������� ������� � ������������� ������ �������
	vector<pair<K, D>> equivalent_vector();

	//���������� ����� ������
	size_t size();

	//���������� �������� (���� + ������)
	void push(K key, D data);

	//���������� �������� ����� ������� ��������� ����
	void push(pair<K, D> cell);

	//�������� �������� �� �����, ���� �� ����������
	void erase(K key);

	//����� �������� �� �����
	bool find(K key);

	//����� ��������� ������ � ������� ����������� �������� �����
	void show_data();

	//�������� ������
	void clear();

	//��������� ������ �� ������ �� �����
	D& get_at(K key);

	//����������
	~tree();

};

//����������� ������� ������
template<class K, class D>
inline tree<K, D>::tree()
{
	this->root = nullptr;
	this->Size = 0;
}

//������ � ������������� �������
template<class K, class D>
inline vector<pair<K, D>> tree<K, D>::equivalent_vector()
{
	return equivalent_vector(this->root);
}

//���������� ����� ������
template<class K, class D>
inline size_t tree<K, D>::size()
{
	return this->Size;
}

//���������� �������� ����� ������� ��������� ����
template<class K, class D>
inline void tree<K, D>::push(K key, D data)
{
	this->root = insert(this->root, key, data);
}

//���������� �������� ����� ������� ��������� ����
template<class K, class D>
inline void tree<K, D>::push(pair<K, D> cell)
{
	this->root = insert(this->root, cell.first, cell.second);
}

//�������� �������� �� �����, ���� �� ����������
template<class K, class D>
inline void tree<K, D>::erase(K key)
{
	if (find(key)) { this->root = remove_at_branch(this->root, key); this->Size--; }
	else { throw exception("Element is not found"); }
}

//����� �������� �� �����
template<class K, class D>
inline bool tree<K, D>::find(K key)
{
	if (this->Size != 0)
	{
		Node<K, D>* temp = this->root;
		while (true)
		{
			if (key == temp->cell.first) { return true; }
			if (key > temp->cell.first)
			{
				if (temp->right != nullptr) { temp = temp->right; }
				else { return false; }
			}
			else
			{
				if (temp->left != nullptr) { temp = temp->left; }
				else { return false; }
			}
		}
	}
	else { return false; }
}

//����� ��������� ������ � ������� ����������� �������� �����
template<class K, class D>
inline void tree<K, D>::show_data()
{
	cout << "Elements of the tree:\n";
	show_elements(this->root, 0);
}

//�������� ������
template<class K, class D>
inline void tree<K, D>::clear()
{
	clear_branch(this->root);
}

//��������� ������ �� ������ �� �����
template<class K, class D>
inline D& tree<K, D>::get_at(K key)
{
	if (find(key))
	{
		Node<K, D>* temp = this->root;
		while (temp != nullptr)
		{
			if (key == temp->cell.first) { return temp->cell.second; }
			if (key > temp->cell.first) { temp = temp->right; }
			else { temp = temp->left; }
		}
	}
	else { throw exception("Invalid key"); }
}

//����������
template<class K, class D>
inline tree<K, D>::~tree()
{
	clear();
}

#endif // !_TREE_