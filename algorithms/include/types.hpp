#ifndef TYPES_H_
#define TYPES_H_

#include <cstddef>
#include <cstdlib>
#include <cstring>

typedef struct {
    int *data;
    size_t len;
} Array;

void *safe_alloc(size_t len, size_t size) {
    void *data = calloc(len, size);
    if (data == NULL) {
        exit(1);
    }
    return data;
}

Array *init(size_t len, int * input) {
    Array *array = (Array *)safe_alloc(1, sizeof(Array));
    array->data = (int *)safe_alloc(len, sizeof(int));
    memcpy(array->data, input, len * sizeof(int));
    array->len = len;
    return array;
}


Array* DigitStringToIntArray(const std::string& digits)
{
	size_t _len = digits.length();
	int* _data = (int*)safe_alloc(_len, sizeof(int));
	for (size_t i = 0; i < _len; i++)
	{
		_data[i] = digits[i] - '0';
	}

	return init(_len, _data);
}

#endif  // TYPES_H_
