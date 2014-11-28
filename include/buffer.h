#ifndef BUFFER_HEADER
#define BUFFER_HEADER

#include <stdio.h>
#include <stdlib.h>

#include "http.h"

struct buffer_node typedef buffer_node;

struct buffer_node {
  buffer_node* next;
  http_request* request;
};

struct {
  buffer_node* first;
  int size, cur_size;
} typedef buffer;

buffer* buffer_create(int size);
buffer_node* buffer_create_node(http_request* hr);
void buffer_add(buffer* buf, http_request* hr);

#endif
