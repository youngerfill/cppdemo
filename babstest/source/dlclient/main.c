#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>


int main(int argc, char **argv) 
{
  void *handle;
  char *error;
  void (*fun)();
  char fileName[300];
  char* sourceDir = getenv("SOURCE_DIR");

  sprintf(fileName, "%s/dllib/libdllib.so.1.0.1", sourceDir);
  printf("fileName = %s\n", fileName);

  printf("mark1\n");

  dlerror();
  handle = dlopen ( fileName , RTLD_LAZY);
  if (!handle) {
      fputs (dlerror(), stderr);
		printf("\n");
      exit(1);
  }

  printf("mark2\n");

  fun = (void (*)()) dlsym(handle, "testfun1");
  if ((error = dlerror()) != NULL)  {
      fputs(error, stderr);
		printf("\n");
      exit(1);
  }

  printf("mark3\n");

  (*fun)();

  printf("mark4\n");

  dlclose(handle);

}

