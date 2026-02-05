#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include "array_ptr.h"
using namespace std;
template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size):items_(size),size_(size),capacity_(size) {
        fill(begin(),end(),Type{});

    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value):items_(size), size_(size),capacity_(size){
        fill(begin(),begin()+size,value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init):items_(init.size()),size_(init.size()),capacity_(init.size()){
        if(init.size() >0){
             copy(init.begin(),init.end(),begin());
        }

    }

    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return (size_==0);
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if(index >= size_){
            throw std::out_of_range("index out of range");
        }
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if(index >= size_){
            throw std::out_of_range("index out of range");
        }
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_=0;
    }
    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if(new_size==size_){
            return;
        }
        if(new_size < size_){
                size_ = new_size;
        }else{
        ArrayPtr<Type> new_items(new_size);        
        fill(new_items.Get(),new_items.Get()+new_size,Type{});
        copy(begin(),begin()+size_,new_items.Get());
        items_.swap(new_items);
        capacity_=max(new_size,capacity_*2);
        size_=new_size;

        }
    }

    Iterator begin() noexcept {
        return size_ == 0 ? nullptr : items_.Get();
    }

    Iterator end() noexcept {
        return size_ == 0 ? nullptr : items_.Get()+size_;

    }

    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    ConstIterator end() const noexcept {
        return size_ == 0 ? nullptr : items_.Get()+size_;
    }

    ConstIterator cbegin() const noexcept {
        return size_ == 0 ? nullptr : items_.Get();
    }

    ConstIterator cend() const noexcept {
        return size_ == 0 ? nullptr : items_.Get()+size_;
    }

    SimpleVector(const SimpleVector& other) {        
        std::copy(other.begin(),other.end(),begin());
        size_=other.size_;
        capacity_=other.capacity_;

     }

     SimpleVector& operator=(const SimpleVector& rhs) {
         SimpleVector s_copy(rhs);
         swap(s_copy);
         return *this;
     }

     // Добавляет элемент в конец вектора
     // При нехватке места увеличивает вдвое вместимость вектора
     void PushBack(const Type& item) {
         if(size_ < capacity_){
             items_[++size_]=item;
         }else{
             Resize(size_+1);
             items_[size_-1]=item;
         }
     }

     // Вставляет значение value в позицию pos.
     // Возвращает итератор на вставленное значение
     // Если перед вставкой значения вектор был заполнен полностью,
     // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
     Iterator Insert(ConstIterator pos, const Type& value) {
         int n=0;
         ArrayPtr<Type> new_items(size_+1);
         for(auto i=begin(); i !=end(); ++i){
             if(i==pos){
                 new_items[n]=value;
                 ++n;
             }
             new_items[n]=*i;
             ++n;
         }
         items_.swap(new_items);
         ++size_;
         if(size_ > capacity_){

             capacity_=capacity_*2;

         }
     }

     void PopBack() noexcept {
         if(size_ ==0){
             return;
         }
         --size_;

     }

     // Удаляет элемент вектора в указанной позиции
     Iterator Erase(ConstIterator pos) {
         ArrayPtr<Type> new_items(size_-1);
         int n=0;
         bool second_part=false;
         for(auto i=begin(); i !=end(); ++i){
             if(i==pos){
                 second_part=true;
                 continue;
             }
             if(!second_part){
                 new_items[n]=*i;
             }else{
                 new_items[n]=*i ;
             }
             ++n;

         }
         items_.swap(new_items);
         --size_;
     }

     // Обменивает значение с другим вектором
     void swap(SimpleVector& other) noexcept {
         items_.swap(other.items_);
         std::swap(size_,other.size_);
         std::swap(capacity_,other.capacity_);
     }

private:
    ArrayPtr<Type> items_;
    size_t size_=0;
    size_t capacity_=0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return true;
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return true;
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return true;
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return true;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return true;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return true;
}
