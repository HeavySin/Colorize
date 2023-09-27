#include "src/colorize-c.h"
#include <stdio.h>

int main(void) {
    // blocks for test ████████████████████████████
    HideCursor();

    char *test_text;
    for (int colorCode = 0; colorCode <= 16; ++colorCode) {

        test_text = crich8Vf(colorCode, -1, 0,
                             "████████████████ [%s%d] ████████████████", colorCode < 10 ? "0" : "", colorCode);
        printf("%s\n", test_text);
        free(test_text);
    }
    putchar('\n');


    char *description = "A library for colorizing texts in terminal!";
    for (unsigned int colorCode = 0, charPtr = 0; charPtr <= strlen(description); ++colorCode, ++charPtr) {
        test_text = crich8Vf(colorCode += 4, -1, 0, "%c", description[charPtr]);
        printf("%s", test_text);
        free(test_text);
    }
    putchar('\n');

    test_text = crich8V("BOLD", -1, -1, BOLD);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("FAINT", -1, -1, FAINT);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("ITALIC", -1, -1, ITALIC);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("UNDERLINE", -1, -1, UNDERLINE);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("SLOW BLINK", -1, -1, BLINK_SLOW);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("RAPID BLINK", -1, -1, BLINK_RAPID);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("REVERSE", -1, -1, REVERSE);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("CONCEAL OR HIDE", -1, -1, CONCEAL_OR_HIDE);
    printf("%s  <- (CONCEAL OR HIDE)\n", test_text);
    free(test_text);

    test_text = crich8V("CROSSEDOUT OR STRIKETHROUGH", -1, -1, CROSSEDOUT_OR_STRIKETHROUGH);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("DOUBLE UNDERLINE OR NOT BOLD", -1, -1, DOUBLE_UNDERLINE_OR_NOT_BOLD);
    printf("%s\n", test_text);
    free(test_text);

    char *titleBar2 = "------------------------------------------------";
    for (int16_t colorCode = 0, charPtr = 0; charPtr <= strlen(titleBar2); ++colorCode, ++charPtr) {
        test_text = crich8Vf(colorCode += 5, -1, BOLD, "%c", titleBar2[charPtr]);
        printf("%s", test_text);
        free(test_text);
    }
    putchar('\n');

    // 8 & 24 BIT Coloring
    test_text = crich24V("Hello World", 0xeb106f, 0x263640, BOLD | UNDERLINE);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich8V("Hello World", 199, 253, 0);
    printf("%s\n", test_text);
    free(test_text);

    putchar('\n');

    // 4-BIT Coloring
    test_text = crich4V("Hello World", COLOR_NONE, BACKGROUND_Magenta, BOLD);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich4V("Hello World", FOREGROUND_LIGHT_Blue, COLOR_NONE, BOLD);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich4V("Hello World", FOREGROUND_LIGHT_Blue, BACKGROUND_Magenta, BOLD);
    printf("%s\n", test_text);
    free(test_text);

    test_text = crich4V("Hello World", FOREGROUND_Black, BACKGROUND_Cyan, 0);
    printf("%s\n", test_text);
    free(test_text);

    ShowCursor();
    return 0;
}
