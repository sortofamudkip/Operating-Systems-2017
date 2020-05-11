#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage long long sys_multiply(long a, long b) {
    return (long long)a*b;
}
