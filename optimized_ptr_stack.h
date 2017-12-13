//
//  Created by bogdan.mytnyk on 12/13/17.
//  Copyright © 2017 bogdan.mytnyk. All rights reserved.
//

#ifndef optimized_ptr_stack_h
#define optimized_ptr_stack_h

template <typename T>
class optimized_ptr_stack
{
private:
    size_t m_allocated;
    T* m_storage;
    T* m_head;
    
    static constexpr size_t s_start_size = 64;
    static constexpr size_t s_realloc_factor = 2;
    
    static_assert(std::is_pointer<T>::value, "Should be used only for pointers");
    static_assert(s_realloc_factor >= 2, "Growth factor should be more or equal 2");
    
public:
    
    optimized_ptr_stack():
        m_allocated(0),
        m_storage(nullptr),
        m_head(nullptr)
    {
    }
    
    optimized_ptr_stack(const optimized_ptr_stack& rht):
        m_allocated(rht.m_allocated),
        m_storage(nullptr),
        m_head(nullptr)
    {
        if (rht.m_storage != nullptr)
        {
            m_storage = new T[rht.m_allocated];
            m_head = m_storage + (rht.m_head - rht.m_storage);
        }
    }
    
    optimized_ptr_stack(optimized_ptr_stack&& rht):
        m_allocated(rht.m_allocated),
        m_storage(rht.m_storage),
        m_head(rht.m_head)
    {
    }
    
    optimized_ptr_stack& operator=(const optimized_ptr_stack& rht)
    {
        if (this != &rht)
        {
            m_allocated = rht.m_allocated;
            if (m_allocated)
            {
                m_storage = new T[m_allocated];
                m_head = m_storage + (rht.m_head - rht.m_storage);
            }
            else
            {
                m_storage = nullptr;
                m_head = nullptr;
            }
        }
        
        return *this;
    }
    
    inline void push(T value)
    {
        if (m_head == nullptr)
        {
            m_allocated = s_start_size;
            m_storage = new T[m_allocated];
            m_head = m_storage;
            *m_head = value;
        }
        else if (m_head - m_storage < m_allocated)
        {
            *++m_head = value;
        }
        else
        {
            T* new_storage = new T[s_realloc_factor * m_allocated];
            memcpy(new_storage, m_storage, m_allocated);
            m_head = new_storage + m_allocated + 1;
            m_allocated = s_realloc_factor * m_allocated;
            delete [] m_storage;
            m_storage = new_storage;
            *m_head = value;
        }
    }
    
    inline void pop()
    {
        if (m_head != m_storage)
        {
            --m_head;
        }
        else
        {
            m_head = nullptr;
        }
    }
    
    inline T& top()
    {
        return *m_head;
    }
    
    inline const T& top() const
    {
        return *m_head;
    }
    
    inline bool empty() const
    {
        return m_head == nullptr;
    }
};

#endif /* optimized_ptr_stack_h */
