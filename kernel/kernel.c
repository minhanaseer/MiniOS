// VGA video memory address
char* video = (char*) 0xb8000;
int cursor = 0;

// Print a single character on screen
void print_char(char c, char color) {
    video[cursor*2]   = c;
    video[cursor*2+1] = color;
    cursor++;
}

// Print a string on screen
void print(const char* str, char color) {
    int i = 0;
    while (str[i] != 0) {
        print_char(str[i], color);
        i++;
    }
}

// Read a byte from a hardware port
unsigned char port_read(unsigned short port) {
    unsigned char result;
    __asm__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Keyboard scan code to character map
char scancode_to_char(unsigned char sc) {
    char keys[] = {
        0, 0, '1','2','3','4','5','6','7','8','9','0','-','=',
        0, 0, 'q','w','e','r','t','y','u','i','o','p','[',']',
        0, 0, 'a','s','d','f','g','h','j','k','l',';','\'', '`',
        0,'\\','z','x','c','v','b','n','m',',','.','/', 0,
        '*', 0, ' '
    };
    if (sc < sizeof(keys)) return keys[sc];
    return 0;
}

void kernel_main() {
    // Clear screen
    for (int i = 0; i < 80*25*2; i++) {
        video[i] = 0;
    }

    // Welcome message
    print("MyKernel v0.1 - Type something!", 0x0A);
    cursor = 80; // move to next line

    print("> ", 0x0F);

    // Keyboard loop
    while (1) {
        // Wait for keyboard input
        if (port_read(0x64) & 1) {
            unsigned char sc = port_read(0x60);

            // Only handle key press (not release)
            if (!(sc & 0x80)) {
                char c = scancode_to_char(sc);
                if (c != 0) {
                    print_char(c, 0x0F);
                }
            }
        }
    }
}