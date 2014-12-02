#include "rectangle.h"
#include <stdlib.h>

typedef struct _rectangle {
	int width;
	int height;
} rectangle;

int sizeof_rectangle() {
	return sizeof(rectangle);
}

rectangle* create_rectangle(const int height, const int width) {
	rectangle* this=(rectangle*)malloc(sizeof(rectangle));
	construct_rectangle(this, height, width);
	return this;
}

void construct_rectangle(rectangle* this, const int height, const int width) {
	this->height=height;
	this->width=width;
}

void rectangle_destroy(rectangle* r) {
	free(r);
}

int rectangle_get_width(const rectangle const * r) {
	return r->width;
}

int rectangle_get_height(const rectangle const * r) {
	return r->height;
}

void rectangle_set_width(rectangle * r, const int width) {
	r->width=width;
}

void rectangle_set_height(rectangle * r, const int height) {
	r->height=height;
}

int rectangle_area(const rectangle const * r) {
	return r->height*r->width;
}
