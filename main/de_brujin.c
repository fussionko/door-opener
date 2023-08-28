// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAIN 1

// // Generate a De Bruijn sequence. Code interpreted from wikipedia.
// void db(int t, int p, int n, int maxlen, int k, int* a, char* sequence,
//         char* charset) {
//   if (strlen(sequence) == maxlen) {
//     return;
//   }
//   if (t > n) {
//     if (n % p == 0) {
//       int j;
//       for (j = 1; j <= p; ++j) {
//         sequence[strlen(sequence)] = charset[a[j]];
//         if (strlen(sequence) == maxlen) {
//           return;
//         }
//       }
//     }
//   } else {
//     a[t] = a[t - p];
//     db(t + 1, p, n, maxlen, k, a, sequence, charset);
//     int j;
//     for (j = a[t - p] + 1; j < k; ++j) {
//       a[t] = j;
//       db(t + 1, t, n, maxlen, k, a, sequence, charset);
//     }
//   }
// }

// // Generate a De Bruijn sequence. Code interpreted from wikipedia.
// // The returned string is malloced, and it is the responsibility of the caller
// // to free the memory.
// char* de_bruijn(char* charset, int n, int maxlen) {
//   int k = strlen(charset);
//   int* a = malloc(sizeof(int) * k * n);
//   memset(a, 0, sizeof(int) * k * n);
//   char* sequence = malloc(sizeof(char) * maxlen + 1);
//   memset(sequence, 0, sizeof(char) * maxlen + 1);
//   db(1, 1, n, maxlen, k, a, sequence, charset);
//   free(a);
//   return sequence;
// }

// #ifdef MAIN


// void main()
// {
//     char charset[2] = { '0', '1' };
//     char* rtr_db = de_bruijn(charset, 12, 100);
//     printf("%s\n", rtr_db);
//     free(rtr_db);   
// }

// #endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALPHABET_SIZE 256

void db(int t, int p, int n, int k, int *a, int *sequence, const char *alphabet) {
    if (t > n) {
        if (n % p == 0) {
            for (int i = 1; i <= p; ++i) {
                sequence[i - 1] = a[i];
            }
        }
    } else {
        a[t] = a[t - p];
        db(t + 1, p, n, k, a, sequence, alphabet);
        for (int j = a[t - p] + 1; j < k; ++j) {
            a[t] = j;
            db(t + 1, t, n, k, a, sequence, alphabet);
        }
    }
}

char *de_bruijn(int k, int n) {
    int alphabet_size;
    const char *alphabet;
    if (k >= 0) {
        alphabet_size = k;
        char *str = (char *)malloc(alphabet_size);
        for (int i = 0; i < alphabet_size; ++i) {
            str[i] = i + '0';
        }
        alphabet = str;
    }

    int *a = (int *)malloc(sizeof(int) * alphabet_size * n);
    int *sequence = (int *)malloc(sizeof(int) * n);
    db(1, 1, n, alphabet_size, a, sequence, alphabet);

    char *result = (char *)malloc(n + 1);
    for (int i = 0; i < n; ++i) {
        result[i] = alphabet[sequence[i]];
    }
    result[n] = '\0';

    free(a);
    free(sequence);
    if (k >= 0) {
        free((char *)alphabet);
    }

    return result;
}

int main() {
    char *sequence1 = de_bruijn(2, 3);
    printf("%s\n", sequence1);
    free(sequence1);

    char *sequence2 = de_bruijn(2, 2);
    printf("%s\n", sequence2);
    free(sequence2);

    return 0;
}