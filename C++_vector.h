#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <new>
#include <memory>
#include <algorithm>
#include <cstdio>

template <typename T>
class Alloc {
 public:
  Alloc() {
  };
  ~Alloc() {
  };
  T* allocate(size_t n = 0) {
    T* p = new  T[n];
    return p;
  }
  void deallocate(T* p, size_t n) {
    delete [] p;
  }
  void construct(T* p, const T args = T(0)) {
    new(p) T(args);
  }
  void destroy(T* p) {
    p -> ~T();
  }
};

template <typename T>
class Vector{
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef T* iterator;
  typedef T& reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 protected:
  iterator start;
  iterator finish;
  iterator end_of_storage;
  Alloc<T> alloc;
  void realloc(size_type n) {
    const size_type old_size = size();
    const size_type new_size = (old_size != 0) ? (old_size + n) : 1;
	
    iterator new_start = alloc.allocate(new_size);
    iterator new_finish = new_start;
    
    new_finish = std::uninitialized_copy(start, finish, new_start);

    deallocate();

    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + new_size;
  }
  void deallocate() {
    if (start)
      alloc.deallocate(start, end_of_storage - start);
  }
  void fill_initialize(size_type n, const T& value) {
    start = alloc.allocate(n);
    for (int i = 0; i < n; ++i) {
      start[i] = value;
    }
    finish = start + n;
    end_of_storage = finish;
  }

 public:
  //  Returns an iterator pointing to the first element in the vector.
  iterator begin() {
    return start;
  }

  //  Returns an iterator referring to the past-the-end element in the vector 
  //    container.
  iterator end() {
    return finish;
  }

  //  Returns the number of elements in the vector.
  size_type size() const {
    return size_type(finish - start);
  }

  //  Returns the maximum number of elements that the vector can hold.
  size_type max_size() const {
    return size_type(-1) / sizeof(size_type);
  }

  //  Returns the size of the storage space currently allocated for the vector, 
  //    expressed in terms of elements.
  //  This capacity is not necessarily equal to the vector size. It can be equal 
  //    or greater, with the extra space allowing to accommodate for growth 
  //    without the need to reallocate on each insertion.
  size_type capacity() const {
    return size_type(end_of_storage - start);
  }

  //  Returns whether the vector is empty (i.e. whether its size is 0).
  //  This function does not modify the container in any way. To clear the 
  //    content of a vector, see vector::clear.
  bool empty() const {
    return begin() == end();
  }

  //  Returns a reference to the element at position n in the vector container.
  value_type& operator[](size_type n) {
    return *(begin() + n);
  }

//  Constructs an empty container, with no elements.
  explicit Vector() : start(0), finish(0), end_of_storage(0) {
  }

  //  Constructs a container with n elements. Each element is a copy of val.
  explicit Vector(size_type n, value_type& val) {
    fill_initialize(n, val);
  }

  //  Constructs a container with as many elements as the range [first,last), 
  //    with each element constructed from its corresponding element in that range, 
  //    in the same order.
  Vector (iterator first, iterator last) {
    size_type n = size_type(last - first);
    fill_initialize(n, T(0));
    size_type i = 0;
    do{
      *(begin() + i) = *first;
      ++first;
    } while(first != last);
  }

  //  Constructs a container with a copy of each of the elements in x, 
  //    in the same order.
  Vector(Vector& ant_vec){
  	deallocate();
    size_type size_of_vec = ant_vec.size();
    size_type capacity_of_vec = ant_vec.capacity();
    fill_initialize(size_of_vec, T(0));
    for (size_type i = 0; i < size_of_vec; ++i) {
      *(begin() + i) = ant_vec[i];
    }
  }

  //  This destroys all container elements, and deallocates all the storage 
  //    capacity allocated by the vector using its allocator.
  ~Vector() {
    deallocate();
  }

  //  Assigns new contents to the container, replacing its current contents, 
  //    and modifying its size accordingly.
  Vector& operator=(Vector& ant_vec) {
    size_type size_of_vec = ant_vec.size();
    size_type capacity_of_vec = ant_vec.capacity();
    deallocate();
    fill_initialize(capacity_of_vec, T(0));
    for (size_type i = 0; i < size_of_vec; ++i) {
      *(begin() + i) = ant_vec[i];
    }
    return *this;
  }

  //  Returns a reference to the element at position n in the vector.
  value_type& at(size_type n) const{
    return *(begin() + n);
  }

  //  Returns a reference to the first element in the vector.
  //  Unlike member vector::begin, which returns an iterator to this same element, 
  //    this function returns a direct reference.
  value_type& front() {
    return *begin();
  }

  //  Returns a reference to the last element in the vector.
  //  Unlike member vector::end, which returns an iterator just past this element, 
  //    this function returns a direct reference.
  value_type& back() {
    return *(end() - 1);
  } 

  //  Adds a new element at the end of the vector, after its current last element. 
  //    The content of val is copied (or moved) to the new element.
  //  This effectively increases the container size by one, which causes an 
  //    automatic reallocation of the allocated storage space if -and only if- 
  //    the new vector size surpasses the current vector capacity.
  void push_back(const value_type& val) {
    if (finish != end_of_storage) {
      alloc.construct(finish, val);
      ++finish;
    } else {
      realloc(1);
      alloc.construct(finish, val);
      ++finish;
    }
    printf("%d %d\n", size(), capacity());
  }

  //  Removes the last element in the vector, effectively reducing the container 
  //    size by one.
  void pop_back() {
    --finish;
    alloc.destroy(finish);
  }

  //  Removes from the vector either a single element (position) or a range of 
  //    elements ([first,last)).
  iterator erase (iterator position) {
    if (position + 1 != end()) {
      std::copy(position + 1, finish, position);
    }
    --finish;
    alloc.destroy(finish);
    return position;
  }

