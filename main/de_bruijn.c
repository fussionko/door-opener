#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Generate a De Bruijn sequence. Code interpreted from wikipedia. https://gist.github.com/crowell/77b601db16562ac49834
void db(int t, int p, int n, int maxlen, int k, int* a, char* sequence, char* charset) {
    if (strlen(sequence) == maxlen) 
    {
        return;
    }
    if (t > n) 
    {
        if (n % p == 0) 
        {
            int j;
            for (j = 1; j <= p; ++j) 
            {
                sequence[strlen(sequence)] = charset[a[j]];
                if (strlen(sequence) == maxlen) 
                {
                    return;
                }
            }
      }
    } 
    else 
    {
        a[t] = a[t - p];
        db(t + 1, p, n, maxlen, k, a, sequence, charset);
        int j;
        for (j = a[t - p] + 1; j < k; ++j) 
        {
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

// Generate a De Bruijn sequence. Code interpreted from wikipedia. https://gist.github.com/crowell/77b601db16562ac49834
void db_bin(int t, int p, int n, int maxlen, int k, int* a, char* sequence) 
{
    size_t seq_len = strlen(sequence);
    if (seq_len == maxlen) return;
    if (t > n) 
    {
        if (n % p == 0) 
        {
            for (int j = 1; j <= p; ++j) 
            {
                // a[j] hold 0 or 1
                //sequence[seq_len] = charset[a[j]];
                sequence[seq_len] = a[j] + '0';
                ++seq_len;
                if (seq_len == maxlen) return;
            }
        }
    } 
    else 
    {
        a[t] = a[t - p];
        db_bin(t + 1, p, n, maxlen, k, a, sequence);

        // Loop trough alphabet

        for (int j = a[t - p] + 1; j < k; ++j) 
        {
            a[t] = j;
            db_bin(t + 1, t, n, maxlen, k, a, sequence);
        }
    }
}

// Generate De Bruijn sequence for binary alphabet (0 and 1) len: k^n+n-1
// need to be free by caller
char* de_bruijn_binary(int n)
{
    // Alphabet length = 2
    const int k = 2;
    const int len = (2 << n) + n - 1;// 10x slowe with pow (int)pow(k, n) + n - 1;

    int* a = malloc(sizeof(int) * k * n);
    if (a == NULL) return NULL;
    memset(a, 0, sizeof(int) * k * n);
    // int* a = malloc(k * n, sizeof(int));
    // if (a == NULL) return NULL;


    char* sequence = malloc(sizeof(char) * len + 1);
    if (sequence == NULL) return NULL;
    memset(sequence, 0, sizeof(char) * len + 1);

    // char* sequence = calloc(len + 1, sizeof(char));
    // if (sequence == NULL) return NULL;

    // Generate sequence
    db_bin(1, 1, n, len, k, a, sequence);

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