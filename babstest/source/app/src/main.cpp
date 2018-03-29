#include <stdio.h>
#include <stdlib.h>
#if defined LOAD_DLLIB
#include <dlfcn.h>
#endif // LOAD_DLLIB

#include "toolkit/testfun1.h"
#include "toolkit/testfun3.hpp"

#if defined LOAD_DLLIB
void loadDLLib()
{
  void *handle;
  char *error;
  void (*fun)();
  char fileName[300];
  char* buildDir = getenv("BUILD_DIR");

  sprintf(fileName, "%s/toolkit/libtoolkit.so.1.0.0", buildDir);
  printf("fileName = %s\n", fileName);

  printf("mark1\n");

  dlerror();
  handle = dlopen ( fileName , RTLD_LAZY);
  if (!handle) {
      fputs (dlerror(), stderr);
                printf("\n");
      exit(1);
  }

  prinatf("mark2\n");

  fun = (void (*)()) dlsym(handle, "testfun1");
  if ((error = dlerror()) != NULL)  {
      fputs(error, stderr);
                printf("\n");
      exit(1);
  }

  printf("mark3\n");

  (*fun)();

  prinatf("mark4\n");

  dlclose(handle);
}
#endif // LOAD_DLLIB

int main()
{
#if defined LOAD_DLLIB
  loadDLLib();
#endif // LOAD_DLLIB

  printf("Hello from main()\n");

#if !defined LOAD_DLLIB
	testfun1();
	testfun3();
#endif
	printf("(=%x\n", '(');
	printf(")=%x\n", ')');
	printf("<=%x\n", '<');
	printf(">=%x\n", '>');

  return 0;
}

