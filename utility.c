#include "utility.h"
#include <stddef.h>

void **getLastFreePtrArrayItem(void **array, int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		if (array[i] == NULL)
			return &array[i];
	}
	return NULL;
}
