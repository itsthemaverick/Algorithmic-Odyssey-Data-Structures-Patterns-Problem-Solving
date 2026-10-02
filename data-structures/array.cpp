#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <sstream>

/**
 * @brief A production-grade, generic dynamic array implementation.
 * 
 * @tparam T The type of elements stored in the dynamic array.
 */
template <typename T>
class DynamicArray {
private:
    T* data;
    size_t length;
    size_t capacity;

    /**
     * @brief Resizes the internal storage array.
     * @param new_capacity New maximum capacity.
     */
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < length; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    // =========================================================================
    // Construct / Destruct / Copy & Move Semantics
    // =========================================================================

    /**
     * @brief Constructs a new Dynamic Array.
     * @param initial_capacity Base capacity allocation size.
     */
    explicit DynamicArray(size_t initial_capacity = 8)
        : data(new T[initial_capacity]), length(0), capacity(initial_capacity) {}

    /**
     * @brief Constructs a Dynamic Array from an initializer list.
     */
    DynamicArray(std::initializer_list<T> list)
        : data(new T[list.size()]), length(list.size()), capacity(list.size()) {
        size_t idx = 0;
        for (const auto& item : list) {
            data[idx++] = item;
        }
    }

    /**
     * @brief Copy Constructor (Deep Copy)
     */
    DynamicArray(const DynamicArray& other)
        : data(new T[other.capacity]), length(other.length), capacity(other.capacity) {
        for (size_t i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
    }

    /**
     * @brief Move Constructor
     */
    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), length(other.length), capacity(other.capacity) {
        other.data = nullptr;
        other.length = 0;
        other.capacity = 0;
    }

