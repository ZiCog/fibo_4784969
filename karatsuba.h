extern "C" {
    const int MAX_LENGTH = 8000;  // Or whatever size you want

    void add_zeros(char* a, int n);

    void remove_zeros(char* a);

    void sum(char* a, char* b, char* res);

    // Assume that a > b
    void sub(char* a, char* b, char* res);

    // Returns a * 10^n
    void mult10(char* a, int n);

    char* CreateArray(int len);

    // Add leading zeros if needed
    void make_equal_length(char* a, char* b);

    void mul(char* x, char* y, char* res);
}

