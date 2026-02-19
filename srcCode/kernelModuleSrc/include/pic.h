#ifndef PIC_H
#define PIC_H

//This is the "programmable interrupt controller" for external exceptions!! Do not fear it - feed it, love it, and it will be your loyal friend.

void pic_Remap(void);
void pic_SendEOI(unsigned char irq);

#endif
