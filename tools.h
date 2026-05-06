#ifndef TOOLS_H_
#define TOOLS_H_

#include <assert.h>
#include <cstddef>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


#define END         "\033[0;0m"

#define RED_CLR     "\033[1;31m"
#define GREEN_CLR   "\033[1;32m"
#define BLUE_CLR    "\033[1;34m"

#define RED(str)    RED_CLR   str END
#define GREEN(str)  GREEN_CLR str END
#define BLUE(str)   BLUE_CLR  str END

#endif //TOOLS_H_