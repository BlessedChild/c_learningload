#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>

static char *server_args[] = {
  "this_program",       /* this string is not used */
  "--datadir=.",
  "--key_buffer_size=32M"
};
static char *server_groups[] = {
  "embedded",
  "server",
  "this_program_SERVER",
  (char *)NULL
};

int main(void) {
  if (mysql_library_init(sizeof(server_args) / sizeof(char *),
                        server_args, server_groups)) {
    fprintf(stderr, "could not initialize MySQL client library\n");
    exit(1);
  }
  /* Use any MySQL API functions here */
  printf("数据库初始化成功！");
  mysql_library_end();

  return EXIT_SUCCESS;
}
