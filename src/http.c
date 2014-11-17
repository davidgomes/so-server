#include "http.h"

void http_send_header(int socket) {
  char buf[SIZE_BUF];
  
  sprintf(buf,HEADER_1);
  send(socket,buf,strlen(HEADER_1),0);

  sprintf(buf,SERVER_STRING);
  send(socket,buf,strlen(SERVER_STRING),0);

  sprintf(buf,HEADER_2);
  send(socket,buf,strlen(HEADER_2),0);
}
