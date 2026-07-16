#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>
#include <new>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t new_capacity) {
        T* new_data = static_cast<T*>(operator new(new_capacity * sizeof(T)));
        
        // Move existing elements using placement new
        for (size_t i = 0; i < size_; ++i) {
            new(new_data + i) T(std::move(data_[i]));
            data_[i].~T();  // Destroy old object
        }
        
        operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void check_index(size_t pos) const {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
    }

    void check_empty() const {
        if (size_ == 0) {
            throw container_is_empty();
        }
    }

public:
    /**
     * TODO
     * a type for actions of the elements of a vector, and you should write
     *   a class named const_iterator with same interfaces.
     */
    /**
     * you can see RandomAccessIterator at CppReference for help.
     */
    class const_iterator;
    class iterator
    {
    friend class vector<T>;
    friend class const_iterator;
    
    private:
        vector<T>* parent_;
        size_t index_;
        
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

        iterator(vector<T>* parent = nullptr, size_t index = 0) 
            : parent_(parent), index_(index) {}
        
        iterator(const iterator &other) 
            : parent_(other.parent_), index_(other.index_) {}
        
        iterator operator+(const int &n) const
        {
            return iterator(parent_, index_ + n);
        }
        
        iterator operator-(const int &n) const
        {
            return iterator(parent_, index_ - n);
        }
        
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const
        {
            if (parent_ != rhs.parent_) {
                throw invalid_iterator();
            }
            return static_cast<int>(index_ - rhs.index_);
        }
        
        iterator& operator+=(const int &n)
        {
            index_ += n;
            return *this;
        }
        
        iterator& operator-=(const int &n)
        {
            index_ -= n;
            return *this;
        }
        
        /**
         * TODO iter++
         */
        iterator operator++(int) 
        {
            iterator temp = *this;
            ++index_;
            return temp;
        }
        
        /**
         * TODO ++iter
         */
        iterator& operator++() 
        {
            ++index_;
            return *this;
        }
        
        /**
         * TODO iter--
         */
        iterator operator--(int) 
        {
            iterator temp = *this;
            --index_;
            return temp;
        }
        
        /**
         * TODO --iter
         */
        iterator& operator--() 
        {
            --index_;
            return *this;
        }
        
        /**
         * TODO *it
         */
        T& operator*() const
        {
            if (parent_ == nullptr || index_ >= parent_->size_) {
                throw invalid_iterator();
            }
            return parent_->data_[index_];
        }
        
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const 
        {
            return parent_ == rhs.parent_ && index_ == rhs.index_;
        }
        
        bool operator==(const const_iterator &rhs) const;
        
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const 
        {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator &rhs) const;
        
        T* operator->() const 
        {
            return &(operator*());
        }
    };
    
    /**
     * TODO
     * has same function as iterator, just for a const object.
     */
    class const_iterator
    {
    friend class vector<T>;
    friend class iterator;
    
    private:
        const vector<T>* parent_;
        size_t index_;
        
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::random_access_iterator_tag;

        const_iterator(const vector<T>* parent = nullptr, size_t index = 0) 
            : parent_(parent), index_(index) {}
        
        const_iterator(const const_iterator &other) 
            : parent_(other.parent_), index_(other.index_) {}
        
        const_iterator(const iterator &other) 
            : parent_(other.parent_), index_(other.index_) {}
        
        const_iterator operator+(const int &n) const
        {
            return const_iterator(parent_, index_ + n);
        }
        
        const_iterator operator-(const int &n) const
        {
            return const_iterator(parent_, index_ - n);
        }
        
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const
        {
            if (parent_ != rhs.parent_) {
                throw invalid_iterator();
            }
            return static_cast<int>(index_ - rhs.index_);
        }
        
        int operator-(const iterator &rhs) const
        {
            if (parent_ != rhs.parent_) {
                throw invalid_iterator();
            }
            return static_cast<int>(index_ - rhs.index_);
        }
        
        const_iterator& operator+=(const int &n)
        {
            index_ += n;
            return *this;
        }
        
        const_iterator& operator-=(const int &n)
        {
            index_ -= n;
            return *this;
        }
        
        const_iterator operator++(int) 
        {
            const_iterator temp = *this;
            ++index_;
            return temp;
        }
        
        const_iterator& operator++() 
        {
            ++index_;
            return *this;
        }
        
        const_iterator operator--(int) 
        {
            const_iterator temp = *this;
            --index_;
            return temp;
        }
        
        const_iterator& operator--() 
        {
            --index_;
            return *this;
        }
        
        const T& operator*() const
        {
            if (parent_ == nullptr || index_ >= parent_->size_) {
                throw invalid_iterator();
            }
            return parent_->data_[index_];
        }
        
        bool operator==(const const_iterator &rhs) const 
        {
            return parent_ == rhs.parent_ && index_ == rhs.index_;
        }
        
        bool operator==(const iterator &rhs) const 
        {
            return parent_ == rhs.parent_ && index_ == rhs.index_;
        }
        
        bool operator!=(const const_iterator &rhs) const 
        {
            return !(*this == rhs);
        }
        
        bool operator!=(const iterator &rhs) const 
        {
            return !(*this == rhs);
        }
        
        const T* operator->() const 
        {
            return &(operator*());
        }
    };
    
    /**
     * TODO Constructs
     * At least two: default constructor, copy constructor
     */
    vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    vector(const vector &other) : data_(nullptr), size_(0), capacity_(0)
    {
        if (other.capacity_ > 0) {
            data_ = static_cast<T*>(operator new(other.capacity_ * sizeof(T)));
            capacity_ = other.capacity_;
            
            try {
                for (size_t i = 0; i < other.size_; ++i) {
                    new(data_ + i) T(other.data_[i]);
                    ++size_;
                }
            } catch (...) {
                // Clean up on exception
                for (size_t i = 0; i < size_; ++i) {
                    data_[i].~T();
                }
                operator delete(data_);
                throw;
            }
        }
    }
    
    /**
     * TODO Destructor
     */
    ~vector() 
    {
        clear();
        operator delete(data_);
    }
    
    /**
     * TODO Assignment operator
     */
    vector &operator=(const vector &other) 
    {
        if (this == &other) {
            return *this;
        }
        
        vector temp(other);
        std::swap(data_, temp.data_);
        std::swap(size_, temp.size_);
        std::swap(capacity_, temp.capacity_);
        return *this;
    }
    
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     */
    T & at(const size_t &pos) 
    {
        check_index(pos);
        return data_[pos];
    }
    
    const T & at(const size_t &pos) const
    {
        check_index(pos);
        return data_[pos];
    }
    
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     * !!! Pay attentions
     *   In STL this operator does not check the boundary but I want you to do.
     */
    T & operator[](const size_t &pos) 
    {
        return at(pos);
    }
    
    const T & operator[](const size_t &pos) const
    {
        return at(pos);
    }
    
    /**
     * access the first element.
     * throw container_is_empty if size == 0
     */
    const T & front() const
    {
        check_empty();
        return data_[0];
    }
    
    /**
     * access the last element.
     * throw container_is_empty if size == 0
     */
    const T & back() const
    {
        check_empty();
        return data_[size_ - 1];
    }
    
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() 
    {
        return iterator(this, 0);
    }
    
    const_iterator begin() const
    {
        return const_iterator(this, 0);
    }
    
    const_iterator cbegin() const
    {
        return begin();
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() 
    {
        return iterator(this, size_);
    }
    
    const_iterator end() const
    {
        return const_iterator(this, size_);
    }
    
    const_iterator cend() const
    {
        return end();
    }
    
    /**
     * checks whether the container is empty
     */
    bool empty() const
    {
        return size_ == 0;
    }
    
    /**
     * returns the number of elements
     */
    size_t size() const
    {
        return size_;
    }
    
    /**
     * clears the contents
     */
    void clear() 
    {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
    
    /**
     * inserts value before pos
     * returns an iterator pointing to the inserted value.
     */
    iterator insert(iterator pos, const T &value) 
    {
        if (pos.parent_ != this || pos.index_ > size_) {
            throw invalid_iterator();
        }
        
        return insert(pos.index_, value);
    }
    
    /**
     * inserts value at index ind.
     * after inserting, this->at(ind) == value
     * returns an iterator pointing to the inserted value.
     * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
     */
    iterator insert(const size_t &ind, const T &value) 
    {
        if (ind > size_) {
            throw index_out_of_bound();
        }
        
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        
        // Move elements to make space
        for (size_t i = size_; i > ind; --i) {
            new(data_ + i) T(std::move(data_[i - 1]));
            data_[i - 1].~T();
        }
        
        // Insert new element
        new(data_ + ind) T(value);
        ++size_;
        
        return iterator(this, ind);
    }
    
    /**
     * removes the element at pos.
     * return an iterator pointing to the following element.
     * If the iterator pos refers the last element, the end() iterator is returned.
     */
    iterator erase(iterator pos) 
    {
        if (pos.parent_ != this || pos.index_ >= size_) {
            throw invalid_iterator();
        }
        
        return erase(pos.index_);
    }
    
    /**
     * removes the element with index ind.
     * return an iterator pointing to the following element.
     * throw index_out_of_bound if ind >= size
     */
    iterator erase(const size_t &ind) 
    {
        check_index(ind);
        
        data_[ind].~T();
        
        // Move elements to fill gap
        for (size_t i = ind; i < size_ - 1; ++i) {
            new(data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        
        --size_;
        return iterator(this, ind);
    }
    
    /**
     * adds an element to the end.
     */
    void push_back(const T &value) 
    {
        insert(size_, value);
    }
    
    /**
     * remove the last element from the end.
     * throw container_is_empty if size() == 0
     */
    void pop_back() 
    {
        check_empty();
        erase(size_ - 1);
    }
};

// Define out-of-class comparison operators
template<typename T>
bool vector<T>::iterator::operator==(const const_iterator &rhs) const 
{
    return parent_ == rhs.parent_ && index_ == rhs.index_;
}

template<typename T>
bool vector<T>::iterator::operator!=(const const_iterator &rhs) const 
{
    return !(*this == rhs);
}

}

#endif
