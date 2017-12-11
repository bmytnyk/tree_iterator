//
//  Created by bogdan.mytnyk on 12/11/17.
//  Copyright © 2017 bogdan.mytnyk. All rights reserved.
//

#ifndef TREE_H
#define TREE_H

#include <vector>
#include <queue>
#include <stack>

template <typename T>
class tree_item
{
public:
    using self_type = tree_item<T>;
    using children_container = std::vector<self_type*>;
    
    explicit tree_item(const T& value) :
    m_value(value),
    m_children()
    {
    }
    
    tree_item(const self_type& item):
    m_value(item.value)
    {
        copy_children(item);
    }
    
    tree_item& operator=(const self_type& item)
    {
        if (this != &item)
        {
            copy_children(item);
        }
        
        return *this;
    }
    
    
    ~tree_item()
    {
        for (self_type* item : m_children)
            delete item;
    }
    
    inline const T& value() const { return m_value; }
    inline T& value() { return m_value; }
    
    inline const children_container& children() const { return m_children; }
    children_container& children() { return m_children; };
    
    void add_child(self_type* child) { m_children.push_back(child); }
    
    // TODO : check edge cases
    void remove_child(size_t index) { m_children.erase(m_children.begin() + index); }
    
    self_type* get_child(size_t index) { return m_children[index]; }
    const self_type* get_child(size_t index) const { return m_children[index]; }
    
    inline size_t count() const { return m_children.size(); };
    
private:
    T m_value;
    children_container m_children;
    
    void copy_children(const self_type& item)
    {
        const auto& children = item.children();
        m_children.reserve(children.size());
        std::for_each(children.begin(), children.end(), [this](self_type* item)
                      {
                          m_children.push_back(new tree_item(*item));
                      });
    }
};

template <typename T, typename Container>
struct top_helper
{
};

template <typename T>
struct top_helper<T, std::queue<T>>
{
    static const T& get(std::queue<T>& c)
    {
        return c.front();
    }
};

template <typename T>
struct top_helper<T, std::stack<T>>
{
    static const T& get(std::stack<T>& c)
    {
        return c.top();
    }
};

template <typename T>
class tree
{
public:
    typedef tree_item<T> item_type;
    
    tree():
        m_top(nullptr)
    {
    }
    
    tree(const T& value) :
        m_top(new tree_item<T>(value))
    {
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
    class const_base_iterator
    {
    public:
        const_base_iterator(item_type* cur) :
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
        
        const_base_iterator& operator++()
        {
            if (m_current != nullptr)
            {
                for (size_t i = 0, count = m_current->count(); i < count; ++i)
                    m_nextItems.push(m_current->get_child(i));
                
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
        
        const_base_iterator operator++(int)
        {
            const_base_iterator prev(*this);
            ++*this;
            return prev;
        };
        
        const T& operator*() const
        {
            if (!m_current)
                throw std::runtime_error("Dereferencing invalid iterator");
            
            return m_current->value();
        }
        
        const T* operator->() const
        {
            if (!m_current)
                throw std::runtime_error("Invalid iterator");
            
            return &m_current->value();
        }
        
        bool operator == (const const_base_iterator& iter) const
        {
            return (m_current == iter.m_current);
        }
        
        bool operator != (const const_base_iterator& iter) const
        {
            return !(*this == iter);
        }
        
    protected:
        ContainerType m_nextItems;
        item_type* m_current;
    };
    
    typedef const_base_iterator<std::stack<item_type*>> const_dfs_iterator;
    typedef const_base_iterator<std::queue<item_type*>> const_bfs_iterator;
    
    template <typename ContainerType>
    class base_iterator : const_base_iterator<ContainerType>
    {
    public:
        using base_type = const_base_iterator<ContainerType>;
        using base_type::m_current;
        using base_type::m_nextItems;
        
        base_iterator(item_type* cur):
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
        
        T& operator*()
        {
            if (!m_current)
                throw std::runtime_error("Dereferencing invalid iterator");
            
            return m_current->value();
        }
        
        T* operator->()
        {
            if (!m_current)
                throw std::runtime_error("Dereferencing invalid iterator");
            
            return &m_current->value();
        }
        
        // Add increments
        base_iterator& operator++()
        {
            if (m_current != nullptr)
            {
                for (size_t i = 0, count = m_current->count(); i < count; ++i)
                    m_nextItems.push(m_current->get_child(i));
                
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
        
        bool operator == (const base_iterator& iter) const
        {
            return (m_current == iter.m_current);
        }
        
        bool operator != (const base_iterator& iter) const
        {
            return !(*this == iter);
        }
    };
    
    typedef base_iterator<std::stack<item_type*>> dfs_iterator;
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
