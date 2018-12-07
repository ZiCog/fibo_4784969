//
// Karatsuba algorithm harvested from here:
// https://www.quora.com/How-can-I-implement-Karatsuba-multiplication-algorithm-using-C++14
//

#include <string.h>
#include <stdlib.h>

const int MAX_LENGTH = 10001;  // Or whatever size you want

void add_zeros(char* a, int n)
{
    int lena = strlen(a);
    for (int i = lena - 1 + n; i >= n; --i)
    {
        a[i] = a[i - n];
    }
    a[lena + n] = 0;
    for (int i = 0; i < n; ++i)
    {
        a[i] = '0';
    }
}

void remove_zeros(char* a)
{
    int lena = strlen(a);
    int ind = 0;
    while (ind < lena && a[ind] == '0')
    {
        ++ind;
    }
    for (int i = ind; i < lena; ++i)
    {
        a[i - ind] = a[i];
    }
    a[lena - ind] = 0;
}

void sum(char* a, char* b, char* res)
{
    int lena = strlen(a);
    int lenb = strlen(b);

    // Reverse operands if b is longer
    if (lena < lenb)
    {
        sum(b, a, res);
        return;
    }

    int toAdd = 0;
    for (int inda = lena - 1, indb = lenb - 1; inda >= 0; --inda, --indb)
    {
        int x = a[inda] - '0';
        int y = indb >= 0 ? b[indb] - '0' : 0;

        int cur = x + y + toAdd;

        if (cur >= 10)
        {
          toAdd = 1;
          cur -= 10;
        }
        else
        {
          toAdd = 0;
        }

        res[inda] = cur + '0';
    }

    if (toAdd == 1)
    {
        add_zeros(res, 1);
        res[0] = '1';
    }
}

// assume that a > b
void sub(char* a, char* b, char* res)
{
    int lena = strlen(a);
    int lenb = strlen(b);

    //assert(lena >= lenb);

    int toSub = 0;
    for (int inda = lena - 1, indb = lenb - 1; inda >= 0; --inda, --indb)
    {
        int x = a[inda] - '0';
        int y = indb >= 0 ? b[indb] - '0' : 0;

        if (toSub == 1)
        {
            x--;
        }
        int cur;
        if (x < y)
        {
            cur = x + 10 - y;
            toSub = 1;
        }
        else
        {
            cur = x - y;
            toSub = 0;
        }
        res[inda] = cur + '0';
    }
}

// Returns a * 10^n
void mult10(char* a, int n)
{
    int lena = strlen(a);
    if (lena == 1 && a[0] == '0')
    {
        return;
    }
    for (int i = lena; i < lena + n; ++i)
    {
        a[i] = '0';
    }
    a[lena + n] = 0;
}

char* CreateArray(int len)
{
    char* res = (char*)malloc(len);
    memset(res, 0, len);
    return res;
}

// Add leading zeros if needed
void make_equal_length(char* a, char* b)
{
    int lena = strlen(a);
    int lenb = strlen(b);
    int n = lena > lenb ? lena : lenb;
    add_zeros(a, n - lena);
    add_zeros(b, n - lenb);
}

void mul(char* x, char* y, char* res)
{
    make_equal_length(x, y);
    int len = strlen(x);

    // Base case, do the multiply.
    if (len == 1)
        {
        int val = (x[0] - '0') * (y[0] - '0');
        if (val < 10) {
            res[0] = val + '0';
        }
        else
        {
            res[0] = (val / 10) + '0';
            res[1] = (val % 10) + '0';
        }
    }
    else
    {
        int rightSize = len / 2;
        int leftSize = len - rightSize;

        // Split x
    //    char* xl = CreateArray(len);
    //    char* xr = CreateArray(len);
        char* xl = CreateArray(leftSize + 1);  // Why use double length arrays here?
        char* xr = CreateArray(rightSize + 1);
        strncpy(xl, x, leftSize);
        strcpy(xr, x + leftSize);

        // Split y
    //    char* yl = CreateArray(len);
    //    char* yr = CreateArray(len);
        char* yl = CreateArray(leftSize + 1);   // Why use double length arrays here?
        char* yr = CreateArray(rightSize + 1);
        strncpy(yl, y, leftSize);
        strcpy(yr, y + leftSize);

    //    int maxl = 3 * len;             // Why 3 here? 2 works!
        int maxl = 2 * len;


        char* P1 = CreateArray(maxl);
        char* P2 = CreateArray(maxl);
        char* P3 = CreateArray(maxl);

        mul(xl, yl, P1);
        mul(xr, yr, P2);

        char* tmp1 = CreateArray(maxl);
        char* tmp2 = CreateArray(maxl);
        sum(xl, xr, tmp1);
        sum(yl, yr, tmp2);
        mul(tmp1, tmp2, P3);
        sub(P3, P1, P3);
        sub(P3, P2, P3);
        mult10(P3, rightSize);
        mult10(P1, 2 * rightSize);
        sum(P1, P2, res);
        sum(res, P3, res);
        remove_zeros(res);

        free (xl);
        free (xr);
        free (yl);
        free (yr);
        free (tmp1);
        free (tmp2);
        free (P1);
        free (P2);
        free (P3);
    }
}

