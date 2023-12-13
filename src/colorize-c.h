#ifndef COLORIZER_C_COLORIZE_C_H
#define COLORIZER_C_COLORIZE_C_H

#if  defined(__WIN32) || defined(__WIN64)
#define _SHITDOWS
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
#include <stdarg.h>
#include <stdio.h>

// Referenced form https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
enum colorState
#ifndef _SHITDOWS
        : unsigned short int
#endif
{
    colorInitialized = 1,
    colorUninitialized = 0
};

enum colorScheme
#ifndef _SHITDOWS
        : unsigned short int
#endif
{
    Background = 48,
    Foreground = 38
};

enum colorMode
#ifndef _SHITDOWS
        : unsigned short int
#endif
{
    BIT_4 = 4,
    BIT_8 = 5,
    BIT_24 = 2
};

enum emphasis
#ifndef _SHITDOWS
        : unsigned int
#endif
{
    BOLD = 1 << (1 - 1),
    FAINT = 1 << (2 - 1),
    ITALIC = 1 << (3 - 1),
    UNDERLINE = 1 << (4 - 1),
    BLINK_SLOW = 1 << (5 - 1),
    BLINK_RAPID = 1 << (6 - 1),
    REVERSE = 1 << (7 - 1),
    CONCEAL_OR_HIDE = 1 << (8 - 1),
    CROSSEDOUT_OR_STRIKETHROUGH = 1 << (9 - 1),
    FRAKTUR_GOTHIC = 1 << (20 - 1),
    DOUBLE_UNDERLINE_OR_NOT_BOLD = 1 << (21 - 1)
};


enum BIT_4_COLORS
#ifndef _SHITDOWS
        : unsigned short int
#endif
{
    COLOR_NONE = 999,
    // 3BIT FOREGROUND COLORS
    FOREGROUND_Black = 30,
    FOREGROUND_Red = 31,
    FOREGROUND_Green = 32,
    FOREGROUND_Yellow = 33,
    FOREGROUND_Blue = 34,
    FOREGROUND_Magenta = 35,
    FOREGROUND_Cyan = 36,
    FOREGROUND_White = 37,

    // 3BIT BACKGROUND COLORS
    BACKGROUND_Black = 40,
    BACKGROUND_Red = 41,
    BACKGROUND_Green = 42,
    BACKGROUND_Yellow = 43,
    BACKGROUND_Blue = 44,
    BACKGROUND_Magenta = 45,
    BACKGROUND_Cyan = 46,
    BACKGROUND_White = 47,

    // All 3BIT Colors are till here,
    // the rest is 4BIT

    // 3BIT FOREGROUND COLORS
    FOREGROUND_Gray = 90,
    FOREGROUND_LIGHT_Red = 91,
    FOREGROUND_LIGHT_Green = 92,
    FOREGROUND_LIGHT_Yellow = 93,
    FOREGROUND_LIGHT_Blue = 94,
    FOREGROUND_LIGHT_Magenta = 95,
    FOREGROUND_LIGHT_Cyan = 96,
    FOREGROUND_LIGHT_White = 97,

    // 3BIT BACKGROUND COLORS
    BACKGROUND_Gray = 100,
    BACKGROUND_LIGHT_Red = 101,
    BACKGROUND_LIGHT_Green = 102,
    BACKGROUND_LIGHT_Yellow = 103,
    BACKGROUND_LIGHT_Blue = 104,
    BACKGROUND_LIGHT_Magenta = 105,
    BACKGROUND_LIGHT_Cyan = 106,
    BACKGROUND_LIGHT_White = 107,
};

const static unsigned int ANSI_MAX_COLOR_SIZE = 255;

typedef struct {
    union {
        short unsigned int R;
        short unsigned int G;
        short unsigned int B;
        short unsigned int O;
    };

    unsigned int HEX;
} RGB_CONTAINER;


bool is_valid_color(const enum colorMode colorModeArg, const signed int colorCodeArg) {
    switch (colorModeArg) {
        case BIT_4:
            return (
                    (colorCodeArg >= 30 && colorCodeArg <= 37 || colorCodeArg >= 90 && colorCodeArg <= 97)
                    ||
                    (colorCodeArg >= 40 && colorCodeArg <= 47 || colorCodeArg >= 100 && colorCodeArg <= 107)
            );
        case BIT_8:
            return colorCodeArg >= 0 && colorCodeArg <= ANSI_MAX_COLOR_SIZE;
        case BIT_24: {
            RGB_CONTAINER colorCodeHolder = {.HEX = colorCodeArg};
            return (colorCodeHolder.R >= 0 && colorCodeHolder.R <= 0xFF) &&
                   (colorCodeHolder.G >= 0 && colorCodeHolder.G <= 0xFF) &&
                   (colorCodeHolder.B >= 0 && colorCodeHolder.B <= 0xFF);
        }
        default:
            return false;
    }
}


