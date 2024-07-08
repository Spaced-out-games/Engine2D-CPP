#pragma once
#include <cstddef>
#include <initializer_list>
#include <vector>
#include <type_traits>
#include "GUI_Node.h"

template <typename T>
struct basic_array {
    basic_array();
    basic_array(size_t size);
    basic_array(const std::vector<T>& vec);
    basic_array(std::initializer_list<T> init_list);
    basic_array(T* data, size_t size);
    ~basic_array();

 
    T& get(size_t index) const;
    void set(size_t index, T new_element);
    const T* c_array() const;
    size_t len() const;
private:
    T* data;
    size_t mLength;
    
    void initialize_elements();
};

template <typename T>
basic_array<T>::basic_array() : data(nullptr), mLength(0) {}

template <typename T>
basic_array<T>::basic_array(size_t size) : data(new T[size]), mLength(size) {
    initialize_elements();
}

template <typename T>
basic_array<T>::basic_array(const std::vector<T>& vec) : data(new T[vec.size()]), mLength(vec.size()) {
    std::copy(vec.begin(), vec.end(), data);
}

template <typename T>
basic_array<T>::basic_array(std::initializer_list<T> init_list) : data(new T[init_list.size()]), mLength(init_list.size()) {
    std::copy(init_list.begin(), init_list.end(), data);
}

template <typename T>
basic_array<T>::basic_array(T* data, size_t size) : data(new T[size]), mLength(size) {
    std::copy(data, data + size, this->data);
}

template <typename T>
basic_array<T>::~basic_array() {
    delete[] data;
}

template <typename T>
T& basic_array<T>::get(size_t index) const {
    return data[index];
}

template <typename T>
void basic_array<T>::set(size_t index, T new_element) {
    data[index] = new_element;
}

template <typename T>
const T* basic_array<T>::c_array() const {
    return data;
}

template <typename T>
size_t basic_array<T>::len() const {
    return mLength;
}

template <typename T>
void basic_array<T>::initialize_elements() {
    if constexpr (std::is_pointer<T>::value) {
        for (size_t i = 0; i < mLength; ++i) {
            data[i] = nullptr;
        }
    }
    else {
        for (size_t i = 0; i < mLength; ++i) {
            data[i] = T();
        }
    }
}