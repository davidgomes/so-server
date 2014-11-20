#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "http.h"

struct buffer_node typedef buffer_node;

struct buffer_node{
	buffer_node* next;
	http_request* request;
};

struct{
	buffer_node* first;
	int size, cur_size;
}typedef buffer;

void create_buffer(int size);

#endif
