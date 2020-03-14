#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unordered_map>
#include <clib/cstring.h>
#include <clib/cmap.h>

declare_CMap(ii, int64_t, int64_t);
declare_CVector_string(s);

int main()
{
    clock_t before, difference;
    CMap_ii map = cmap_initializer;
    CMapEntry_ii* entry;
    uint64_t checksum = 0, erased, get;
    const size_t N = 50000000;

    printf("Starting %llu\n", N);
    //cmap_ii_reserve(&map, N * 1.25);
    cmap_ii_clear(&map);
    srand(123);
    before = clock();
    checksum = 0; erased = 0; get = 0;
    for (size_t i = 0; i < N; ++i) {
        int64_t rnd = rand();
        int op = rand() >> 13;
        switch (op) {
        case 1:
            checksum += ++cmap_ii_put(&map, rnd, i-1)->value;
            break;
        case 2:
            entry = cmap_ii_get(map, rnd);
            if (entry) { 
                ++erased;
                checksum += entry->value;
                cmap_ii_erase(&map, rnd);
            }
            break;
        case 3:
            get += (cmap_ii_get(map, rnd) != NULL);
            break;
        }
    }
    difference = clock() - before;
    printf("CMap_ii: size: %llu, time: %f, sum: %llu, erased: %llu, get %llu\n", cmap_size(map), 1.0 * difference / CLOCKS_PER_SEC, checksum, erased, get);

    std::unordered_map<int64_t, int64_t> map2, map3;
    std::unordered_map<int64_t, int64_t>::const_iterator iter;
    //map2.reserve(N);
    srand(123);
    before = clock();
    checksum = 0; erased = 0; get = 0;
    for (size_t i = 0; i < N; ++i) {
        int64_t rnd = rand();
        int op = rand() >> 13;
        switch (op) {        
        case 1:
            checksum += ++(map2[rnd] = i-1);
            break;
        case 2:
            iter = map2.find(rnd);
            if (iter != map2.end()) { 
                ++erased;
                checksum += iter->second;
                map2.erase(rnd);
            }
            break;
        case 3:
            get += (map2.find(rnd) != map2.end());
            break;
        }
    }
    difference = clock() - before;
    printf("std::um: size: %llu, time: %f, sum: %llu, erased: %llu, get %llu\n", map2.size(), 1.0 * difference / CLOCKS_PER_SEC, checksum, erased, get);
     
    cmap_ii_destroy(&map);
    map2.clear();
}
