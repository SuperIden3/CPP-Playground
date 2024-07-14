// imports
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdarg>
#include <memory>
#include <iomanip>
using namespace std;

// object-oriented
/**
 * Creates a Node that can point to or be pointed by another Node, and even both.
 * 
```cpp
int a = 10 ;
Node *node_a = new Node(&a);

int b = 5 ;
Node *node_b = new Node(&b);

node_a -> next = node_b;
node_b -> next = node_a;

std::cout << *(int *)(node_a -> data) << std::endl;
std::cout << *(int *)(node_b -> data) << std::endl;
```
 */
struct Node {
    void *data;
    Node *next;
    Node(void *data) : data(data), next(nullptr) {}
};

// functions
/**
 * For `printf`ing to `stderr`.
 */
void error_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

/**
 * Prints debug messages when `-DDEBUG` is added.
 */
void dprintf(const char *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
#endif
}

/**
 * Frees a pointer.
 * @note `pointer` must be in this format: `(void**)&<pointer-name>`.
 */
void custom_free(void **pointer) {
    if (pointer == NULL || *pointer == NULL) {
        dprintf("%p is NULL.\n", pointer);
        error_printf("ERROR:\n\t%p = 0x0;\tpointer is NULL.\n", pointer);
        exit(EXIT_FAILURE);
    }
    dprintf("DEBUG: Pointer %p will be freed...\n", pointer);
    free(*pointer);
    *pointer = NULL;
    dprintf("DEBUG: Value of %p is freed and is set to NULL.\n", pointer);
}

/**
 * Prints a pointer with its contents.
 * @param ptr Pointer that points to a set of hex values.
 * @param size Number of bytes to print.
 */
void memory_printf(const void *ptr, size_t size) {
    const unsigned char *byte_ptr = static_cast<const unsigned char *>(ptr);
    for (size_t i = 0; i < size; ++i) {
        if (i % 16 == 0) {
            std::cout << '\n' << static_cast<const void *>(byte_ptr + i) << ": ";
        }
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte_ptr[i]) << " ";
    }
    std::cout << std::dec << '\n';
}

/**
 * Generates a random decimal between 0 (inclusive) and 1 (exclusive).
 */
double custom_rand() {
    int rand1 = std::rand();
    dprintf("DEBUG: Randomly generated number: %i.\n", rand1);
    double rand2 = static_cast<double>(rand1) / static_cast<double>(RAND_MAX);
    dprintf("DEBUG: Random number from 0 to 1: %.54lf.\n", rand2);
    return rand2;
}

int main() {
    const std::clock_t start = std::clock();

    std::srand(std::time(nullptr));

    // main
    printf("Name: ");
    std::unique_ptr<std::string> name = std::make_unique<std::string>("");
    if(std::getline(std::cin, *name)) {
        std::cout << "Hello, " << *name << "!\n";
    } else {
        error_printf("Error reading input.\n");
        return -1;
    }

    std::cout << std::endl;
    const std::clock_t end = std::clock();
    double performance = double(end - start) / CLOCKS_PER_SEC;
    printf("Code: %.6lfs", performance);
    std::cout << std::endl;
    return 0;
}
