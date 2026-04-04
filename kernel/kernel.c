static char* video = (char*) 0xb8000;
static int cursor = 0;

static void clear() {
    for (int i = 0; i < 80*25*2; i++) video[i] = 0;
    cursor = 0;
}

static void putchar(char c, char col) {
    if (cursor >= 80*25) cursor = 0;
    video[cursor*2]   = c;
    video[cursor*2+1] = col;
    cursor++;
}

static void print(const char* s, char col) {
    for (int i = 0; s[i]; i++) putchar(s[i], col);
}

static void newline() {
    cursor = ((cursor/80)+1)*80;
    if (cursor >= 80*25) cursor = 0;
}

static void backspace() {
    if (cursor > 0) {
        cursor--;
        video[cursor*2]   = ' ';
        video[cursor*2+1] = 0x07;
    }
}

static unsigned char inb(unsigned short port) {
    unsigned char r;
    __asm__ volatile("inb %1,%0":"=a"(r):"Nd"(port));
    return r;
}

static void wait() {
    for (volatile int i = 0; i < 10000; i++);
}

static char sc_map[58] = {
    0,  0,  '1','2','3','4','5','6','7','8','9','0','-','=',
    0,  0,  'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' '
};

void kernel_main() {
    clear();

    print("MyKernel v0.1", 0x0A);
    newline();
    print("Keyboard: OK", 0x0A);
    newline();
    print("----------------------------", 0x08);
    newline();
    print("> ", 0x0F);

    unsigned char last_sc = 0;
    int key_held = 0;
    int hold_counter = 0;

    while (1) {
        unsigned char status = inb(0x64);

        if (!(status & 1)) {
            continue;
        }

        unsigned char sc = inb(0x60);

        // Key release
        if (sc & 0x80) {
            unsigned char released = sc & 0x7F;
            if (released == last_sc) {
                last_sc = 0;
                key_held = 0;
                hold_counter = 0;
            }
            continue;
        }

        // New key press
        if (sc != last_sc) {
            last_sc = sc;
            key_held = 0;
            hold_counter = 0;

            char c = (sc < 58) ? sc_map[sc] : 0;

            if (c == '\n') {
                newline();
                print("> ", 0x0F);
            } else if (sc == 0x0E) {
                // Backspace
                backspace();
            } else if (c != 0) {
                putchar(c, 0x0F);
            }

        } else {
            // Same key held down
            hold_counter++;
            if (hold_counter > 3000) {
                key_held = 1;
            }
            if (key_held) {
                char c = (sc < 58) ? sc_map[sc] : 0;
                if (c != 0 && c != '\n') {
                    putchar(c, 0x0F);
                    wait();
                }
            }
        }
    }
}