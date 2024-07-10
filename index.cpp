#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdarg>
#include <memory>
#include <iomanip>
using namespace std;

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) debug_printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

/**
 * @brief Prints debug messages when `-DDEBUG` is added.
 */
void debug_printf(const char *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
#endif
}

/**
 * @brief For `printf`ing to `stderr`.
 */
void error_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

/**
 * @brief Frees a pointer.
 * @note `pointer` must be in this format: `(void**)&<pointer-name>`.
 */
void custom_free(void **pointer) {
    if (pointer == NULL || *pointer == NULL) {
        DEBUG_PRINT("%p is NULL.\n", pointer);
        error_printf("ERROR:\n\t%p = 0x0;\tpointer is NULL.\n", pointer);
        exit(EXIT_FAILURE);
    }
    DEBUG_PRINT("DEBUG: Pointer %p will be freed...\n", pointer);
    free(*pointer);
    *pointer = NULL;
    DEBUG_PRINT("DEBUG: Value of %p is freed and is set to NULL.\n", pointer);
}

/**
 * @brief Prints a pointer with its contents.
 * @param ptr Pointer that points to a set of hex values.
 * @param size Number of bytes to print.
 */
void memory_printf(const void *ptr, size_t size) {
    const unsigned char *byte_ptr = static_cast<const unsigned char *>(ptr);
    for (size_t i = 0; i < size; ++i) {
        if (i % 16 == 0) {
            std::cout << '\n' << static_cast<const void *>(byte_ptr + i) << ": "; // Print address
        }
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte_ptr[i]) << " "; // Print byte value in hex
    }
    std::cout << std::dec << '\n'; // Switch back to decimal
}

/**
 * @brief Generates a random decimal between 0 (inclusive) and 1 (exclusive).
 */
double custom_rand() {
    int rand1 = std::rand();
    DEBUG_PRINT("DEBUG: Randomly generated number: %i.\n", rand1);
    double rand2 = static_cast<double>(rand1) / static_cast<double>(RAND_MAX);
    DEBUG_PRINT("DEBUG: Random number from 0 to 1: %.54lf.\n", rand2);
    return rand2;
}

int main() {
    std::srand(std::time(nullptr));

    int size = 5;
    std::unique_ptr<double[]> numbers(new double[size]);
    for (int i = 0; i < size; ++i) {
        numbers[i] = custom_rand();
    }
    std::cout << std::fixed << std::setprecision(2) << numbers[0] << '\n';

    std::cout << std::endl;
    return 0;
}