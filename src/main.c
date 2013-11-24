#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *db;
  char name[200];

  /* debug only */
  if (argc != 2) {
    printf("Icorrect input\n");
    exit(1);
  }

  db = fopen("./database.txt", "r");
  fscanf(db, "%[^|]", name);
  printf("%s\n", name);

  fclose(db);

  return 0;
}
