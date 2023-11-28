#include <stdio.h>
#include <string.h>

void reverse(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char tmp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = tmp;
    }
}

int main()
{
    char stra[1001], strb[1001], result[1002];
    int i, j, carry = 0;

    
    scanf("%s", stra);

    
    scanf("%s", strb);

    reverse(stra);
    reverse(strb);

    int lena = strlen(stra);
    int lenb = strlen(strb);

    for (i = 0, j = 0; i < lena || j < lenb || carry; i++, j++) {
        int a = i < lena ? stra[i] - '0' : 0;
        int b = j < lenb ? strb[j] - '0' : 0;
        int sum = a + b + carry;
        carry = sum / 10;
        result[i] = sum % 10 + '0';
    }

    result[i] = '\0';

    reverse(result);

    printf("%s\n", result);

    return 0;
}

