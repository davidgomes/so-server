#include "main.h"

void send_header(int socket) {
  sprintf(buf,HEADER_1);
  send(socket,buf,strlen(HEADER_1),0);

  sprintf(buf,SERVER_STRING);
  send(socket,buf,strlen(SERVER_STRING),0);

  sprintf(buf,HEADER_2);
  send(socket,buf,strlen(HEADER_2),0);
}

void cleanup() {
  printf("Cleaning up.\n");
  close(connection_socket);
  close(client_socket);
  exit(0);
}

int main(void) {
  connection_socket = start_connection(3000);
  client_name_len = sizeof(client_name);

  signal(SIGINT, cleanup);
  while (1) {
    if ((client_socket = accept(connection_socket,
                                (struct sockaddr *) &client_name,
                                &client_name_len)) == -1 ) {
      printf("Error accepting connection.\n");
      exit(1);
    }

    /* Send index.html */
    send_header(client_socket);

    int i,j;
    int found_get;
    char req_buf[SIZE_BUF];

    found_get = 0;
    while (utils_socket_read_line(client_socket,SIZE_BUF) > 0 ) {
      if (!strncmp(buf,GET_EXPR,strlen(GET_EXPR))) {
        // GET received, extract the requested page/script
        found_get=1;
        i=strlen(GET_EXPR);
        j=0;
        while( (buf[i]!=' ') && (buf[i]!='\0') )
          req_buf[j++]=buf[i++];
        req_buf[j]='\0';
      }
    }

    if (found_get) {
      FILE *fp;

      fp = fopen("../data/index.html", "r");

      char buf_tmp[SIZE_BUF];
      while (fgets(buf_tmp, SIZE_BUF, fp)) {
        send(client_socket, buf_tmp, strlen(buf_tmp), 0);
      }

      fclose(fp);

      close(client_socket);
    }
  }

  cleanup();

  return 0;
}

int run_command(char *command, char *output) {
  FILE *fp;
  char line[1024];
  output[0] = 0;

  fp = popen(command, "r");

  if (fp == NULL) {
    printf("Failed to run command\n");
    return 1;
  }

  while (fgets(line, sizeof(line) - 1, fp) != NULL) {
    strcat(output, line);
  }

  pclose(fp);

  return 0;
}
