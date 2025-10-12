void kmain(void) {
    const char *msg = "Hello, world from kernel!\n";
    volatile char *vga = (volatile char*)0xB8000;
    for (int i = 0; msg[i]; ++i) {
        vga[i*2] = msg[i];
        vga[i*2+1] = 0x07;
    }
    for (;;) {}
}

