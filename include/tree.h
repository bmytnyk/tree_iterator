//
//  Created by bogdan.mytnyk on 12/11/17.
//  Copyright © 2017 bogdan.mytnyk. All rights reserved.
//

#ifndef TREE_H
#define TREE_H

#include <queue>
#include <stack>
#include <iterator>
#include <cassert>
#include <stdexcept>

#include "tree_item.h"
#include "optimized_ptr_stack.h"

template <typename T, typename Container>
struct top_helper
{
};

template <typename T>
struct top_helper<T, std::queue<T>>
{
    inline static void push_next_items(T current, std::queue<T>& next_items)
    {
        for (size_t i = 0, count = current->count(); i < count; ++i)
            next_items.push(current->get_child(i));
    }
    
    inline static const T& get(std::queue<T>& c)
    {
        return c.front();
    }
};

template <typename T>
struct top_helper<T, std::stack<T>>
{
    inline static void push_next_items(T current, std::stack<T>& next_items)
    {
        for (size_t i = 0, count = current->count(); i < count; ++i)
            next_items.push(current->get_child(count - i - 1));
    }
    
    inline static const T& get(std::stack<T>& c)
    {
        return c.top();
    }
};

template <typename T>
struct top_helper<T, optimized_ptr_stack<T>>
{
    inline static void push_next_items(T current, optimized_ptr_stack<T>& next_items)
    {
        for (size_t i = 0, count = current->count(); i < count; ++i)
            next_items.push(current->get_child(count - i - 1));
    }
    
    inline static const T& get(optimized_ptr_stack<T>& c)
    {
        return c.top();
    }
};

template <typename T>
class tree
{
public:
    typedef tree_item<T> item_type;

    tree(const T& value) :
        m_top(new tree_item<T>(value))
    {
    }
    
    tree(item_type* top_item = nullptr) :
        m_top(top_item)
    {
    }

	tree(tree&& rht) :
		m_top(rht.m_top)
	{
		rht.m_top = nullptr;
	}
    
    ~tree()
    {
        if (m_top)
            delete m_top;
    }
    
    inline tree_item<T>* top() { return m_top; };
    inline const tree_item<T>* top() const { return m_top; };
    
    void add_child(tree_item<T>* pChild) { m_top->add_child(pChild); }
    
    void remove_child(size_t index) { m_top->remove_child(index);};
    size_t count() const { return m_top->count(); }
    
    template <typename ContainerType>
    struct const_base_iterator : public std::iterator<std::forward_iterator_tag, T>
    {
    public:
        const_base_iterator(item_type* cur = nullptr) :
            m_current(cur)
        {
        }
        
        const_base_iterator(const const_base_iterator& iter) :
            m_nextItems(iter.m_nextItems),
            m_current(iter.m_current)
        {
        }
        
        const_base_iterator& operator=(const const_base_iterator& iter)
        {
            if (this != &iter)
            {
                m_nextItems = iter.m_nextItems;
                m_current = iter.m_current;
            };
            
            return *this;
        }
        
        inline const_base_iterator& operator++()
        {
            if (m_current != nullptr)
            {
                top_helper<item_type*, ContainerType>::push_next_items(m_current, m_nextItems);
                
                if (!m_nextItems.empty())
                {
                    m_current = top_helper<item_type*, ContainerType>::get(m_nextItems);
                    m_nextItems.pop();
                }
                else
                {
                    m_current = nullptr;
                }
            }
			else
			{
				throw std::runtime_error("Increment on invalid iterator");
			}
            
            return *this;
        };
        
        const_base_iterator operator++(int)
        {
            const_base_iterator prev(*this);
            ++*this;
            return prev;
        };
        
        const T& operator*() const
        {
            assert(m_current && "Dereferencing invalid iterator");
            
            return m_current->value();
        }
        
        const T* operator->() const
        {
			assert(m_current && "Dereferencing invalid iterator");
            
            return &m_current->value();
        }
        
        inline bool operator == (const const_base_iterator& iter) const
        {
            return (m_current == iter.m_current);
        }
        
		inline bool operator != (const const_base_iterator& iter) const
        {
            return (m_current != iter.m_current);
        }
        
    protected:
        ContainerType m_nextItems;
        item_type* m_current;
    };
    
    typedef const_base_iterator<std::stack<item_type*>> const_dfs_iterator;
    typedef const_base_iterator<std::queue<item_type*>> const_bfs_iterator;
    
    template <typename ContainerType>
	struct base_iterator : const_base_iterator<ContainerType>
    {
    public:
        using base_type = const_base_iterator<ContainerType>;
        using base_type::m_current;
        using base_type::m_nextItems;
        
        base_iterator(item_type* cur = nullptr):
            base_type(cur)
        {
        }
        
        base_iterator(const base_iterator& iter) :
            base_type(iter)
        {
        }
        
        base_iterator& operator=(const base_iterator& iter)
        {
            base_type::operator=(iter);
            return *this;
        }
        
        inline T& operator*()
        {
            assert(m_current && "Dereferencing invalid iterator");
            
            return m_current->value();
        }
        
        T* operator->()
        {
            assert(m_current && "Dereferencing invalid iterator");
            
            return &m_current->value();
        }
        
        // Add increments
        base_iterator& operator++()
        {
            if (m_current != nullptr)
            {
                top_helper<item_type*, ContainerType>::push_next_items(m_current, m_nextItems);
                
                if (!m_nextItems.empty())
                {
                    m_current = top_helper<item_type*, ContainerType>::get(m_nextItems);
                    m_nextItems.pop();
                }
                else
                {
                    m_current = nullptr;
                }
            }
            
            return *this;
        };
        
        inline bool operator == (const base_iterator& iter) const
        {
            return (m_current == iter.m_current);
        }
        
        inline bool operator != (const base_iterator& iter) const
        {
            return (m_current != iter.m_current);
        }
    };
    
    typedef base_iterator<optimized_ptr_stack<item_type*>> dfs_iterator;
    //typedef base_iterator<std::stack<item_type*>> dfs_iterator;
    typedef base_iterator<std::queue<item_type*>> bfs_iterator;
    
    const_bfs_iterator bfs_cbegin() const
    {
        return const_bfs_iterator(m_top);
    }
    
    const_bfs_iterator bfs_cend() const
    {
        return const_bfs_iterator(nullptr);
    }
    
    const_dfs_iterator dfs_cbegin() const
    {
        return const_dfs_iterator(m_top);
    }
    
    const_dfs_iterator dfs_cend() const
    {
        return const_dfs_iterator(nullptr);
    }
    
    bfs_iterator bfs_begin()
    {
        return bfs_iterator(m_top);
    }
    
    bfs_iterator bfs_end()
    {
        return bfs_iterator(nullptr);
    }
    
    dfs_iterator dfs_begin()
    {
        return dfs_iterator(m_top);
    }
    
    dfs_iterator dfs_end()
    {
        return dfs_iterator(nullptr);
    }
    
private:
    tree_item<T>* m_top;
};

#endif /* TREE_H */
