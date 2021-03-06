#ifndef MTMPARKINGLOT_UNIQUEARRAYIMP_H
#define MTMPARKINGLOT_UNIQUEARRAYIMP_H

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
            data[i] = new_element ;
            current_occupation++;
        }
    }
}

template<class Element, class Compare>
UniqueArray<Element, Compare>::~UniqueArray() {
    for (int i=0;i<size;i++){
        delete data[i];
    }
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
        delete new_element;
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
Element *
UniqueArray<Element, Compare>::getElementbyIndex(unsigned int index) const {
    return this->data[index];
}
#endif //MTMPARKINGLOT_UNIQUEARRAYIMP_H
