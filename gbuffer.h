#ifndef GBUFFER_H
#define GBUFFER_H
typedef struct GapBuffer {
	char *data;
	int bufLen;
	int left;
	int right;
} GapBuffer;

char * printBuffer(GapBuffer *gapBuffer);
void grow(GapBuffer *gapBuffer, int pos);
#endif
