#include <iostream>

#include <string.h>

#include "karatsuba.h"
#include "fibo_20000.h"

using namespace std;

int isEven(int n)
{
    return (n & 1) != 1;
}

char* memo[3] = {(char*)"0", (char*)"1", (char*)"1"};

void fibo (int n, char* res)
{
    // If memoized
    if (n <= 2) {
        res[0] = memo[n][0];
    }
    else
    {
        int k = (n / 2);
        if (isEven(n))
        {
            // fibo(k) * ((fibo(k + 1) * 2n) - fibo(k))
            char *fk = CreateArray(MAX_LENGTH);
            char *fk1 = CreateArray(MAX_LENGTH);
            char *prod = CreateArray(MAX_LENGTH);
            char *diff = CreateArray(MAX_LENGTH);
            char* two = CreateArray(MAX_LENGTH);
            two[0] = '2';
            fibo(k, fk);
            fibo(k + 1, fk1);
            mul(fk1, two, prod);
            sub(prod, fk, diff);
            mul(fk, diff, res);
            free(fk);
            free(fk1);
            free(prod);
            free(diff);
            free(two);
        }
        else
        {
            //  fibo(k) ** 2n + fibo(k + 1) ** 2n
            char *fk = CreateArray(MAX_LENGTH);
            char *fk1 = CreateArray(MAX_LENGTH);
            char *fk_2 = CreateArray(MAX_LENGTH);
            char *fk1_2 = CreateArray(MAX_LENGTH);
            fibo(k, fk);
            fibo(k + 1, fk1);
            mul(fk, fk, fk_2);
            mul(fk1, fk1, fk1_2);
            sum(fk_2, fk1_2, res);
            free(fk);
            free(fk1);
            free(fk_2);
            free(fk1_2);
        }
    }
}


int main()
{
    char a[] = "123456789";
    char b[] = "123456789";

    char *res = CreateArray(MAX_LENGTH);
    mul(a, b, res);
    cout << "The product of the entered nos. is: " << res << "\n";
    free(res);

    res = CreateArray(MAX_LENGTH);
    mul(a, a, res);
    cout << "The square of the numbers is: " << res << "\n";
    free(res);

    res = CreateArray(MAX_LENGTH);
    char* two = CreateArray(MAX_LENGTH);
    two[0] = '2';
    mul(a, two, res);
    cout << "Number times \"2\" is: " << res << "\n";
    free(res);

    char fart[] ="Hello"
                 "World";
    printf("%s\n", fart);

    res = CreateArray(MAX_LENGTH);
    int n = 20000;
    fibo(n, res);
    cout << "fibo(" << n << ") is: " << res << "\n";
    if (strcmp(res, fibo_20000) == 0)
    {
        printf("PASS.\n");
    }
    else
    {
        printf("FAIL.\n");
    }
    free(res);

    return 0;
}
// 0, 1, 1, 2, 3, 5, 8