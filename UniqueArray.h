#ifndef MTMPARKINGLOT_UNIQUEARRAY_H
#define MTMPARKINGLOT_UNIQUEARRAY_H

#include <iostream>

template<class Element, class Compare = std::equal_to<Element>>
class UniqueArray {
    Element **data;
    int size;
    int next_index; //one index above highest occupied index
    int current_occupation; //amount of occupied cells
    Compare compareFunc = Compare();
public:

    UniqueArray(unsigned int size);

    UniqueArray(const UniqueArray &other);

    ~UniqueArray();

    int elementExists(const Element &element) const;

    UniqueArray &operator=(const UniqueArray &) = delete;

    unsigned int insert(const Element &element);

    bool getIndex(const Element &element, unsigned int &index) const;

    const Element *operator[](const Element &element) const;

    bool remove(const Element &element);

    unsigned int getCount() const;

    unsigned int getSize() const;

    const Element* getFirst() const;

    const Element** getData() const;

    class Filter {
    public:
        virtual bool operator()(const Element &) const = 0;
    };

    UniqueArray filter(const Filter &f) const;

    class UniqueArrayIsFullException {
    };

};

#include "UniqueArrayImp.h"


template<class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(unsigned int size) :
        data(new Element *[size]), size(size), next_index(0),
        current_occupation(0) {
    for (int i = 0; i < size; i++) {
        data[i] = NULL;
    }
}

template<class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(const UniqueArray &other) :
        data(new Element *[other.size]), size(other.size), next_index(0),
        current_occupation(0) {
    for (int i = 0; i < other.next_index; i++) { //copies only non NULL elements
        next_index++;
        if (other.data[i] == NULL) {
            data[i] = NULL;
            continue;
        } else {
            Element *new_element = new Element(*(other.data[i]));
            data[i] = new_element;
            current_occupation++;
        }
    }
}

template<class Element, class Compare>
UniqueArray<Element, Compare>::~UniqueArray() {
    delete[] data;
}

template<class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::insert(const Element &element) {
    //create copy of original element
    Element *new_element = new Element(element);

    int index = elementExists(*new_element);
    if (index >= 0) { // element already exists
        delete new_element;
        return index;
    }


    for (int i = 0; i < next_index; i++) { //check for earlier available cell
        if (data[i] == NULL) {
            data[i] = new_element;
            current_occupation++;
            return i;
        }
    }
    if (next_index >= size) {
        throw UniqueArrayIsFullException();

    }
    //add new element to array
    data[next_index++] = new_element;
    current_occupation++;
    return next_index - 1;

}

template<class Element, class Compare>
bool UniqueArray<Element, Compare>::getIndex(const Element &element,
                                             unsigned int &index) const {
    //what should we put in index if false
    int i = elementExists(element);
    if (i >= 0) {
        index = i;
        return true;
    }
    return false;
}

template<class Element, class Compare>
const Element *UniqueArray<Element, Compare>::operator[](
        const Element &element) const {
    int index = elementExists(element);
    if (index >= 0) {
        return data[index];
    }
    return NULL;
}

template<class Element, class Compare>
bool UniqueArray<Element, Compare>::remove(const Element &element) {
    int index = elementExists(element);
    if (index >= 0) {
        //change next index if removing element in current next index
        if (index == (next_index - 1)) {
            next_index--;
        }
        delete data[index];
        current_occupation--;
        data[index] = NULL;
        return true;
    }
    return false;
}

template<class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getCount() const {
    return current_occupation;
}

template<class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getSize() const {
    return size;
}

template<class Element, class Compare>
UniqueArray<Element, Compare>
UniqueArray<Element, Compare>::filter(const Filter &f) const {

    UniqueArray<Element,Compare> Filtered(*this);
    for (int i=0; i<Filtered.next_index;i++){
        if (!f(*(data[i]))){
            Filtered.remove(*data[i]);
        }
    }
    return Filtered;


}

template<class Element, class Compare>
int UniqueArray<Element, Compare>::elementExists(const Element &element) const {
    for (int i = 0; i < this->next_index; i++) {
        // understand this line
        //if ((Compare(),element) == *(this->data[i])){

        if ((this->data[i] != NULL) && compareFunc(element, *(this->data[i]))) {
            return i;
        }
    }
    return -1;
}
template<class Element, class Compare>
const Element* UniqueArray<Element,Compare>::getFirst() const {
    return this->data[0];
}

template<class Element, class Compare>
const Element** UniqueArray<Element,Compare>::getData() const {
    return this->data;
}
#endif //MTMPARKINGLOT_UNIQUEARRAY_H
