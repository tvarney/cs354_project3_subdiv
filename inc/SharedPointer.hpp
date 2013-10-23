
#ifndef CS354_GENERIC_SHARED_POINTER_HPP
#define CS354_GENERIC_SHARED_POINTER_HPP

namespace cs354 {
    template<typename T> void _delete(T *ptr) {
        delete ptr;
    }
    template<typename T> void _delete_array(T *ptr) {
        delete [] ptr;
    }
    
    template <typename T, void (*_deleter)(T *) = &(_delete<T>)>
    class SharedPointer {
    public:
        /* Default constructor, initialize to NULL */
        SharedPointer() :
            refs(NULL), ptr(NULL)
        { }
        /* Normal constructor, checks for NULL. */
        SharedPointer(T* ptr) :
            refs(ptr ? new int(1) : NULL), ptr(ptr)
        { }
        /* Copy Constructor, increments the reference count of the object it
         * is copying if (source.refs != NULL)
         */
        SharedPointer(const SharedPointer<T> &source) :
            refs(source.refs), ptr(source.ptr)
        {
            if(refs) {
                *refs = (*refs) + 1;
            }
        }
        ~SharedPointer() {
            if(refs) {
                *refs = (*refs) - 1;
                if((*refs) == 0) {
                    delete refs;
                    _deleter(ptr);
                }
            }
        }
        
        T& operator*() {
            /* If NULL, segfault. It's not MY responsibility to check for NULL
             */
            return *ptr;
        }
        SharedPointer<T> & operator=(const SharedPointer<T> rhs) {
            if(refs) {
                *refs = (*refs) - 1;
                if((*refs) == 0) {
                    delete refs;
                    _deleter(ptr);
                }
            }
            
            refs = rhs.refs;
            ptr = rhs.ptr;
            if(refs) {
                *refs = (*refs) + 1;
            }
            
            return *this;
        }
    protected:
        volatile int *refs;
        T* ptr;
    };
}

#endif
