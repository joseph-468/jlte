#include <stdlib.h>

typedef struct GapBuffer {
	char *data;
	int bufLen;
	int left;
	int right;
} GapBuffer;

char * printBuffer(GapBuffer *gapBuffer) {
	int counter = 0;
	char *dog = malloc(gapBuffer->bufLen);
	for (int i = 0; i < gapBuffer->bufLen+64; i++) {
		if (gapBuffer->data[i] != '\0') {
			dog[counter] = gapBuffer->data[i];	
			counter++;
		}
	}
	return dog;
}

void grow(GapBuffer *gapBuffer, int pos) {
	// Call when left == right
	gapBuffer->bufLen += 8;
	int newSize = gapBuffer->bufLen+64;
	char *newData = malloc(newSize);
	for (int i = 0; i < pos; i++) {
		newData[i] = gapBuffer->data[i];
	}
	for (int i = 0; i < 8; i++) {
		newData[pos+i] = '\0';
	}
	for (int i = pos; i < newSize; i++) {
		newData[i+8] = gapBuffer->data[i];
	}
	free(gapBuffer->data);
	gapBuffer->data = newData;
	gapBuffer->left += 8;
	gapBuffer->right += 8;
}
