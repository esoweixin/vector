#include "vector.h"

template <typename T>
Vector<T>::Vector(){
  size_of_vec = 0;
  capacity_of_vec = 0;
  elements = alloc.allocate();
  kMaxSize = (size_t(-1) / sizeof(T) - 1);
}

template <typename T>
Vector<T>::Vector(size_t n, T& val){
  kMaxSize = (size_t(-1) / sizeof(T) - 1);
  size_of_vec = n;
  capacity_of_vec = 1;
  for (size_t i = 0; capacity_of_vec < n; ++i)
  {
    capacity_of_vec *= 2;
  }
  elements = alloc.allocate(capacity_of_vec);
  for (size_t i = 0; i < n; ++i)
  {
    elements[i] = val;
  }
}

template <typename T>
Vector<T>::Vector(const Vector& ant_vec){
  kMaxSize = (size_t(-1) / sizeof(T) - 1);
  size_of_vec = ant_vec.size();
  capacity_of_vec = ant_vec.capacity();
  elements = alloc.allocate(capacity_of_vec);
  for (size_t i = 0; i < size_of_vec; ++i){
    elements[i] = ant_vec[i];
  }
}

template <typename T>
Vector<T>::Vector(iterator first, iterator last){
  kMaxSize = (size_t(-1) / sizeof(T) - 1);
  size_t n = 0;
  iterator it = first;
  do{
    ++n;
    ++it;
  } while(it != last);
  size_of_vec = n;
  capacity_of_vec = 1;
  for (size_t i = 0; capacity_of_vec < n; ++i){
    capacity_of_vec *= 2;
  }
  elements = alloc.allocate(capacity_of_vec);
  size_t i = 0;
  do{
    elements[i] = *first;
    ++first;
  } while(first != last);
}

template <typename T>
Vector<T>::~Vector(){
  alloc.deallocate(elements, capacity_of_vec);
}

