//
//  Created by bogdan.mytnyk on 12/11/17.
//  Copyright © 2017 bogdan.mytnyk. All rights reserved.
//

#ifndef BFS_ADAPTOR_H
#define BFS_ADAPTOR_H

#include "tree.h"

template <typename T>
struct const_bfs_adaptor
{
    const_bfs_adaptor(const tree<T>& tree) :
        m_tree(tree)
    {
    }
    
    const_bfs_adaptor(const const_bfs_adaptor<T>& adaptor) :
        m_tree(adaptor.m_tree)
    {
    }
    
    inline typename tree<T>::const_bfs_iterator begin() const
    {
        return m_tree.bfs_cbegin();
    };
    
    inline typename tree<T>::const_bfs_iterator end() const
    {
        return m_tree.bfs_cend();
    };
    
private:
    const tree<T>& m_tree;
};

template <typename T>
struct bfs_adaptor : public const_bfs_adaptor<T>
{
    typedef const_bfs_adaptor<T> base_type;
    
    bfs_adaptor(tree<T>& tree) :
        base_type(tree),
        m_tree(tree)
    {
    }
    
    bfs_adaptor(const bfs_adaptor<T>& adaptor) :
        base_type(adaptor.m_tree),
        m_tree(adaptor.m_tree)
    {
    }
    
    inline typename tree<T>::bfs_iterator begin()
    {
        return m_tree.bfs_begin();
    };
    
    inline typename tree<T>::bfs_iterator end()
    {
        return m_tree.bfs_end();
    };
    
    inline typename tree<T>::bfs_iterator begin() const
    {
        return m_tree.bfs_begin();
    };
    
    inline typename tree<T>::dfs_iterator end() const
    {
        return m_tree.bfs_end();
    }
    
private:
    tree<T>& m_tree;
};

template <typename T>
bfs_adaptor<T> make_bfs_adaptor(tree<T>& tree)
{
    return bfs_adaptor<T>(tree);
}

template <typename T>
const const_bfs_adaptor<T> make_bfs_adaptor(const tree<T>& tree)
{
    return const_bfs_adaptor<T>(tree);
}

#endif /* BFS_ADAPTOR_H */
