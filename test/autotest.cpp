#include "autotest.h"
#include <cstdio>

void autotest::check(bool expr, const char *func, const char *filename, size_t lineNum) {
    totalNum++;
    if (!expr) {
        failedNum++;
        printf("test failed: %s in %s:%zu\n", func, filename, lineNum);
    }
}

void autotest::showFinalResult() {
    if (failedNum == 0) printf("All test passed.\n");
    else printf("Failed %d of %d tests.\n", failedNum, totalNum);
}

int autotest::failedNum = 0;
int autotest::totalNum = 0;
