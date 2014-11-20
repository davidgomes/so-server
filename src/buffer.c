#include "buffer.h"

buffer* buffer_create(int size){
	buffer* buf = (buffer*) malloc(sizeof(buffer));
	buf->size = size;
	buf->cur_size = 0;
	buf->first = (buffer_node*) malloc(sizeof(buffer_node));
	buf->first->next = NULL;
	return buf;
}

buffer_node* buffer_create_node(http_request* hr){
	buffer_node* bn = (buffer_node*) malloc(sizeof(buffer_node));
	bn->request = hr;
	bn->next = NULL;
	return bn;
}

void buffer_add(buffer* buf, http_request* hr){
	buffer_node* bn;
	for(bn = buf->first; bn->next != NULL; bn = bn->next){
		;
	}
	bn->next = buffer_create_node(hr);

	printf("Added node to buffer\n");
}