template <typename T>
const Vector<T>& Vector<T>::operator=(const Vector& ant_vec){
  if (this != &ant_vec){
    *this = Vector(ant_vec);
  }
  return *this;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin(){
  return iterator(elements);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end(){
  return iterator(elements + size_of_vec - 1);
}

template <typename T>
typename Vector<T>::reverse_iterator Vector<T>::rbegin(){
  return reverse_iterator(elements + size_of_vec - 1);
}

template <typename T>
typename Vector<T>::reverse_iterator Vector<T>::rend(){
  return reverse_iterator(elements);
}

template <typename T>
size_t Vector<T>::size() const{
  return size_of_vec;
}

template <typename T>  
size_t Vector<T>::max_size() const{
  return kMaxSize;
}

template <typename T>
void Vector<T>::resize(size_t n){
  if(n <= capacity_of_vec){
    for (size_t i = size_of_vec; i < n; ++i)
    {
      elements[i] = T();
    }
  } else{
    T* old = elements;
    while(capacity_of_vec < n){
      capacity_of_vec *= 2; 
    }
    elements = alloc.allocate(capacity_of_vec);
    for(size_t i = 0; i < size_of_vec; ++i){
      elements[i] = old[i];
    }
    for (size_t i = size_of_vec; i < n; ++i)
    {
      elements[i] = T();
    }
  }
  size_of_vec = n;
}
  
template <typename T>  
void Vector<T>::resize(size_t n, T value){
  if(n <= capacity_of_vec){
    for (size_t i = size_of_vec; i < n; ++i)
    {
      elements[i] = value;
    }
  } else{
    T* old = elements;
    while(capacity_of_vec < n){
      capacity_of_vec *= 2; 
    }
    elements = alloc.allocate(capacity_of_vec);
    for(size_t i = 0; i < size_of_vec; ++i){
      elements[i] = old[i];
    }
    for (size_t i = size_of_vec; i < n; ++i)
    {
      elements[i] = value;
    }
    alloc.deallocate(elements, capacity_of_vec);
  }
  size_of_vec = n;
}

template <typename T>
size_t Vector<T>::capacity() const{
  return capacity_of_vec;
}

template <typename T>
bool Vector<T>::empty() const{
  if (size_of_vec == 0)
  {
    return true;
  }
  return false;
}

template <typename T>
void Vector<T>::reserve(size_t n){
  if(n <= capacity_of_vec)
      return;
  T* old = elements;
  while(capacity_of_vec < n)
    capacity_of_vec *= 2;
  elements = alloc.allocate(capacity_of_vec);
  for(size_t i = 0; i < size_of_vec; ++i){
    elements[i] = old[i];
  }
  alloc.deallocate(elements, capacity_of_vec);
}

template <typename T>
T& Vector<T>::operator[](size_t n) const{
  return *(elements + n);
}

template <typename T>
T& Vector<T>::at(size_t n) const{
  return *(elements + n);
}

template <typename T>
T& Vector<T>::front(){
  return *elements;
}

template <typename T>
T& Vector<T>::back(){
  return *(elements + size_of_vec + 1);
}

template <typename T>
void Vector<T>::assign (size_t n, const T& val){
  for (size_t i = 0; i < n; ++i){
    elements[i] = val;
  }
  size_of_vec = n;
}

template <typename T>
void Vector<T>::assign (iterator first, iterator last){
  int n = 0;
  for (iterator it = first; it != last; ++it){
    elements[n] = *it;
    ++n;
  }
  size_of_vec = n;
}

template <typename T>
void Vector<T>::push_back(const T& val){
  if (size_of_vec < capacity_of_vec)
  {
    elements[size_of_vec] = val;
    ++size_of_vec;
  } else{
    T* old = elements;
    capacity_of_vec *= 2;
    elements = alloc.allocate(capacity_of_vec);
    for(size_t i = 0; i < size_of_vec; ++i){
      elements[i] = old[i];
    }
    elements[size_of_vec] = val;
    ++size_of_vec;
    alloc.deallocate(old, capacity_of_vec);
  }
}

template <typename T>
void Vector<T>::pop_back(){
  --size_of_vec;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert (iterator position, const T& val){
  if (size_of_vec == capacity_of_vec){
    T* old = elements;
    capacity_of_vec *= 2;
    elements = alloc.allocate(capacity_of_vec);
    for(size_t i = 0; i < size_of_vec; ++i){
      elements[i] = old[i];
    }
    alloc.deallocate(old, capacity_of_vec);
  }
  for(iterator it = (*this).end(); it != position; --it){
    *(it + 1) = *it;
  }
  *position = val;
  ++size_of_vec;
  return position;
}

template <typename T>
void Vector<T>::insert (iterator position, size_t n, const T& val){
  T* old = elements;
  if (size_of_vec + n > capacity_of_vec){
    while(capacity_of_vec < size_of_vec + n){
      capacity_of_vec *= 2;
    } 
  }
  elements = alloc.allocate(capacity_of_vec);
  int i = 0;
  for(iterator it = (*this).begin(); it != position; ++it){
    elements[i] = old[i];
    ++i;
  }
  for(size_t j = 0; j < n; ++j){
    elements[i + j] = val;
  }
  for(size_t j = i + n; j < size_of_vec + n; ++j){
    elements[j] = old[j - n];
  }
  alloc.deallocate(old, capacity_of_vec);
}

template <typename T>
void Vector<T>::insert (iterator position, iterator first, iterator last){
  T* old = elements;
  int n = 0;
  for(iterator it = first; it != last; ++it){
    elements[n] = old[n];
    ++n;
  }
  if (size_of_vec + n > capacity_of_vec){
    while(capacity_of_vec < size_of_vec + n){
      capacity_of_vec *= 2;
    } 
  }
  int i = 0;
  for(iterator it = (*this).begin(); it != position; ++it){
    elements[i] = old[i];
    ++i;
  }
  for(iterator it = first; it != last; ++it){
    elements[i] = *it;
    ++i;
  }
  for(size_t j = i; j < size_of_vec + n; ++j){
    elements[j] = old[j - n];
  }
  alloc.deallocate(old, capacity_of_vec);
  size_of_vec = size_of_vec + n;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase (iterator position){
  for(iterator it = position; it != (*this).end(); ++it){
    *(it) = *(it + 1);
  }
  --size_of_vec;
  return position;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase (iterator first, iterator last){
  int n = 0;
  for(iterator it = first; it!= last; ++it){
    *it = *(last + n);
    ++n;
  }
  size_of_vec += n;
  return first;
}

template <typename T>
void Vector<T>::swap(Vector& x){
  Vector<T>* p_tmp = Vector<T>(x);
  x = *this;
  *this = *p_tmp;
  delete p_tmp;
}

template <typename T>
void Vector<T>::clear(){
  T* old = elements;
  size_of_vec = 0;
  capacity_of_vec = 0;
  elements = alloc.allocate(capacity_of_vec);
  alloc.deallocate(old, capacity_of_vec);
}

template <typename T>
allocator<T> Vector<T>::get_allocator() const{
  return alloc;
}

template <typename T>
bool operator== (const Vector<T>& lhs, const Vector<T>& rhs){
  if (lhs.size() != rhs.size())  return false;
  size_t size = lhs.size();
  for(size_t i = 0; i < size; ++i){
    if(lhs[i] != rhs[i])  return false;
  }
  return true;
}

template <typename T>
bool operator!= (const Vector<T>& lhs, const Vector<T>& rhs){
  if (lhs.size() != rhs.size())  return true;
  for(size_t i = 0; i < lhs.size(); ++i){
    if(lhs[i] != rhs[i])  return true;
  }
  return false;
}
 
template <typename T> 
bool operator<  (const Vector<T>& lhs, const Vector<T>& rhs){
  size_t n = (lhs.size() > rhs .size()) ? lhs.size() : rhs.size();
  for(size_t i = 0; i < n; ++i){
    if(lhs[i] < rhs[i]) return true;
    if(lhs[i] > rhs[i]) return false;
  }
  if(lhs.size() < rhs.size()) return true;
  return false;
}

template <typename T>
bool operator>  (const Vector<T>& lhs, const Vector<T>& rhs){
  size_t n = (lhs.size() > rhs .size()) ? lhs.size() : rhs.size();
  for(size_t i = 0; i < n; ++i){
    if(lhs[i] < rhs[i]) return false;
    if(lhs[i] > rhs[i]) return true;
  }
  if(lhs.size() > rhs.size()) return true;
  return false;
}
 
template <typename T> 
bool operator<= (const Vector<T>& lhs, const Vector<T>& rhs){
  return !(lhs > rhs);
}
 
template <typename T> 
bool operator>= (const Vector<T>& lhs, const Vector<T>& rhs){
  return !(lhs < rhs);
}

template <typename T>
void swap (Vector<T>& x, Vector<T>& y){
  Vector<T>* p_tmp = Vector<T>(x);
  x = y;
  y = *p_tmp;
  delete p_tmp;
}
