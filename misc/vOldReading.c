int main(int argc, char *argv[]) {
   if (argc < 3) {
      printf("Too few arguments!\n");
      return EXIT_FAILURE;
   }
   int i;
   int j;
   FILE *fp;
   char list[BUFF][BUFF];
   for (i = 1; i < argc; i++) {
      fp = fopen(argv[i], "r");
      for (j = 0; fscanf(fp, "%s", list[j]) == 1; j++) {
      }
      fclose(fp);
   }
   for (i = 0; i < j; i++) {
      printf("%s\n", list[i]);
   }



   return EXIT_SUCCESS;
}

