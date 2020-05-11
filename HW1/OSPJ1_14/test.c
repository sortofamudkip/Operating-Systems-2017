#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    syscall(337); //this calls sys_hello()
    puts("input two positive integers:");
    long a, b; scanf("%ld%ld", &a, &b);
    long long c = syscall(338, a, b); //this calls sys_multiply
    printf("%ld * %ld = %lld\n", a, b, c);
    long m = syscall(339, a, b); //this calls sys_min
    printf("min(%ld, %ld) = %ld\n", a, b, m);
    return 0;
}
