//
//  Created by bogdan.mytnyk on 12/11/17.
//  Copyright © 2017 bogdan.mytnyk. All rights reserved.
//

#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include <vector>
#include <algorithm>

template <typename T>
class tree_item
{
public:
	using children_container = std::vector<tree_item*>;

	tree_item(const T& value, std::initializer_list<tree_item*> in_list = std::initializer_list<tree_item*>());
	tree_item(const tree_item& item);
	tree_item& operator=(const tree_item& item);
	tree_item(tree_item&& item) noexcept;
	tree_item& operator=(tree_item&& item) noexcept;
	~tree_item() noexcept;

	inline const T& value() const { return m_value; }
	inline T& value() { return m_value; }

	inline const children_container& children() const { return m_children; }
	children_container& children() { return m_children; };

	void add_child(tree_item* child) { m_children.push_back(child); }

	// TODO : check edge cases
	void remove_child(size_t index) { m_children.erase(m_children.begin() + index); }

	tree_item* get_child(size_t index);
	const tree_item* get_child(size_t index) const;

	inline size_t count() const { return m_children.size(); };

private:
	void copy_children(const tree_item& item);
	void clear_children() noexcept;

private:
	T m_value;
	children_container m_children;
};

template <typename T>
tree_item<T>::tree_item(const T& value, std::initializer_list<tree_item<T>*> children) :
	m_value(value),
	m_children(children)
{
}

template <typename T>
tree_item<T>::tree_item(const tree_item<T>& item) :
	m_value(item.m_value),
	m_children()
{
	copy_children(item);
}

template <typename T>
tree_item<T>::tree_item(tree_item<T>&& item) noexcept:
	m_value(std::move(item.m_value)),
	m_children(std::move(item.m_children))
{
}

template <typename T>
tree_item<T>& tree_item<T>::operator=(const tree_item<T>& item)
{
	if (this != &item)
	{
		m_value = item.m_value;
		copy_children(item);
	}

	return *this;
}

template <typename T>
tree_item<T>& tree_item<T>::operator=(tree_item<T>&& item) noexcept
{
	m_value = std::move(item.m_value);
	m_children = std::move(item.m_children);

	return *this;
}

template <typename T>
tree_item<T>::~tree_item() noexcept
{
	clear_children();
}

template <typename T>
void tree_item<T>::copy_children(const tree_item<T>& item)
{
	const auto& children = item.children();
	m_children.reserve(children.size());
	std::for_each(children.begin(), children.end(), [this](tree_item* item)
		{
			m_children.push_back(new tree_item(*item));
		});
}

template <typename T>
void tree_item<T>::clear_children() noexcept
{
	for (tree_item* item : m_children)
		delete item;
	m_children.clear();
}

template <typename T>
tree_item<T>* tree_item<T>::get_child(size_t index)
{
	assert(index < m_children.size());
	return m_children[index];
}

template <typename T>
const tree_item<T>* tree_item<T>::get_child(size_t index) const
{
	assert(index < m_children.size());
	return m_children[index];
}

#endif // TREE_ITEM_H