const static char ANSI_DELIMITER = ';';
const static char ANSI_CURSOR_MANIPULATOR_DELIMITER = '?';
const static char ANSI_ESCAPE_SEQUENCE = '\033';
const static char *ANSI_ESCAPE_SEQUENCE_STRING = "\033[";
const static char *ANSI_RESET = "\033[0m";
const static char *ANSI_CURSOR_MANIPULATOR_HIDE = "\033[?25l";
const static char *ANSI_CURSOR_MANIPULATOR_SHOW = "\033[?25h";


int unstd_vscprintf(const char *restrict format, va_list pargs) {
    va_list argcopy;
    va_copy(argcopy, pargs);
    int result = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return result;
}

int _____unstd_vasprintf(char **restrict strp, const char *restrict fmt, va_list ap) {
    int len = unstd_vscprintf(fmt, ap);
    if (len == -1) { return -1; }
    size_t size = (size_t) len + 1;
    char *str = malloc(size);
    if (!str) { return -1; }
    int r = vsnprintf(str, len + 1, fmt, ap);
    if (r == -1) {
        free(str);
        return -1;
    }
    *strp = str;
    return r;
}

#define _____unstd_format(var, format_str)\
    va_list valist;\
    va_start(valist, format_str);\
    char *var = NULL;\
    _____unstd_vasprintf(&var, format_str, valist);\
    va_end(valist)

int _____unstd_asprintf(char **strp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r = _____unstd_vasprintf(strp, fmt, ap);
    va_end(ap);
    return r;
}

char *_____unstd_inplace_asprintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char *temp = NULL;
    _____unstd_vasprintf(&temp, fmt, ap);
    va_end(ap);
    return temp;
}

char *RGBToANSI(const signed int colorCodeArg) {
    RGB_CONTAINER hexColorArg = {.HEX = colorCodeArg};
    return _____unstd_inplace_asprintf("%d;%d;%d",
                                       hexColorArg.R,
                                       hexColorArg.G,
                                       hexColorArg.B);
}

char *color_scheme_style(const enum colorMode colorModeArg,
                         const signed int foregroundColorArg,
                         const signed int backgroundColorArg,
                         const unsigned long int stylesArg) {
    bool backgroundColorState = is_valid_color(colorModeArg, backgroundColorArg);
    bool foregroundColorState = is_valid_color(colorModeArg, foregroundColorArg);

    char *backgroundStartTemp = NULL;
    if (backgroundColorState && BIT_4 != colorModeArg) {
        _____unstd_asprintf(&backgroundStartTemp, "%s", ANSI_ESCAPE_SEQUENCE_STRING);
    }

    char *foregroundStartTemp = NULL;
    if (foregroundColorState || stylesArg) {
        _____unstd_asprintf(&foregroundStartTemp, "%s", ANSI_ESCAPE_SEQUENCE_STRING);
    }

    if (BIT_4 == colorModeArg) {
        if (backgroundColorState) {
            _____unstd_asprintf(&foregroundStartTemp, "%s%d;", foregroundStartTemp, backgroundColorArg);
        }
        if (foregroundColorState) {
            _____unstd_asprintf(&foregroundStartTemp, "%s%d;", foregroundStartTemp, foregroundColorArg);
        }
    } else {
        if (backgroundColorState) {
            char *backgroundColorAsX =
                    BIT_8 == colorModeArg
                    ? _____unstd_inplace_asprintf("%d", backgroundColorArg)
                    : RGBToANSI(backgroundColorArg);
            _____unstd_asprintf(&backgroundStartTemp,
                                "%s%d%c%d%c%s%c",
                                backgroundStartTemp,
                                Background,
                                ANSI_DELIMITER,
                                colorModeArg,
                                ANSI_DELIMITER,
                                backgroundColorAsX,
                                'm');
            free(backgroundColorAsX);
        }
        if (foregroundColorState) {
            char *foregroundColorAsX =
                    BIT_8 == colorModeArg
                    ? _____unstd_inplace_asprintf("%d", foregroundColorArg)
                    : RGBToANSI(foregroundColorArg);
            _____unstd_asprintf(&foregroundStartTemp,
                                "%s%d%c%d%c%s%c",
                                foregroundStartTemp,
                                Foreground,
                                ANSI_DELIMITER,
                                colorModeArg,
                                ANSI_DELIMITER,
                                foregroundColorAsX,
                                ANSI_DELIMITER);
            free(foregroundColorAsX);
        }
    }

    if (stylesArg) {
        for (signed int i = 1, b = 1; i >= 0; i <<= 1, ++b) {
            if (stylesArg & i) {
                _____unstd_asprintf(&foregroundStartTemp, "%s%d%c", foregroundStartTemp, b, ';');
            }
        }
    }


    if (*foregroundStartTemp) {
        foregroundStartTemp[strlen(foregroundStartTemp) - 1] = 'm';
    }

    char *result = _____unstd_inplace_asprintf(
            "%s%s",
            !backgroundStartTemp ? "" : backgroundStartTemp,
            !foregroundStartTemp ? "" : foregroundStartTemp);

    free(foregroundStartTemp);
    free(backgroundStartTemp);
    return result;
}

