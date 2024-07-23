// imports
#include <iostream>
#include <cstdlib>
#include <functional>
#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <memory>
#include <iomanip>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
using namespace std;

// object-oriented
/**
 * Creates a Node that can point to or be pointed by another Node, and even both.
 *
```cpp
int a = 10;
Node *node_a = new Node(&a);

double b = 5;
std::unique_ptr<Node> node_b = std::make_unique<Node>(&b);

node_a -> next = node_b;
node_b -> prev = node_a;

cout << *(int *)(node_a -> data) << endl;
cout << *(double *)(node_b -> data) << endl;

delete node_a;
node_a = NULL;
node_b = nullptr;
```
 */
typedef struct Node
{
    Node *prev;
    void *data;
    Node *next;
    Node(void *data) : prev(nullptr), data(data), next(nullptr) {}
} Node;

// functions
/**
 * For `printf`ing to `stderr`.
 */
void error_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

/**
 * Prints debug messages when `-DDEBUG` is added.
 */
void dprintf(const char *format, ...)
{
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
#endif
}

/**
 * Frees a pointer.
```cpp
double *pi = (double *)malloc(sizeof(double));
*pi = 3.14;
printf("PI: %.2f\n", pi);
custom_free((void **) &pi);
```
 */
int custom_free(void **pointer)
{
    if (pointer == NULL || *pointer == NULL)
    {
        dprintf("%p is NULL.\n", pointer);
        error_printf("ERROR:\n\t%p = 0x0;\tpointer is NULL.\n", pointer);
        return -1;
    }
    dprintf("DEBUG: Pointer %p will be freed...\n", pointer);
    free(*pointer);
    *pointer = NULL;
    dprintf("DEBUG: Value of %p is freed and is set to NULL.\n", pointer);
    return 0;
}

/**
 * Prints a pointer with its contents.
 * @param ptr Pointer that points to a set of hex values.
 * @param size Number of bytes to print.
 */
void memory_printf(const void *ptr, size_t size)
{
    const unsigned char *byte_ptr = static_cast<const unsigned char *>(ptr);
    for (size_t i = 0; i < size; ++i)
    {
        if (i % 16 == 0)
        {
            cout << '\n'
                 << static_cast<const void *>(byte_ptr + i) << ": ";
        }
        cout << hex << setfill('0') << setw(2) << static_cast<int>(byte_ptr[i]) << " ";
    }
    cout << dec << '\n';
}

/**
 * Generates a random decimal between 0 (inclusive) and 1 (exclusive).
 */
double custom_rand()
{
    int rand1 = rand();
    dprintf("DEBUG: Randomly generated number: %i.\n", rand1);
    double rand2 = static_cast<double>(rand1) / static_cast<double>(RAND_MAX);
    dprintf("DEBUG: Random number from 0 to 1: %.54lf.\n", rand2);
    return rand2;
}
/**
 * Shorter `getline(cin, <string>)`.
 */
string ask(string question = "")
{
    string input;
    cout << question;
    if (getline(cin, input))
    {
        return input;
    }
    else
    {
        throw runtime_error("Error getting input.");
    }
}

/**
 * Convert `string` into different types using `istringstream`.
 */
template <typename T>
T into(const string &input)
{
    istringstream stream(input);
    T value;
    stream >> value;
    if (stream.fail())
    {
        throw runtime_error("Conversion failed.");
    }
    return value;
}

/**
 * Structure for `istringstream`.
 */
typedef struct InputStringStream {
    istringstream iss;
    InputStringStream(string input) : iss(input) {};
    int set(const string& input) {
        if (input.empty()) {
            cerr << "Input string is empty." << endl;
            return -1;
        }
        iss.str(input);
        iss.clear();
        iss.seekg(0, ios::beg);
        return 0;
    }
    bool failed() {
        return iss.fail();
    }
    bool ended() {
        return iss.eof();
    }
    string read(size_t bytes = string::npos) {
        if (bytes == string::npos) {
            bytes = iss.str().size() - iss.tellg();
        }
        char *buffer = new char[bytes];
        iss.read(buffer, bytes);
        string data = string(buffer, iss.gcount());
        delete[] buffer;
        return data;
    }
} ISS;

/**
 * Structure for `ostringstream`.
 */
typedef struct OutputStringStream {
    ostringstream oss;
    OutputStringStream() : oss() {};
    int write(const string& data) {
        if (data.empty()) {
            cerr << "Data string is empty." << endl;
            return -1;
        }
        oss << data;
        return 0;
    }
    string get(size_t bytes = string::npos) {
        const size_t size = oss.str().size();
        if (bytes == string::npos) {
            bytes = size;
        }
        if (bytes > size) {
            bytes = size;
        }
        return oss.str().substr(0, bytes);
    }
} OSS;

typedef struct StringStream {
    ISS& iss;
    OSS& oss;
    StringStream(ISS& input, OSS& output) : iss(input), oss(output) {}
    int transform(function<string(const string&)> func, size_t bytes = string::npos) {
        while(!iss.ended()) {
            string data = iss.read(bytes);
            if(!data.empty()) {
                oss.write(func(data));
            }
        }
        return 0;
    }
} SS;

/**
 * The main function.
 */
int _main()
{
    ISS a("Hello World!");
    cout << a.read() << endl;

    return 0;
}
int main()
{
    const clock_t start = clock();

    srand(time(0));

    // main
    const int code = _main();

    cout << endl;
    const clock_t end = clock();
    double performance = double(end - start) / CLOCKS_PER_SEC;
    printf("Code: %.6lfs", performance);
    cout << endl;
    return code;
}