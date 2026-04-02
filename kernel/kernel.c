void kernel_main() {
    char* video = (char*) 0xb8000;
    const char* msg = "Hello from MyKernel!";
    
    int i = 0;
    while (msg[i] != 0) {
        video[i*2]   = msg[i];
        video[i*2+1] = 0x0A;
        i++;
    }
    
    while(1);
}