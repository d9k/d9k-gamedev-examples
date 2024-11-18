#include <cstdio>
#include <vector>
#include <time.h>
#include <stdlib.h>

int random_range(int min, int max) {
    int n = rand();
    return n % (max-min+1) + min;
}

void generate_pseudo_random_array(int size) {
    std::vector<int> remaining_numbers = {};
    for (int i=0; i<size; i++) {
        remaining_numbers.push_back(i);
    }

    std::vector<int> result = {};

    int remaining_size = size;

    while (remaining_size > 0) {
        int random_pos = random_range(0, remaining_size-1);
        int random_number = remaining_numbers[random_pos];

        result.push_back(random_number);

        remaining_numbers.erase(
            remaining_numbers.begin() + random_pos
        );
        remaining_size = remaining_numbers.size();

        // printf("remaining size: %i\n", remaining_size);
    }

    for (int n : result) {
        printf("%i\n", n);
    }
}

int main() {
    printf("Hello, gcc!\n");
    srand( (unsigned)time(NULL) );

    // for (int i=0; i<100; i++) {
    //     printf("%i\n", random_range(1, 6));
    // }

    generate_pseudo_random_array(256);
}