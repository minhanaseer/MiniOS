void kernel_main() {
    // Pointer to video memory
    char* video = (char*) 0xb8000;

    // Write 'H'
    video[0] = 'H';
    video[1] = 0x07;
    // Write 'e'
    video[2] = 'e';
    video[3] = 0x07;
    // Write 'l'
    video[4] = 'l';
    video[5] = 0x07;
    // Write 'l'
    video[6] = 'l';
    video[7] = 0x07;
    // Write 'o'
    video[8] = 'o';
    video[9] = 0x07;

    while(1); // keep running
}