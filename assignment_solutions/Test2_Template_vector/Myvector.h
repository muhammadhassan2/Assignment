#ifndef MYVECTOR_H_INCLUDED
#define MYVECTOR_H_INCLUDED
#include<iostream>
#include<bits/stdc++.h>
using namespace std;
#define start_capacity 10
template<class Type>
class Myvector{
  private:
        size_t vec_size; //define the current size of the vector
        size_t vec_capacity; // define the total capacity of the vector
        unique_ptr<Type[]> vec_data;// define the values of the vector in array

  public:
      Myvector():vec_size{0},vec_capacity{start_capacity},vec_data{nullptr}{
           /*
           define a default constractor to set value of the calss usign initalization list
            making vec_size=0 and the first inndex of vector
            make the capacityy of vector with 10;
            allocate vec_data wit te capacit size
          */
           vec_data = std::make_unique<Type[]>(start_capacity);

               }
      /*
         provide a one-args costractor to set the capacity of the vector
         useing explicit keyword to prevent class assignmet
         eg   Myvector<int> v=20; set sz=20 so explicit prevent it
      */
      explicit Myvector(size_t sz):vec_size{0},vec_capacity{sz}{

              vec_data = std::make_unique<Type[]>(sz);

        }
        void pr(){
        cout<<vec_capacity<<endl;
        }

      ~Myvector(){
      }

  public:
    void increase_buffer(size_t n); // function used to increase the size of the array
    void push_back(Type value);  // insert value at the end of vector
    template <typename... Args>
    void emplace_back(Args&&... args); // insert value at the end of the vector
    Type &operator[](size_t index); // Overloading Subscript operator to retrieve the value at some index
    const Type&operator[](size_t index)const; // Overloading Subscript operator to retrieve the value at some index but with const object



     Myvector & operator=(Myvector&& other)  { // declare move assignment operator to assign objects with an R-value objects
         cout<<"move assignment operator is called \n";
        if (this != &other) { // if the current object not equal the r-value object
            vec_data = std::move(other.vec_data); // move other object resources to the current object
            vec_size = other.vec_size;
            vec_capacity = other.vec_capacity;
            other.vec_size =0;
            other.vec_capacity = 0;
        }
        return *this;
    }
};
template<class Type>
const Type&Myvector<Type>::operator[](size_t index)const{
   if(index>=vec_size){ // if the index out of range throw an exception
     throw out_of_range("Index out of range");
   }
   return vec_data[index];

}
template<class Type>
Type&Myvector<Type>::operator[](size_t index){
   if(index>=vec_size){// if the index out of range throw an excepti
     throw out_of_range("Index out of range");
   }
   return vec_data[index]; // return the value

}
template<class Type>
void Myvector<Type>::push_back(Type value){
          if(vec_size>=vec_capacity){ // if vec_size reaches the capacity size we need to increase it
            increase_buffer(vec_capacity*2);
          }
          else{
             vec_data[vec_size]=value;
              vec_size++;
          }


}
template<class Type>
template <class... Args>
void Myvector<Type>::emplace_back(Args&&... args) {
    // Check if the current size exceeds the capacity of the vector.
   if (vec_size >= vec_capacity) {
       increase_buffer(2 * vec_capacity); // double the capacity of the vector
   }
   // construct the new element in place at the end of the vector using perfect forwarding.
   new (&vec_data[vec_size]) Type(std::forward<Args>(args)...); // construct the new element in place
   vec_size++; // increment the size of the vector
}

template<class Type>
void Myvector<Type>::increase_buffer(size_t new_size){
    // increase the buffer size to maintain increasing of the values
                  if(new_size>vec_capacity){
                        // create temprary array to store the data from object
                   auto temp_buffer = std::make_unique<Type[]>(vec_capacity);
                    for (size_t i = 0; i < vec_size; i++)
		              	temp_buffer[i] = move(vec_data[i]); // using move to move resources instead of copying it
                  // copy data from the new array to the old array
                  vec_data = move(temp_buffer);
                  vec_capacity = new_size; // assign capacity th the new size
                  }
}


#endif // MYVECTOR_H_INCLUDED
