/**
 * @file SparseSet.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief Declaration for SparseSet
 * @version 0.1
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <vector>
#include <unordered_map>

template<typename T>
class SparseSet
{

public:

    SparseSet();

    /** Places the item at the back of vector. 
     * 
     * @param item 
    */
    void push_back(T item);

    /** Erases the item at the index. 
     * 
     * @param index Index to erase.
    */
    void erase(std::size_t index);

    /** Finds the index of the item in the vector. Returns 1 past the end of the vector
     * if the item was not found.
     * 
     * @param item Item to find.
    */
    std::size_t find(T& item) const;

    /** Returns a reference to the item at $index.
     * 
     * @param index Index to fetch.
     */
    T& at(std::size_t index) const;

private:

    std::unordered_map<T, std::size_t> sparse_set;

    std::vector<T> packed_set;
};
