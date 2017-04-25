#include <stdio.h>
#include <linux/uinput.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "uinput.h"

int uinput_init()
{
	int i;
	int uinput_fd;
	struct uinput_user_dev uidev;

	uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if (uinput_fd < 0)
	{
		return -1;
	}

	ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinput_fd, UI_SET_EVBIT, EV_REP);

        for (i = 0; i < 256; i++) {
		ioctl(uinput_fd, UI_SET_KEYBIT, i);
        }

	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "nes-controller");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 1;

	if (write(uinput_fd, &uidev, sizeof(uidev)) < 0)
	{
		return -1;
	}

	if (ioctl(uinput_fd, UI_DEV_CREATE) < 0)
	{
		return -1;
	}

	return uinput_fd;
}

int uinput_close(int uinput_fd)
{
	if (ioctl(uinput_fd, UI_DEV_DESTROY) < 0)
	{
		fprintf(stderr, "Could not destroy input fd");
		return -1;
	}

	return close(uinput_fd);
}

int uinput_sync(int uinput_fd)
{
	struct input_event ev;
	memset(&ev, 0, sizeof(ev));

	ev.type = EV_SYN;
	ev.code = 0;
	ev.value = 0;

	if (write(uinput_fd, &ev, sizeof(ev)) < 0)
	{
		return -1;
	}

	return 1;
}

int nes_buttons_to_uinput(int uinput_fd, nes_button *buttons, int length)
{
	struct input_event ev;
	int i;

	for (i = 0; i < length; i++)
	{
		memset(&ev, 0, sizeof(ev));

		ev.type = EV_KEY;
		ev.code = buttons[i].keycode;
		ev.value = buttons[i].pressed;

		if (write(uinput_fd, &ev, sizeof(ev)) < 0)
		{
			return -1;
		}
	}

	return 1;
}
