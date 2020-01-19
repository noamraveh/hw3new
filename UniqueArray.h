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

    Element * getElementbyIndex(unsigned int index) const;


    class Filter {
    public:
        virtual bool operator()(const Element &) const = 0;
    };

    UniqueArray filter(const Filter &f) const;

    class UniqueArrayIsFullException {
    };

};

#include "UniqueArrayImp.h"

#endif //MTMPARKINGLOT_UNIQUEARRAY_H
