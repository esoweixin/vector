#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

template <typename T>
class allocator{
   public:
    allocator(){
    };
    ~allocator(){
    };
    T* allocate(size_t n = 0){
      T* p = new  T[n];
      return p;
    }
    void deallocate(T* p, size_t n){
      delete [] p;
    }
    void construct(T* p, const T args = T(0)){
      p = new T(args);
    }
    void destroy(T* p){
      p -> ~Vector();
    }
};

template <typename T>
class Vector
{
 public: 
  class iterator{
   public:
    T* ptr;
    iterator(){
      ptr = NULL; 
    };
    ~iterator(){
    };
    iterator(T* p){
      ptr = p;
    }
    T& operator*(){
      return *ptr;
    }
    iterator& operator++(){
      ++ptr;
      return *this;
    }
    iterator& operator--(){
      --ptr;
      return *this;
    }
    iterator& operator=(const iterator& it){
      return iterator(it.ptr);
    }
    bool operator==(const iterator& it){
      return (ptr == it.ptr);
    }
    bool operator!=(const iterator& it){
      return (ptr != it.ptr);
    }
};


  class reverse_iterator{
   public:
    T* ptr;
    reverse_iterator(){
      ptr = NULL;
    };
    ~reverse_iterator(){
    };
    reverse_iterator(T* p){
      ptr = p;
    }
    T& operator*(){
      return *ptr;
    } 
    reverse_iterator& operator++(){
      --ptr;
      return *this;
    }
    reverse_iterator& operator--(){
      ++ptr;
      return *this;
    }
    reverse_iterator& operator=(const reverse_iterator& it){
      return reverse_iterator(it.ptr);
    }
    bool operator==(const reverse_iterator& it){
      return (ptr == it.ptr);
    }
    bool operator!=(const reverse_iterator& it){
      return (ptr != it.ptr);
    }
};


  class const_iterator{
   public:
    T* ptr;
    const_iterator();
    ~const_iterator();
    const_iterator(T* p){
      ptr = p;
    }
    T& operator*() const{
      return *ptr;
    } 
    bool operator==(const const_iterator& it){
      return (ptr == it.ptr);
    }
    bool operator!=(const const_iterator& it){
      return (ptr != it.ptr);
    }
};


  class const_reverse_iterator{
   public:
    T* ptr;
    const_reverse_iterator();
    ~const_reverse_iterator();
    const_reverse_iterator(T* p){
      ptr = p;
    }
    T& operator*() const{
      return *ptr;
    } 
    bool operator==(const const_reverse_iterator& it){
      return (ptr == it.ptr);
    }
    bool operator!=(const const_reverse_iterator& it){
      return (ptr != it.ptr);
    }
  };
  
 private:
  size_t size_of_vec;
  size_t capacity_of_vec;
  size_t kMaxSize;
  T* elements;
  allocator<T> alloc;
  iterator iter;
  reverse_iterator riter;

 public:
  //  Constructs an empty container, with no elements.
  explicit Vector();

  //  Constructs a container with n elements. Each element is a copy of val.
  explicit Vector(size_t n, T& val);

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

  //  Returns a reverse iterator pointing to the last element in the vector 
  //    (i.e., its reverse beginning).
  reverse_iterator rbegin();

  //  Returns a reverse iterator pointing to the theoretical element preceding 
  //    the first element in the vector (which is considered its reverse end).
  reverse_iterator rend();

  //  Returns the number of elements in the vector.
  size_t size() const;

  //  Returns the maximum number of elements that the vector can hold.
  size_t max_size() const;

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
  void resize(size_t n);
  void resize(size_t n, T value);

  //  Returns the size of the storage space currently allocated for the vector, 
  //    expressed in terms of elements.
  //  This capacity is not necessarily equal to the vector size. It can be equal 
  //    or greater, with the extra space allowing to accommodate for growth 
  //    without the need to reallocate on each insertion.
  size_t capacity() const;

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
  void reserve(size_t n);

  //  Returns a reference to the element at position n in the vector container.
  T& operator[](size_t n) const;

  //  Returns a reference to the element at position n in the vector.
  T& at(size_t n) const;

  //  Returns a reference to the first element in the vector.
  //  Unlike member vector::begin, which returns an iterator to this same element, 
  //    this function returns a direct reference.
  T& front();

  //  Returns a reference to the last element in the vector.
  //  Unlike member vector::end, which returns an iterator just past this element, 
  //    this function returns a direct reference.
  T& back(); 

  //  Assigns new contents to the vector, replacing its current contents, and 
  //    modifying its size accordingly.
  void assign (iterator first, iterator last);

  void assign (size_t n, const T& val);

  //  Adds a new element at the end of the vector, after its current last element. 
  //    The content of val is copied (or moved) to the new element.
  //  This effectively increases the container size by one, which causes an 
  //    automatic reallocation of the allocated storage space if -and only if- 
  //    the new vector size surpasses the current vector capacity.
  void push_back(const T& val);

  //  Removes the last element in the vector, effectively reducing the container 
  //    size by one.
  void pop_back();

  //  The vector is extended by inserting new elements before the element at the 
  //    specified position, effectively increasing the container size by the
  //    number of elements inserted.
  iterator insert (iterator position, const T& val);
  void insert (iterator position, size_t n, const T& val);
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
  allocator<T> get_allocator() const;
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