extern inline void HideCursor(void) {
    printf("%s", ANSI_CURSOR_MANIPULATOR_HIDE);
    fflush(stdout);
}

extern inline void ShowCursor(void) {
    printf("%s", ANSI_CURSOR_MANIPULATOR_SHOW);
    fflush(stdout);
}

char *crich4V(const char *const rawMessageArg,
              const short signed int foregroundColorArg,
              const short signed int backgroundColorArg,
              const unsigned long int stylesArg) {
    char *start = color_scheme_style(BIT_4, foregroundColorArg, backgroundColorArg, stylesArg);
    char *result = _____unstd_inplace_asprintf("%s%s%s", start, rawMessageArg, ANSI_RESET);
    free(start);
    return result;
}

char *crich4Vf(const short signed int foregroundColorArg,
               const short signed int backgroundColorArg,
               const unsigned long int stylesArg,
               const char *const rawMessageFormatArg,
               ...) {
    _____unstd_format(rawMessageArg, rawMessageFormatArg);
    char *start = color_scheme_style(BIT_4, foregroundColorArg, backgroundColorArg, stylesArg);
    char *result = _____unstd_inplace_asprintf("%s%s%s", start, rawMessageArg, ANSI_RESET);
    free(start);
    free(rawMessageArg);
    return result;
}


char *crich8V(const char *const rawMessageArg,
              const short signed int foregroundColorArg,
              const short signed int backgroundColorArg,
              const unsigned long int stylesArg) {
    char *start = color_scheme_style(BIT_8, foregroundColorArg, backgroundColorArg, stylesArg);
    char *result = _____unstd_inplace_asprintf("%s%s%s", start, rawMessageArg, ANSI_RESET);
    free(start);
    return result;
}


char *crich8Vf(const short signed int foregroundColorArg,
               const short signed int backgroundColorArg,
               const unsigned long int stylesArg,
               const char *const rawMessageFormatArg,
               ...) {
    _____unstd_format(rawMessageArg, rawMessageFormatArg);
    char *start = color_scheme_style(BIT_8, foregroundColorArg, backgroundColorArg, stylesArg);
    char *result = _____unstd_inplace_asprintf("%s%s%s", start, rawMessageArg, ANSI_RESET);
    free(start);
    free(rawMessageArg);
    return result;
}

char *crich24V(const char *const rawMessageArg,
               const signed int foregroundColorArg,
               const signed int backgroundColorArg,
               const unsigned long int stylesArg) {
    char *start = color_scheme_style(BIT_24, foregroundColorArg, backgroundColorArg, stylesArg);
    char *result = _____unstd_inplace_asprintf("%s%s%s", start, rawMessageArg, ANSI_RESET);
    free(start);
    return result;
}


char *crich24Vf(const signed int foregroundColorArg,
                const signed int backgroundColorArg,
                const unsigned long int stylesArg,
                const char *const rawMessageFormatArg,
                ...) {
    _____unstd_format(rawMessageArg, rawMessageFormatArg);
    char *start = color_scheme_style(BIT_24, foregroundColorArg, backgroundColorArg, stylesArg);
    char *result = _____unstd_inplace_asprintf("%s%s%s", start, rawMessageArg, ANSI_RESET);
    free(start);
    free(rawMessageArg);
    return result;
}

#endif //COLORIZER_C_COLORIZE_C_H