    /**
     * @brief Copy Assignment Operator
     */
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            T* new_data = new T[other.capacity];
            for (size_t i = 0; i < other.length; ++i) {
                new_data[i] = other.data[i];
            }
            delete[] data;
            data = new_data;
            length = other.length;
            capacity = other.capacity;
        }
        return *this;
    }

    /**
     * @brief Move Assignment Operator
     */
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            length = other.length;
            capacity = other.capacity;

            other.data = nullptr;
            other.length = 0;
            other.capacity = 0;
        }
        return *this;
    }

    /**
     * @brief Destructor
     */
    ~DynamicArray() {
        delete[] data;
    }

    // =========================================================================
    // Capacity & Status Inspections
    // =========================================================================

    [[nodiscard]] size_t size() const noexcept { return length; }
    [[nodiscard]] size_t get_capacity() const noexcept { return capacity; }
    [[nodiscard]] bool empty() const noexcept { return length == 0; }

    /**
     * @brief Shrinks capacity to match current length to save memory.
     */
    void shrink_to_fit() {
        if (capacity > length && length > 0) {
            resize(length);
        }
    }

    // =========================================================================
    // Element Access
    // =========================================================================

    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    T& at(size_t index) {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds!");
        }
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds!");
        }
        return data[index];
    }

    T& front() {
        if (empty()) throw std::underflow_error("Array is empty!");
        return data[0];
    }

    T& back() {
        if (empty()) throw std::underflow_error("Array is empty!");
        return data[length - 1];
    }

    // =========================================================================
    // Insertion & Deletion Operations
    // =========================================================================

    void push_back(const T& value) {
        if (length == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[length++] = value;
    }

    void push_back(T&& value) {
        if (length == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[length++] = std::move(value);
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Array is empty!");
        --length;
    }

    void insert(size_t index, const T& value) {
        if (index > length) {
            throw std::out_of_range("Index out of bounds for insertion!");
        }
        if (length == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        for (size_t i = length; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = value;
        ++length;
    }

    void remove_at(size_t index) {
        if (index >= length) {
            throw std::out_of_range("Index out of bounds for removal!");
        }
        for (size_t i = index; i < length - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --length;
    }

    void clear() noexcept {
        length = 0;
    }

    // =========================================================================
    // Search Algorithms
    // =========================================================================

    /**
     * @brief Linear Search algorithm.
     * @return Index of match, or -1 if non-existent.
     */
    int linear_search(const T& target) const {
        for (size_t i = 0; i < length; ++i) {
            if (data[i] == target) return static_cast<int>(i);
        }
        return -1;
    }

    /**
     * @brief Binary Search algorithm (Requires sorted array).
     * @return Index of match, or -1 if non-existent.
     */
    int binary_search(const T& target) const {
        int low = 0;
        int high = static_cast<int>(length) - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (data[mid] == target) return mid;
            if (data[mid] < target) low = mid + 1;
            else high = mid - 1;
        }
        return -1;
    }

    // =========================================================================
    // Sorting Algorithms
    // =========================================================================

    /**
     * @brief Quicksort implementation (In-place).
     */
    void sort(std::function<bool(const T&, const T&)> comp = std::less<T>()) {
        if (length > 1) {
            quicksort(0, static_cast<int>(length) - 1, comp);
        }
    }

private:
    void quicksort(int low, int high, const std::function<bool(const T&, const T&)>& comp) {
        if (low < high) {
            int p = partition(low, high, comp);
            quicksort(low, p - 1, comp);
            quicksort(p + 1, high, comp);
        }
    }

    int partition(int low, int high, const std::function<bool(const T&, const T&)>& comp) {
        T pivot = data[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (comp(data[j], pivot)) {
                ++i;
                std::swap(data[i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        return i + 1;
    }

public:
    /**
     * @brief Reverse the elements inside the array.
     */
    void reverse() noexcept {
        size_t start = 0;
        size_t end = length == 0 ? 0 : length - 1;
        while (start < end) {
            std::swap(data[start++], data[end--]);
        }
    }

    // =========================================================================
    // High-Order Functional Utilities (Map, Filter, Reduce)
    // =========================================================================

    template <typename R>
    DynamicArray<R> map(std::function<R(const T&)> transformer) const {
        DynamicArray<R> result(length);
        for (size_t i = 0; i < length; ++i) {
            result.push_back(transformer(data[i]));
        }
        return result;
    }

    DynamicArray<T> filter(std::function<bool(const T&)> predicate) const {
        DynamicArray<T> result;
        for (size_t i = 0; i < length; ++i) {
            if (predicate(data[i])) {
                result.push_back(data[i]);
            }
        }
        return result;
    }

    template <typename R>
    R reduce(R initial, std::function<R(const R&, const T&)> accumulator) const {
        R acc = initial;
        for (size_t i = 0; i < length; ++i) {
            acc = accumulator(acc, data[i]);
        }
        return acc;
    }

    // =========================================================================
    // Iterators Support (Range-based For Loop Support)
    // =========================================================================

    T* begin() noexcept { return data; }
    T* end() noexcept { return data + length; }
    const T* begin() const noexcept { return data; }
    const T* end() const noexcept { return data + length; }

    // =========================================================================
    // Utilities
    // =========================================================================

    std::string to_string() const {
        std::ostringstream ss;
        ss << "[";
        for (size_t i = 0; i < length; ++i) {
            ss << data[i];
            if (i + 1 < length) ss << ", ";
        }
        ss << "]";
        return ss.str();
    }
};

// =========================================================================
// Demonstrative Main Execution
// =========================================================================

int main() {
    std::cout << "--- 1. Initialization & Push operations ---\n";
    DynamicArray<int> arr = {5, 2, 8, 1, 9};
    arr.push_back(3);
    std::cout << "Array contents: " << arr.to_string() << "\n";
    std::cout << "Size: " << arr.size() << " | Capacity: " << arr.get_capacity() << "\n\n";

    std::cout << "--- 2. Insertion & Removal ---\n";
    arr.insert(2, 99); // Insert 99 at index 2
    std::cout << "After insertion at index 2: " << arr.to_string() << "\n";
    arr.remove_at(0); // Remove element at index 0
    std::cout << "After removing index 0: " << arr.to_string() << "\n\n";

    std::cout << "--- 3. Sorting & Searching ---\n";
    arr.sort(); // Ascending sort
    std::cout << "Sorted Array: " << arr.to_string() << "\n";

    int target = 8;
    int idx = arr.binary_search(target);
    std::cout << "Binary search for element (" << target << "): found at index " << idx << "\n\n";

    std::cout << "--- 4. Functional Operators (Map, Filter, Reduce) ---\n";
    // Filter even numbers
    auto evens = arr.filter([](const int& x) { return x % 2 == 0; });
    std::cout << "Filtered (Evens): " << evens.to_string() << "\n";

    // Map: square elements
    auto squares = arr.map<int>([](const int& x) { return x * x; });
    std::cout << "Mapped (Squared): " << squares.to_string() << "\n";

    // Reduce: calculate sum
    int sum = arr.reduce<int>(0, [](const int& acc, const int& val) { return acc + val; });
    std::cout << "Reduced (Sum): " << sum << "\n\n";

    std::cout << "--- 5. Range-based Loop ---\n";
    std::cout << "Iterating: ";
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    return 0;
}
