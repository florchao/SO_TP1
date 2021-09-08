#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MINISAT "minisat"
#define GREP "grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e\".*SATISFIABLE\"  | xargs | tr -s \\n"
#define BUFFER_SIZE 256

void cleanBuffer(char *buffer);