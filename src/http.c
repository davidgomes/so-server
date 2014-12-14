#include "http.h"

/**
 * Parses a given HTTP request from 'socket'. If it is a GET, then
 * set request->found_get accordingly. Also, set request->name
 * to the name of the file requested.
 */
void http_parse_request(int socket, http_request *request) {
  int found_get = 0;
  int i, j;

  while (utils_socket_read_line(socket, SIZE_BUF) > 0) {
    if (!strncmp(buf, GET_EXPR, strlen(GET_EXPR))) {
      found_get = 1;
      i = strlen(GET_EXPR);
      j = 0;

      while (buf[i] != ' ' && buf[i] != '\0') {
        request->name[j++] = buf[i++];
      }

      request->name[j] = '\0';
    }
  }

  if (!strlen(request->name)) {
    strcpy(request->name, "index.html");
  }

  request->socket = socket;
  request->found_get = found_get;

  char *file_extension = utils_get_file_extension(request->name);

  char prefix_dir[SIZE_BUF];
  prefix_dir[0] = 0;
  utils_get_prefix_directory(prefix_dir, request->name);

  if (strcmp(prefix_dir, SCRIPT_DIR) == 0) {
    request->type = DYNAMIC_SCRIPT;
    return;
  }

  if (strcmp(file_extension, "html") == 0) {
    request->type = STATIC_PAGE;
  } else {
    request->type = SOMETHING_ELSE; // things such as favicon.ico's
  }
}

void http_send_header(int socket, char content_type[]) {
  char buf[SIZE_BUF];

  sprintf(buf, HEADER_1);
  send(socket, buf, strlen(HEADER_1),0);

  sprintf(buf, SERVER_STRING);
  send(socket, buf, strlen(SERVER_STRING),0);

  sprintf(buf, "Content-Type: %s\r\n\r\n", content_type);
  send(socket, buf, strlen(HEADER_2), 0);
}

void http_int_to_type(char *result, int type) {
  if (type == 1) {
    strcpy(result, "STATIC_PAGE");
  } else if (type == 2) {
    strcpy(result, "DYNAMIC_SCRIPT");
  } else if (type == 3) {
    strcpy(result, "SOMETHING_ELSE");
  }
}