  iterator erase (iterator first, iterator last) {
    int n = size_type(last - first);
    if (last != end())
      std::copy(last, finish, first);
    finish -= n;
    for (int i = 0; i < n; ++i) 
      alloc.destroy(finish + i);
    return first;
  }

  //  The vector is extended by inserting new elements before the element at the 
  //    specified position, effectively increasing the container size by the
  //    number of elements inserted.
  iterator insert (iterator position, const value_type& val) {
    if (position + 1 != end()) {
      std::copy_backward(position + 1, finish, finish + 1);
    }
    ++finish;
    destroy(finish);
    return position;
  }

  void insert (iterator position, size_type n, const value_type& val) {
    if (size() + n <= capacity()) {
      std::copy_backward(position, finish, finish + n);
      std::fill(position, position + n, val);
      finish += n;
    } else {
      const size_type old_size = size();
      const size_type new_size = (old_size != 0) ? (old_size + n * 2) : 1;
	
      iterator new_start = alloc.allocate(new_size);
      iterator new_finish = new_start;
    
      new_finish = std::uninitialized_copy(start, position, new_start);
      std::uninitialized_fill_n(new_finish, n, val);
	    new_finish = std::uninitialized_copy(position, finish, new_finish + n);
      deallocate();

      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + new_size;
    }
  }

  void insert (iterator position, iterator first, iterator last) {
    size_type n = size_type(last - first);
    if (size() + n <= capacity()) {
      std::copy_backward(position, finish, finish + n);
      std::copy(first, last, position);
      finish += n;
    } else {
      realloc(n);
      std::copy_backward(position, finish, finish + n);
      std::copy(first, last, position);
      finish += n;
    }
  }

  //  Resizes the container so that it contains n elements.
  //  If n is smaller than the current container size, the content is reduced 
  //    to its first n elements, removing those beyond (and destroying them).
  //  If n is greater than the current container size, the content is expanded 
  //    by inserting at the end as many elements as needed to reach a size of n. 
  //    If val is specified, the new elements are initialized as copies of val, 
  //    otherwise, they are value-initialized.
  //  If n is also greater than the current container capacity, an automatic 
  //    reallocation of the allocated storage space takes place.
  //  Notice that this function changes the actual content of the container by 
  //    inserting or erasing elements from it.
  void resize(size_type n) {
    if (n < size()) {
      erase(begin() + n, finish);
    } else if (n > size()) {
      insert(end(), n - size(), T(0));
    }
  }

  void resize(size_type n, value_type value) {
    if (n < size())
      erase(begin() + n, end());
    else
      insert(end(), n - size(), value);
  }

  //  Requests that the vector capacity be at least enough to contain n elements.
  //  If n is greater than the current vector capacity, the function causes the 
  //    container to reallocate its storage increasing its capacity to n 
  //    (or greater).
  //  In all other cases, the function call does not cause a reallocation and 
  //    the vector capacity is not affected.
  //  This function has no effect on the vector size and cannot alter its elements.
  void reserve(size_type n) {
    if(n > size())
      insert(end(), n - size(), T(0));
  }

  //  Removes from the vector either a single element (position) or a range of 
  //    elements ([first,last)).
  void swap(Vector& x) {
    Vector<T> temp(x);
    x = *this;
    *this = temp;
  }

  //  Assigns new contents to the vector, replacing its current contents, and 
  //    modifying its size accordingly.
  void assign (iterator first, iterator last) {
  	size_type n = size_type(last - first);
    resize(n);
    unintialized_copy(first, last, start);
  }

  void assign (size_type n, const value_type& val) {
    resize(n);
    for (size_type i = 0; i < n; ++i) {
      *(begin() + i) = val;
    }
  }

  //  Removes all elements from the vector (which are destroyed), leaving the 
  //    container with a size of 0.
  void clear() {
    erase(begin(), end());
  }

  //  Returns a copy of the allocator object associated with the vector.
  Alloc<value_type> get_allocator() const {
    return alloc;
  }
};

//  Performs the appropriate comparison operation between the vector containers 
//    lhs and rhs.
template <typename T>
bool operator== (Vector<T>& lhs, Vector<T>& rhs) {
  if (lhs.size() != rhs.size())  return false;
  size_t size = lhs.size();
  for(size_t i = 0; i < size; ++i) {
    if(lhs[i] != rhs[i])  return false;
  }
  return true;
}

template <typename T>
bool operator!= (Vector<T>& lhs, Vector<T>& rhs) {
  return !(lhs == rhs);
}
 
template <typename T> 
bool operator<  (Vector<T>& lhs, Vector<T>& rhs) {
  size_t n = (lhs.size() > rhs .size()) ? lhs.size() : rhs.size();
  for(size_t i = 0; i < n; ++i) {
    if(lhs[i] < rhs[i]) return true;
    if(lhs[i] > rhs[i]) return false;
  }
  if(lhs.size() < rhs.size()) return true;
  return false;
}

template <typename T>
bool operator>  (Vector<T>& lhs, Vector<T>& rhs) {
  return !(lhs < rhs) && !(lhs == rhs);
}
 
template <typename T> 
bool operator<= (Vector<T>& lhs, Vector<T>& rhs) {
  return (lhs < rhs) || (lhs == rhs);
}
 
template <typename T> 
bool operator>= (Vector<T>& lhs, Vector<T>& rhs) {
  return !(lhs < rhs);
}

//  Exchanges the content of the container by the content of x, which is another
//    vector object of the same type. Sizes may differ.
template <class T>
void swap (Vector<T>& x, Vector<T>& y) {
  Vector<T>* p_tmp = Vector<T>(x);
  x = y;
  y = *p_tmp;
  delete p_tmp;
}
#endif
