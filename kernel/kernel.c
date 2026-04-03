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

static unsigned char inb(unsigned short port) {
    unsigned char r;
    __asm__ volatile("inb %1,%0":"=a"(r):"Nd"(port));
    return r;
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

    print("MyKernel v0.1 - Keyboard Working!", 0x0A);
    newline();
    newline();
    print("> ", 0x0F);

    unsigned char prev = 0;

    while (1) {
        unsigned char status = inb(0x64);
        if (status & 1) {
            unsigned char sc = inb(0x60);

            // key press only (not release)
            if (!(sc & 0x80)) {
                if (sc != prev) {
                    char c = (sc < 58) ? sc_map[sc] : 0;
                    if (c == '\n') {
                        newline();
                        print("> ", 0x0F);
                    } else if (c != 0) {
                        putchar(c, 0x0F);
                    }
                    prev = sc;
                }
            } else {
                prev = 0;
            }
        }
    }
}