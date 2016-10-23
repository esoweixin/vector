#include "vector.h"
#include <algorithm>

template <typename T>
typename Vector<T>::iterator Vector<T>::begin(){
  return start;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end(){
  return finish;
}

template <typename T>
size_t Vector<T>::size() const{
  return size_t(finish - start);
}

template <typename T>  
size_t Vector<T>::max_size() const{
  return size_t(-1) / sizeof(size_t);
}

template <typename T>
size_t Vector<T>::capacity() const{
  return size_t(end_of_storage - start);
}

template <typename T>
bool Vector<T>::empty() const{
  return begin() == end();
}

template <typename T>
T& Vector<T>::operator[](size_t n) const{
  return *(begin() + n);
}

template <typename T>
Vector<T>::Vector() : start(0), finish(0), end_of_storage(0) {
}

template <typename T>
Vector<T>::Vector(size_t n, value_type& val) {
  fill_initialize(n, val);
}

template <typename T>
Vector<T>::Vector(const Vector& ant_vec) {
  size_t size_of_vec = ant_vec.size();
  size_t capacity_of_vec = ant_vec.capacity();
  fill_initialize(capacity_of_vec, T(0));
  for (size_t i = 0; i < size_of_vec; ++i) {
    *(begin() + i) = ant_vec[i];
  }
}

template <typename T>
Vector<T>::Vector(typename Vector<T>::iterator first, typename Vector<T>::iterator last) {
  size_t n = size_t(last - first);
  fill_initialize(n, T(0));
  size_t i = 0;
  do{
    *(begin() + i) = *first;
    ++first;
  } while(first != last);
}

template <typename T>
Vector<T>::~Vector() {
  deallocate();
}

template <typename T>
const Vector<T>& Vector<T>::operator=(const Vector& ant_vec) {
  if (this != &ant_vec) {
    *this = Vector(ant_vec);
  }
  return *this;
}

template <typename T>
T& Vector<T>::at(size_t n) const{
  return *(begin() + n);
}

template <typename T>
T& Vector<T>::front() {
  return *begin();
}

template <typename T>
T& Vector<T>::back() {
  return *(end() - 1);
}

template <typename T>
void Vector<T>::realloc(){
  const size_t old_size = size();
  const size_t new_size = old_size != 0 ? 2 * old_size : 1;

  typename Vector<T>::iterator new_start = alloc.allocate(new_size);
  typename Vector<T>::iterator new_finish = new_start;
  typename Vector<T>::iterator new_end = new_start + new_size;
  new_finish = unintialized_copy(start, finish, new_start);

  deallocate();

  start = new_start;
  finish = new_finish;
  end_of_storage = new_end;
}

template <typename T>
void Vector<T>::push_back(const T& val) {
  if (finish != end_of_storage) {
    construct(finish, val);
    ++finish;
  }
  else {
    realloc();
    construct(finish, val);
    ++finish;
  }
}

template <typename T>
void Vector<T>::pop_back() {
  --finish;
  destroy(finish);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase (typename Vector<T>::iterator position) {
  if (position + 1 != end()) {
    copy(position + 1, finish, position);
  }
  --finish;
  destroy(finish);
  return position;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase (typename Vector<T>::iterator first, typename Vector<T>::iterator last) {
  int n = size_t(last - first);
  if (last != end())
    copy(last, finish, first);
  finish -= n;
  for (int i = 0; i < n; ++i) 
    destroy(finish + i);
  return first;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert (typename Vector<T>::iterator position, const T& val) {
  if(finish != end_of_storage) {
    copy(position, end(), position + 1);
    *position = val;
    ++finish;
  } else {
    resize();
    copy(position, end(), position + 1);
    *position = val;
    ++finish;
  }
}

template <typename T>
void Vector<T>::insert (typename Vector<T>::iterator position, size_t n, const T& val) {
  if (size() + n <= capacity()) {
    copy(position, end(), position + n);
    fill(position, position + n, val);
    finish += n;
  } else {
    while (size() + n > capacity()) {
      realloc();
    }
    copy(position, end(), position + n);
    fill(position, position + n, val);
    finish += n;
  }
}

template <typename T>
void Vector<T>::insert (typename Vector<T>::iterator position, typename Vector<T>::iterator first, typename Vector<T>::iterator last) {
  size_t n = size_t(last - first);
  if (size() + n <= capacity()) {
    copy(position, end(), position + n);
    copy(first, last, position);
    finish += n;
  } else {
    while (size() + n > capacity()) {
      realloc();
    }
    copy(position, end(), position + n);
    copy(first, last, position);
    finish += n;
  }
}

template <typename T>
void Vector<T>::resize(size_t n) {
  if (n < size()) {
    erase(begin() + n, finish);
  } else if (n > size()) {
    insert(end(), n - size(), T(0));
  }
}
  
template <typename T>  
void Vector<T>::resize(size_t n, T value) {
  if (n < size())
    erase(begin() + n, end());
  else
    insert(end(), n - size(), value);
}

template <typename T>
void Vector<T>::reserve(size_t n) {
  if(n > size())
    insert(end(), n - size(), T(0));
}

template <typename T>
void Vector<T>::swap(Vector& x) {
  Vector<T>* p_tmp = Vector<T>(x);
  x = *this;
  *this = *p_tmp;
  delete p_tmp;
}

template <typename T>
void Vector<T>::assign (size_t n, const T& val) {
  resize(n);
  for (size_t i = 0; i < n; ++i) {
    *(begin() + i) = val;
  }
}

template <typename T>
void Vector<T>::assign (typename Vector<T>::iterator first, typename Vector<T>::iterator last) {
  size_t n = size_t(last - first);
  resize(n);
  unintialized_copy(first, last, start);
}

template <typename T>
void Vector<T>::clear() {
  erase(begin(), end());
}

template <typename T>
Alloc<T> Vector<T>::get_allocator() const{
  return alloc;
}

template <typename T>
bool operator== (const Vector<T>& lhs, const Vector<T>& rhs) {
  if (lhs.size() != rhs.size())  return false;
  size_t size = lhs.size();
  for(size_t i = 0; i < size; ++i) {
    if(lhs[i] != rhs[i])  return false;
  }
  return true;
}

template <typename T>
bool operator!= (const Vector<T>& lhs, const Vector<T>& rhs) {
  if (lhs.size() != rhs.size())  return true;
  for(size_t i = 0; i < lhs.size(); ++i) {
    if(lhs[i] != rhs[i])  return true;
  }
  return false;
}
 
template <typename T> 
bool operator<  (const Vector<T>& lhs, const Vector<T>& rhs) {
  size_t n = (lhs.size() > rhs .size()) ? lhs.size() : rhs.size();
  for(size_t i = 0; i < n; ++i) {
    if(lhs[i] < rhs[i]) return true;
    if(lhs[i] > rhs[i]) return false;
  }
  if(lhs.size() < rhs.size()) return true;
  return false;
}

template <typename T>
bool operator>  (const Vector<T>& lhs, const Vector<T>& rhs) {
  size_t n = (lhs.size() > rhs .size()) ? lhs.size() : rhs.size();
  for(size_t i = 0; i < n; ++i) {
    if(lhs[i] < rhs[i]) return false;
    if(lhs[i] > rhs[i]) return true;
  }
  if(lhs.size() > rhs.size()) return true;
  return false;
}
 
template <typename T> 
bool operator<= (const Vector<T>& lhs, const Vector<T>& rhs) {
  return !(lhs > rhs);
}
 
template <typename T> 
bool operator>= (const Vector<T>& lhs, const Vector<T>& rhs) {
  return !(lhs < rhs);
}

template <typename T>
void swap (Vector<T>& x, Vector<T>& y) {
  Vector<T>* p_tmp = Vector<T>(x);
  x = y;
  y = *p_tmp;
  delete p_tmp;
}
