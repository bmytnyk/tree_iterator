//
//  Created by bogdan.mytnyk on 12/11/17.
//  Copyright © 2017 bogdan.mytnyk. All rights reserved.
//

#ifndef DFS_ADAPTOR_H
#define DFS_ADAPTOR_H

#include "tree.h"

template <typename T>
struct const_dfs_adaptor
{
    const_dfs_adaptor(tree<T>& tree):
        m_tree(tree)
    {
    }
    
    const_dfs_adaptor(const const_dfs_adaptor<T>& adaptor):
        m_tree(adaptor.m_tree)
    {
    }
    
    inline typename tree<T>::const_dfs_iterator begin() const
    {
        return m_tree.dfs_cbegin();
    }
    
    inline typename tree<T>::const_dfs_iterator end() const
    {
        return m_tree.dfs_cend();
    }
    
    typedef typename tree<T>::const_dfs_iterator const_iterator;
    
private:
    const tree<T>& m_tree;
};

template <typename T>
struct dfs_adaptor : public const_dfs_adaptor<T>
{
    dfs_adaptor(tree<T>& tree):
        const_dfs_adaptor<T>(tree),
        m_tree(tree)
    {
    }
    
    dfs_adaptor(const dfs_adaptor<T>& adaptor):
        const_dfs_adaptor<T>(adaptor),
        m_tree(adaptor.m_tree)
    {
    }
    
    inline typename tree<T>::dfs_iterator begin() const
    {
        return m_tree.dfs_begin();
    }
    
    inline typename tree<T>::dfs_iterator end() const
    {
        return m_tree.dfs_end();
    }
    
    typedef typename tree<T>::dfs_iterator iterator;
    
private:
    tree<T>& m_tree;
};

template <typename T>
dfs_adaptor<T> make_dfs_adaptor(tree<T>& tree)
{
    return dfs_adaptor<T>(tree);
}

template <typename T>
const const_dfs_adaptor<T> make_dfs_adaptor(const tree<T>& tree)
{
    return const_dfs_adaptor<T>(tree);
}

#endif /* DFS_ADAPTOR_H */
