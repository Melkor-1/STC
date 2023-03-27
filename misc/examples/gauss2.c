#include <stdio.h>
#include <time.h>

#include <stc/crandom.h>
#include <stc/cstr.h>

// Declare int -> int sorted map.
#define i_key int
#define i_val int
#include <stc/csmap.h>

int main()
{
    enum {N = 10000000};
    const double Mean = -12.0, StdDev = 6.0, Scale = 74;

    printf("Demo of gaussian / normal distribution of %d random samples\n", N);

    // Setup random engine with normal distribution.
    uint64_t seed = (uint64_t)time(NULL);
    stc64_t rng = stc64_new(seed);
    stc64_normalf_t dist = stc64_normalf_new(Mean, StdDev);

    // Create and init histogram map with defered destruct
    csmap_int hist = {0};
    cstr bar = {0};

    c_forrange (N) {
        int index = (int)round( stc64_normalf(&rng, &dist) );
        csmap_int_insert(&hist, index, 0).ref->second += 1;
    }

    // Print the gaussian bar chart
    c_forpair (index, count, csmap_int, hist) {
        int n = (int)((float)*_.count * StdDev * Scale * 2.5f / (float)N);
        if (n > 0) {
            cstr_resize(&bar, n, '*');
            printf("%4d %s\n", *_.index, cstr_str(&bar));
        }
    }
    cstr_drop(&bar);
    csmap_int_drop(&hist);
}
