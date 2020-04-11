#ifndef __ARRAY_HPP
#define __ARRAY_HPP

template <class T, size_t N>
struct array {
    T data[N];

    static size_t length() { 
        return N; 
    }
    
    using type = T;

    T &operator[](size_t index) { 
        return data[index]; 
    }

    const T &operator[](size_t index) const { 
        return data[index]; 
    }

    T *begin() { 
        return &data[0]; 
    }
    const T *begin() const { 
        return &data[0]; 
    }

    T *end() { 
        return &data[N]; 
    }

    const T *end() const { 
        return &data[N]; 
    }

    bool operator==(const array<T, N> &rhs) const {
        if (this == &rhs)
            return true;
        for (size_t i = 0; i < N; i++)
            if ((*this)[i] != rhs[i])
                return false;
        return true;
    }

    bool operator!=(const array<T, N> &rhs) const {
        return !(*this == rhs);
    }
};

#endif //__ARRAY_HPP