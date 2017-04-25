// Raspberry Pi NES controller
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <bcm2835.h>

#include "gpio.h"
#include "uinput.h"

int is_polling;

void handler(int sig)
{
	is_polling = 0;
	(void) signal(SIGINT, SIG_DFL);
}

// TODO: remove function
void nes_buttons_to_stdout(nes_button *buttons, int length)
{
	int i;
	system("clear"); // TODO: remove (debug purpose only)

	for (i = 0; i < length; i++)
	{
		printf("%s\t%s\n", buttons[i].name, buttons[i].pressed ? "PRESSED" : "-");
	}
}

int main()
{
	int uinput_fd;
	int i;
	is_polling = 1;

	(void) signal(SIGINT, handler);

	nes_button buttons[BUTTON_COUNT] =
	{
	  {"A",		KEY_A,		0},
	  {"B",		KEY_S,		0},
	  {"Select",	KEY_Z,		0},
	  {"Start",	KEY_X,		0},
	  {"Up",	KEY_UP,		0},
	  {"Down",	KEY_DOWN,	0},
	  {"Left",	KEY_LEFT,	0},
	  {"Right",	KEY_RIGHT,	0}
	};

	// Initialize uinput
	if (!(uinput_fd = uinput_init()))
	{
		printf("Could not initialize uinput!");
		exit(1);
	}

	// Initialize BCM2835
	if (!bcm2835_init())
	{
		printf("Could not initialize BCM2835!");
		exit(1);
	}

	// Initialize GPIO pins
	if (!gpio_init())
	{
		printf("Could not initialize GPIO pins!");
		exit(1);
	}

	// Polling loop
	while (is_polling)
	{
		// Send latch signal
		bcm2835_gpio_write(LATCH_PIN, HIGH);
		delay(DURATION_LATCH_SIGNAL_IN_MS);
		bcm2835_gpio_write(LATCH_PIN, LOW);

		// Read status of each button
		for (i = 0; i < BUTTON_COUNT; i++)
		{
			buttons[i].pressed = !bcm2835_gpio_lev(DATA_PIN);

			delayMicroseconds(DURATION_PULSE_SIGNAL_IN_US);
			bcm2835_gpio_write(PULSE_PIN, HIGH);
			delayMicroseconds(DURATION_PULSE_SIGNAL_IN_US);
			bcm2835_gpio_write(PULSE_PIN, LOW);
		}

		//nes_buttons_to_stdout(buttons, BUTTON_COUNT);
		nes_buttons_to_uinput(uinput_fd, buttons, BUTTON_COUNT);
		uinput_sync(uinput_fd);

		delay(DURATION_POLLING_IN_MS);
	}

	printf("Closing...\n");
	gpio_close();
	bcm2835_close();
	uinput_close(uinput_fd);

	return 0;
}
