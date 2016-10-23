#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <new>
#include <memory>

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
  void realloc();
  void deallocate() {
    if (start)
      alloc.deallocate(start, end_of_storage - start);
  }
  void fill_initialilze(size_type n, const T& value) {
    start = alloc.allocate(n);
    for (int i = 0; i < n; ++i) {
      start[i] = value;
    }
    finish = start + n;
    end_of_storage = finish;
  }

 public:
  //  Constructs an empty container, with no elements.
  explicit Vector();

  //  Constructs a container with n elements. Each element is a copy of val.
  explicit Vector(size_type n, value_type& val);

  //  Constructs a container with as many elements as the range [first,last), 
  //    with each element constructed from its corresponding element in that range, 
  //    in the same order.
  Vector (iterator first, iterator last);

  //  Constructs a container with a copy of each of the elements in x, 
  //    in the same order.
  Vector(const Vector& ant_vec);

  //  This destroys all container elements, and deallocates all the storage 
  //    capacity allocated by the vector using its allocator.
  ~Vector();

  //  Assigns new contents to the container, replacing its current contents, 
  //    and modifying its size accordingly.
  const Vector& operator=(const Vector& ant_vec);

  //  Returns an iterator pointing to the first element in the vector.
  iterator begin();

  //  Returns an iterator referring to the past-the-end element in the vector 
  //    container.
  iterator end();

  //  Returns the number of elements in the vector.
  size_type size() const;

  //  Returns the maximum number of elements that the vector can hold.
  size_type max_size() const;

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
  void resize(size_type n);
  void resize(size_type n, value_type value);

  //  Returns the size of the storage space currently allocated for the vector, 
  //    expressed in terms of elements.
  //  This capacity is not necessarily equal to the vector size. It can be equal 
  //    or greater, with the extra space allowing to accommodate for growth 
  //    without the need to reallocate on each insertion.
  size_type capacity() const;

  //  Returns whether the vector is empty (i.e. whether its size is 0).
  //  This function does not modify the container in any way. To clear the 
  //    content of a vector, see vector::clear.
  bool empty() const;

  //  Requests that the vector capacity be at least enough to contain n elements.
  //  If n is greater than the current vector capacity, the function causes the 
  //    container to reallocate its storage increasing its capacity to n 
  //    (or greater).
  //  In all other cases, the function call does not cause a reallocation and 
  //    the vector capacity is not affected.
  //  This function has no effect on the vector size and cannot alter its elements.
  void reserve(size_type n);

  //  Returns a reference to the element at position n in the vector container.
  value_type& operator[](size_type n) const;

  //  Returns a reference to the element at position n in the vector.
  value_type& at(size_type n) const;

  //  Returns a reference to the first element in the vector.
  //  Unlike member vector::begin, which returns an iterator to this same element, 
  //    this function returns a direct reference.
  value_type& front();

  //  Returns a reference to the last element in the vector.
  //  Unlike member vector::end, which returns an iterator just past this element, 
  //    this function returns a direct reference.
  value_type& back(); 

  //  Assigns new contents to the vector, replacing its current contents, and 
  //    modifying its size accordingly.
  void assign (iterator first, iterator last);

  void assign (size_type n, const value_type& val);

  //  Adds a new element at the end of the vector, after its current last element. 
  //    The content of val is copied (or moved) to the new element.
  //  This effectively increases the container size by one, which causes an 
  //    automatic reallocation of the allocated storage space if -and only if- 
  //    the new vector size surpasses the current vector capacity.
  void push_back(const value_type& val);

  //  Removes the last element in the vector, effectively reducing the container 
  //    size by one.
  void pop_back();

  //  The vector is extended by inserting new elements before the element at the 
  //    specified position, effectively increasing the container size by the
  //    number of elements inserted.
  iterator insert (iterator position, const value_type& val);
  void insert (iterator position, size_type n, const value_type& val);
  void insert (iterator position, iterator first, iterator last);

  //  Removes from the vector either a single element (position) or a range of 
  //    elements ([first,last)).
  iterator erase (iterator position);
  iterator erase (iterator first, iterator last);

  //  Removes from the vector either a single element (position) or a range of 
  //    elements ([first,last)).
  void swap(Vector& x);

  //  Removes all elements from the vector (which are destroyed), leaving the 
  //    container with a size of 0.
  void clear();

  //  Returns a copy of the allocator object associated with the vector.
  Alloc<value_type> get_allocator() const;
};

//  Performs the appropriate comparison operation between the vector containers 
//    lhs and rhs.
template <class T>
bool operator== (const Vector<T>& lhs, const Vector<T>& rhs);
template <class T>
bool operator!= (const Vector<T>& lhs, const Vector<T>& rhs);
template <class T>
bool operator<  (const Vector<T>& lhs, const Vector<T>& rhs);
template <class T>
bool operator<= (const Vector<T>& lhs, const Vector<T>& rhs);
template <class T>
bool operator>  (const Vector<T>& lhs, const Vector<T>& rhs);
template <class T>
bool operator>= (const Vector<T>& lhs, const Vector<T>& rhs);

//  Exchanges the content of the container by the content of x, which is another
//    vector object of the same type. Sizes may differ.
template <class T>
void swap (Vector<T>& x, Vector<T>& y);
#endif
