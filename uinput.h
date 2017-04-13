#ifndef _UINPUT_H_
#define _UINPUT_H_

#include <linux/input.h>

#define BUTTON_COUNT 8

typedef struct
{
	char *name;
	int keycode;
	int pressed;
} nes_button;

int uinput_init();
int uinput_close();
int nes_buttons_to_uinput(int uinput_fd, nes_button *buttons, int length);

#endif
