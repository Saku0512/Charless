#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Helper function to run a command and check its status
void run_command(const char* command, const char* test_name) {
    printf("    Executing: %s\n", command);
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "\n🔴 Command failed for test '%s': %s\n", test_name, command);
        // We don't exit here to allow cleanup to run
        assert(status == 0);
    }
}

// Helper function to read a file into a string
char* read_output_file(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(len + 1);
    if (buffer) {
        fread(buffer, 1, len, f);
        buffer[len] = '\0';
    }
    fclose(f);
    return buffer;
}

void run_test(const char* test_name, const char* cless_code, const char* expected_output) {
    printf("Running test: %s...\n", test_name);

    char cless_filename[50];
    char output_filename[50];
    char compile_command[100];
    char run_command_str[100];

    // Setup filenames and commands
    sprintf(cless_filename, "%s.cless", test_name);
    sprintf(output_filename, "%s.txt", test_name);
    sprintf(compile_command, "./clessc %s", cless_filename);
    sprintf(run_command_str, "./cless.out > %s", output_filename);

    // 1. Create .cless file
    FILE* f = fopen(cless_filename, "w");
    assert(f != NULL);
    fprintf(f, "%s", cless_code);
    fclose(f);

    // 2. Compile
    run_command(compile_command, test_name);

    // 3. Run and capture output
    run_command(run_command_str, test_name);

    // 4. Check output
    char* actual_output = read_output_file(output_filename);
    printf("    Expected: '%s', Got: '%s'\n", expected_output, actual_output ? actual_output : "(null)");
    assert(actual_output != NULL);
    assert(strcmp(actual_output, expected_output) == 0);
    
    free(actual_output);

    // 5. Cleanup
    remove(cless_filename);
    remove("cless.out");
    remove(output_filename);

    printf("✅ Test %s PASSED\n\n", test_name);
}

int main() {
    printf("===== Running Codegen E2E Tests =====\n");

    run_test(
        "if_not_equal", 
        "512099102051209920208220722099532001202652071209962200120266200020",
        "A"
    );

    run_test(
        "if_equal",
        "512099102051209910208220722099532001202652071209962200120266200020",
        "B"
    );

    printf("\nAll codegen tests passed!\n");
    return 0;
}
