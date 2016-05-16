#include "util.h"

char *string(int length) {
    char *str = array(str, length + 1);
    str[length] = '\0';
    return str;
}
