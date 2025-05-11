/**
 * @file 2DArray.hpp
 * @author Joel Height (On3SnowySnowman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 04-29-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once


#include <array>
#include <cstdint>


/**
 * @brief A fixed-size 2D array class template.
 * 
 * Provides a simple interface for managing a 2D array with compile-time 
 * dimensions. Supports direct access and bounds-checked access to elements.
 * 
 * @tparam T The type of elements stored in the array.
 * @tparam N The size of each dimension (NxN array).
 */
template<typename T, uint64_t N>
class Array2D
{

public:

    Array2D() {}

    typename std::array<std::array<T, N>, N>::iterator begin() 
        { return content.begin(); }

    typename std::array<std::array<T, N>, N>::iterator end()
        { return content.end(); }

    constexpr uint64_t size() const noexcept { return N; }
    
    /**
     * @brief Retrieves a reference to the value at the specified location with 
     * bounds checking.
     * 
     * This function performs bounds checking. If the indices are out of 
     * bounds, it will throw an std::out_of_range exception.
     * 
     * @param row The row index (y-coordinate).
     * @param column The column index (x-coordinate).
     * @return T& Reference to the value at the specified location.
     * 
     */
    T& at(uint64_t row, uint64_t column) 
        { return content.at(row).at(column); }

    /**
     * @brief Provides a reference to the first row of the 2D array.
     * 
     * This function returns a reference to the first row of the underlying 
     * 2D array. It can be used to access or manipulate the first row directly.
     * 
     * @return std::array<T, N>* Pointer to the first row of the 2D array.
     * 
     */
    std::array<T, N>& front() { return content.front(); }

private:

    // Underlying 2D array that stores the values. 
    std::array<std::array<T, N>, N> content {};

};


