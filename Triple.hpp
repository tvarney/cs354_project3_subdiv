
#ifndef CS354_GENERIC_TRIPLE_HPP
#define CS354_GENERIC_TRIPLE_HPP

namespace cs354 {
    template <typename T>
    struct Triple {
        T x, y, z;
        
        Triple() { }
        Triple(T x, T y, T z) : x(x), y(y), z(z) { }
        Triple(T data[3]) : x(data[0]), y(data[1]), z(data[1]) { }
        ~Triple() { }
    };
}

#endif
