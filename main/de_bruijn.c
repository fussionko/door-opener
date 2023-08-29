#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generate a De Bruijn sequence. Code interpreted from wikipedia. https://gist.github.com/crowell/77b601db16562ac49834
void db(int t, int p, int n, int maxlen, int k, int* a, char* sequence,
        char* charset) {
  if (strlen(sequence) == maxlen) {
    return;
  }
  if (t > n) {
    if (n % p == 0) {
      int j;
      for (j = 1; j <= p; ++j) {
        sequence[strlen(sequence)] = charset[a[j]];
        if (strlen(sequence) == maxlen) {
          return;
        }
      }
    }
  } else {
    a[t] = a[t - p];
    db(t + 1, p, n, maxlen, k, a, sequence, charset);
    int j;
    for (j = a[t - p] + 1; j < k; ++j) {
      a[t] = j;
      db(t + 1, t, n, maxlen, k, a, sequence, charset);
    }
  }
}

// Generate a De Bruijn sequence. Code interpreted from wikipedia.
// The returned string is malloced, and it is the responsibility of the caller
// to free the memory.
char* de_bruijn(char* charset, int n, int maxlen) {
  int k = strlen(charset);
  int* a = malloc(sizeof(int) * k * n);
  memset(a, 0, sizeof(int) * k * n);
  char* sequence = malloc(sizeof(char) * maxlen + 1);
  memset(sequence, 0, sizeof(char) * maxlen + 1);
  db(1, 1, n, maxlen, k, a, sequence, charset);
  free(a);
  return sequence;
}

#ifdef MAIN


void main()
{
    char charset[2] = { '0', '1' };
    char* rtr_db = de_bruijn(charset, 3, 100);
    printf("%s\n", rtr_db);
    free(rtr_db);   
}

#endif