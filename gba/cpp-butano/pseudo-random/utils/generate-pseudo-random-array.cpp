#include <cstdio>
#include <vector>
#include <time.h>
#include <stdlib.h>
#define STRING_PATH_SIZE 512
#define STRING_NAME_SIZE 128

int random_range(int min, int max) {
    int n = rand();
    return n % (max-min+1) + min;
}

std::vector<int> generate_pseudo_random_array(int max, int repeat = 1) {
    std::vector<int> remaining_numbers = {};
    for (int r=0; r<repeat; r++) {
        for (int i=0; i<max; i++) {
            remaining_numbers.push_back(i);
        }
    }

    std::vector<int> result = {};

    int remaining_size = max;

    while (remaining_size > 0) {
        int random_pos = random_range(0, remaining_size-1);
        int random_number = remaining_numbers[random_pos];

        result.push_back(random_number);

        remaining_numbers.erase(
            remaining_numbers.begin() + random_pos
        );
        remaining_size = remaining_numbers.size();
    }

    return result;
}

void output_to_file_h(char output_path_h[STRING_PATH_SIZE], int max, int repeat) {
    int size = max * repeat;

    FILE *output_file_h;
    output_file_h = fopen(output_path_h, "w");

    char headerFileDefine[STRING_NAME_SIZE] = "";
    sprintf(headerFileDefine, "PRN_%i_H", max);

    fprintf(output_file_h, "#ifndef %s\n", headerFileDefine);
    fprintf(output_file_h, "#define %s\n", headerFileDefine);
    fprintf(output_file_h, "\n");
    fprintf(output_file_h, "#include \"bn_common.h\"\n");
    fprintf(output_file_h, "\n");
    fprintf(output_file_h, "#define PRN_%i_SIZE %i\n", max, size);
    fprintf(output_file_h, "\n");
    fprintf(output_file_h, "namespace prn_%i\n", max);
    fprintf(output_file_h, "{\n"); // namespace begin
    fprintf(output_file_h, "  extern int max;\n");
    fprintf(output_file_h, "  extern int repeat;\n");
    fprintf(output_file_h, "  extern int size;\n");
    // TODO or BN_DATA_EWRAM_BSS?
    fprintf(output_file_h, "  extern BN_DATA_EWRAM int values[PRN_%i_SIZE];\n", max);
    fprintf(output_file_h, "}\n"); // namespace end
    fprintf(output_file_h, "\n");
    fprintf(output_file_h, "#endif");
    fclose(output_file_h);
}

void output_to_file_cpp(char output_path_cpp[STRING_PATH_SIZE], char header_name[STRING_NAME_SIZE], std::vector<int> result, int max, int repeat) {
    int size = result.size();

    FILE *output_file_cpp;
    output_file_cpp = fopen(output_path_cpp, "w");
    // fprintf(output_file_cpp, "#include \"bn_common.h\"\n");
    fprintf(output_file_cpp, "#include \"%s\"\n", header_name);
    fprintf(output_file_cpp, "\n");
    fprintf(output_file_cpp, "namespace prn_%i\n", max);
    fprintf(output_file_cpp, "{\n"); // namespace begin
    fprintf(output_file_cpp, "  int max = %i;\n", max);
    fprintf(output_file_cpp, "  int repeat = %i;\n", repeat);
    fprintf(output_file_cpp, "  int size = %i;\n", size);
    fprintf(output_file_cpp, "\n");
    // TODO or BN_DATA_EWRAM_BSS?
    fprintf(output_file_cpp, "  BN_DATA_EWRAM int values[PRN_%i_SIZE] = {\n", max);

    for (int i=0; i < result.size(); i++) {
        int n = result[i];
        fprintf(output_file_cpp, "    %i, // %i\n", n, i);
    }

    fprintf(output_file_cpp, "  };\n"); // array end
    fprintf(output_file_cpp, "}\n"); // namespace end
    fclose(output_file_cpp);
}

int main() {
    printf("Hello, gcc!\n");
    srand( (unsigned)time(NULL) );

    int max = 256;
    int repeat = 4;
    char output_file_name_h[STRING_NAME_SIZE] = "";
    char output_path_h[STRING_PATH_SIZE] = "";
    char output_path_cpp[STRING_PATH_SIZE] = "";

    sprintf(output_file_name_h, "prn_%i.h", max);
    sprintf(output_path_h, "include/%s", output_file_name_h);
    sprintf(output_path_cpp, "src/prn_%i.cpp", max);

    std::vector<int> result = generate_pseudo_random_array(max, repeat);

    // for (int i=0; i < result.size(); i++) {
    //     int n = result[i];
    //     printf("%i //%i\n", n, i);
    // }

    output_to_file_h(output_path_h, max, repeat);
    output_to_file_cpp(output_path_cpp, output_file_name_h, result, max, repeat);

    printf("%s", output_path_cpp);
}