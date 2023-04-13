#pragma once
#ifndef _TREE_
#define _TREE_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Бинарное дерево пар вида { ключ + данные } с сортировкой и поиском по ключу
template<class K, class D>
class tree
{
private:
	template<class K, class D>
	class Node
	{
	public:
		pair<K, D> cell; //Ячейка вида { ключ, данные }
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

	//Функция возврата высоты ветки
	size_t height(Node<K, D>* branch)
	{
		return branch ? branch->height : 0;
	}

	//Вычисление разности высот подветок передаваемой ветки
	int balance_factor(Node<K, D>* branch)
	{
		return height(branch->right) - height(branch->left);
	}

	//Функция пересчёта высоты ветви
	void fix_height(Node<K, D>* branch)
	{
		size_t left_height = height(branch->left);
		size_t right_height = height(branch->right);
		branch->height = (left_height > right_height ? left_height : right_height) + 1;
	}

	//Правый поворот вокруг переданной ветки
	Node<K, D>* turn_right(Node<K, D>* branch)
	{
		Node<K, D>* new_root = branch->left;
		branch->left = new_root->right;
		new_root->right = branch;
		fix_height(branch);
		fix_height(new_root);
		return new_root;
	}

	//Левый поворот вокруг переданной ветки
	Node<K, D>* turn_left(Node<K, D>* branch)
	{
		Node<K, D>* new_root = branch->right;
		branch->right = new_root->left;
		new_root->left = branch;
		fix_height(branch);
		fix_height(new_root);
		return new_root;
	}

	//Балансировка передаваемой ветви (служебная функция)
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
		return branch; //Балансировка не нужна
	}

	//Вставка элемента с балансировкой
	Node<K, D>* insert(Node<K, D>* branch, K key, D data)
	{
		if (!branch) { this->Size++; return new Node<K, D>(pair<K, D>(key, data)); }
		if (key == branch->cell.first) { return branch; }
		if (key < branch->cell.first) { branch->left = insert(branch->left, key, data); }
		else { branch->right = insert(branch->right, key, data); }
		return balance_branch(branch);
	}

	//Удаление ветви дерева (параметром передаётся узел, начиная с которого срубаем ветвь; служебная функция, недоступная вне класса)
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

	//Служебная функция для вывода ветви начиная с данной
	void show_elements(Node<K, D>* branch, size_t level)
	{
		std::string str;
		if (branch->right != nullptr) { show_elements(branch->right, ++level); }
		for (int i = 0; i < level; i++) { str += " "; }
		cout << str << branch->cell.first << "," << branch->cell.second << endl;
		if (branch->left != nullptr) { show_elements(branch->left, ++level); }
	}

	//Служебная функция возвращающая вектор с аналогичными данными, начиная с заданной ветви
	vector<pair<K, D>> equivalent_vector(Node<K, D>* branch)
	{
		vector<pair<K, D>> vct, vctL, vctR;
		if (branch->left != nullptr) { vctL = equivalent_vector(branch->left); }   //Конвертирует левую ветвь, если она есть
		if (branch->right != nullptr) { vctR = equivalent_vector(branch->right); } //Конвертирует правую ветвь, если она есть
		if (!vctL.empty()) { vct = vctL; }
		vct.push_back(branch->cell);				//Объединение в виде:
		if (!vctR.empty())							//"данные левой ветви -> данные данного узла -> данные правой ветви"
		{
			for (int i = 0; i < vctR.size(); i++)
			{
				vct.push_back(vctR[i]);
			}
		}
		return vct;
	}

	//возвращает узел с минимальным ключом в переданной ветви (служебная)
	Node<K, D>* find_min(Node<K, D>* branch)
	{
		if (branch == nullptr) { return nullptr; }
		if (branch->left == nullptr) { return branch; }
		else { find_min(branch->left); }
	}

	//Удаление узла с минимальным ключом в заданной ветви (служебная)
	Node<K, D>* remove_min(Node<K, D>* branch)
	{
		if (branch == nullptr) { return nullptr; }
		if (branch->left == nullptr) { return branch->right; }
		branch->left = remove_min(branch->left);
		return balance_branch(branch); //Добавить балансировку
	}

	//Служебная функция удаления элемента по ключу в переданной ветке
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
				return balance_branch(minimum); //добавить балансировку
			}
		}
		return balance_branch(branch);
	}

public:

	//Конструктор пустого дерева
	tree();

	//Создание вектора с эквивалентыми дереву данными
	vector<pair<K, D>> equivalent_vector();

	//Количество узлов дерева
	size_t size();

	//Добавление элемента (ключ + данные)
	void push(K key, D data);

	//Добавление элемента через заранее созданную пару
	void push(pair<K, D> cell);

	//Удаление элемента по ключу, если он существует
	void erase(K key);

	//Поиск элемента по ключу
	bool find(K key);

	//Вывод элементов дерева в порядке возрастания значения ключа
	void show_data();

	//Удаление дерева
	void clear();

	//Получение данных из ячейки по ключу
	D& get_at(K key);

	//Деструктор
	~tree();

};

//Конструктор пустого дерева
template<class K, class D>
inline tree<K, D>::tree()
{
	this->root = nullptr;
	this->Size = 0;
}

//Вектор с эквивалентыми данными
template<class K, class D>
inline vector<pair<K, D>> tree<K, D>::equivalent_vector()
{
	return equivalent_vector(this->root);
}

//Количество узлов дерева
template<class K, class D>
inline size_t tree<K, D>::size()
{
	return this->Size;
}

//Добавление элемента через заранее созданную пару
template<class K, class D>
inline void tree<K, D>::push(K key, D data)
{
	this->root = insert(this->root, key, data);
}

//Добавление элемента через заранее созданную пару
template<class K, class D>
inline void tree<K, D>::push(pair<K, D> cell)
{
	this->root = insert(this->root, cell.first, cell.second);
}

//Удаление элемента по ключу, если он существует
template<class K, class D>
inline void tree<K, D>::erase(K key)
{
	if (find(key)) { this->root = remove_at_branch(this->root, key); this->Size--; }
	else { throw exception("Element is not found"); }
}

//Поиск элемента по ключу
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

//Вывод элементов дерева в порядке возрастания значения ключа
template<class K, class D>
inline void tree<K, D>::show_data()
{
	cout << "Elements of the tree:\n";
	show_elements(this->root, 0);
}

//Удаление дерева
template<class K, class D>
inline void tree<K, D>::clear()
{
	clear_branch(this->root);
}

//Получение данных из ячейки по ключу
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

//Деструктор
template<class K, class D>
inline tree<K, D>::~tree()
{
	clear();
}

#endif // !_TREE_