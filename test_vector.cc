#include "C++_vector.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

template <typename T>
void printVector(Vector<T>& vector){
	size_t size = vector.size();
	size_t capacity = vector.capacity();
	cout << "The size is " << size << endl;
	cout << "The capacity is " << capacity << endl;
	cout << "The max size is " << vector.max_size() << endl;
	cout << "And its contents are:" << endl;
	for(int i = 0; i < size; ++i)
		cout << vector[i] << endl;
	cout << "End." << endl;
}

int main(){
	Vector<int> v;
	printVector(v);
	
	for(int i = 0; i < 20; ++i){
		v.push_back(i);
	}
	printVector(v);
	
	v.insert(v.begin(), 10, 2);
	printVector(v);

	std::sort(v.begin(), v.end());
	v.erase(&v[5]);
	printVector(v); 
	
	v.pop_back();
	printVector(v);
	
	Vector<int> v1;
	v.swap(v1);
	printVector(v);
	
	v = v1;
	printVector(v);
	
	cout << (v == v1) << ' ' << (v < v1) << endl;
	v.resize(10);
	printVector(v);
	
	v.reserve(16);
	printVector(v);
	
	v.clear();
	printVector(v);
	return 0;
} 
