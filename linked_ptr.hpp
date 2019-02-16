#ifndef LINKED_PTR_HPP
#define LINKED_PTR_HPP

#include <iostream>

namespace smart_ptr {

    namespace details {
        struct linked_ptr_bind {
            linked_ptr_bind() : prev(nullptr), next(nullptr) {}
            linked_ptr_bind(linked_ptr_bind * prev, linked_ptr_bind * next) : prev(prev), next(next) {}
            mutable linked_ptr_bind * prev;
            mutable linked_ptr_bind * next;
        };
    }

    template <typename T>
    class linked_ptr : details::linked_ptr_bind {
    public:
        linked_ptr() : linked_ptr_bind(this, this), data(nullptr) {}

        template <typename U>
        explicit linked_ptr(U* ptr) : linked_ptr_bind(this, this), data(ptr) {}

        linked_ptr(const linked_ptr& other) : data(other.data) {
            bind(other);
        }

        template <class U>
        explicit linked_ptr(const linked_ptr<U>& other) : data(other.data) {
            bind(other);
        }

        linked_ptr& operator=(const linked_ptr& other) {
            reset(other.data);
            bind(other);
            return *this;
        }

        template <class U>
        linked_ptr& operator=(const linked_ptr<U>& other) {
            reset(other.data);
            bind(other);
            return *this;
        }

        template <class U>
        linked_ptr& operator=(U* ptr) {
            reset(ptr);
            return *this;
        }

        ~linked_ptr() {
            reset();
        }

        bool unique() {
            return (static_cast<const linked_ptr_bind*>(this) == next);
        }

        template <class U>
        void reset(U* ptr) {
            if (unique()) {
                delete data;
            } else {
                unbind();
            }
            data = ptr;
        }

        void reset() {
            if (unique()) {
                delete data;
            } else {
                unbind();
            }
            data = nullptr;
        }

        T& operator*()  const{
            return *data;
        }

        T* operator->() const {
            return data;
        }

        T* get() const {
            return data;
        }

        explicit operator bool() const {
            return (data != nullptr);
        }

        bool operator!() const {
            return (data == nullptr);
        }

        template <typename U>
        void swap(linked_ptr<U>& other) {
            if (data == other.data) {
                return;
            }
            if (unique() && !other.unique()) {
                next = other.next;
                prev = other.prev;
                next->prev = prev->next = this;
            } else if (!unique() && other.unique()) {
                other.next = next;
                other.prev = prev;
                other.next->prev = other.prev->next = &other;
            } else {
                std::swap(next, other.next);
                std::swap(prev, other.prev);
                next->prev = prev->next = this;
                other.next->prev = other.prev->next = &other;
            }
            std::swap(data, other.data);
        }

    private:
        template <class U>
        void bind(const linked_ptr<U> &other) {
            next = other.next;
            other.next = this;
            prev = const_cast<linked_ptr<U> *>(&other);
            next->prev = this;
        }

        void unbind() {
            next->prev = prev;
            prev->next = next;
            next = prev = this;
        }

        template <typename U> friend class linked_ptr;
        T* data;
    };

    //other function

    template <class T, class U>
    inline void swap(linked_ptr<T>& a, linked_ptr<U>& b) {
        a.swap(b);
    };

    template <class U>
    linked_ptr<U> make_linked_ptr(U* ptr) {
        return linked_ptr<U>(ptr);
    }

    //comparisons operator

    template <class T, class U>
    inline bool operator==(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() == b.get());
    }

    template <class T, class U>
    inline bool operator!=(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() != b.get());
    }

    template <class T, class U>
    bool operator<(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() < b.get());
    }

    template <class T, class U>
    inline bool operator>(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() > b.get());
    }

    template <class T, class U>
    inline bool operator>=(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() >= b.get());
    }

    template <class T, class U>
    inline bool operator<=(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() <= b.get());
    }


}

#endif //LINKED_PTR_HPP