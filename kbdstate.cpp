#include <iostream>
#include <linux/input.h>
#include <string.h>

#include "kbdstate.h"

int main (int argc, char *argv[], char *env[])
{
    if (argc != 2) exit(EXIT_FAILURE);

    std::string name = argv[1];
    auto keyi = KEYNAMES.find(name);
    if (keyi == KEYNAMES.end()) {
        std::cerr << "Unknown key name '" << name << "'" << std::endl;
        return 0;
    }
    int key = keyi->second;

    FILE *kbd = fopen("/dev/input/by-path/platform-i8042-serio-0-event-kbd", "r");

    char key_map[KEY_MAX / 8 + 1];
    memset(key_map, 0, sizeof(key_map));
    ioctl(fileno(kbd), EVIOCGKEY(sizeof(key_map)), key_map);

    int keyb = key_map[key / 8];
    int mask = 1 << (key % 8);

    bool pressed = (keyb & mask);
    std::cout << "The '" << name << "' key is " << (pressed ? "" : "NOT ") << "PRESSED" << std::endl;
    return pressed ? 0 : 1;
}
