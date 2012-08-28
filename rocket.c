// ****************************************
//
// rocket.c
// Launch rockets from a TI Chronos watch
//
// 2012 CC-BY-SA Nathan Plamondon
//
// ****************************************

#include <openchronos.h>
#include <drivers/display.h>
#include <drivers/buzzer.h>

static void rocket_arm();
static void rocket_disarm();
static void rocket_launch();

static union {
	uint8_t armed:1;
} rocket_state;

static void up_pressed()
{
	rocket_disarm();
}

static void down_pressed()
{
	rocket_disarm();
}

static void up_down_pressed()
{
	rocket_arm();
}

static void num_long_pressed()
{
	if (rocket_state.armed) {
		// launch
		rocket_launch();
		rocket_disarm();
	}
}

static void rocket_arm()
{
	display_chars(0, LCD_SEG_L1_3_0, (uint8_t *)" ARM", SEG_SET);
	display_chars(0, LCD_SEG_L1_3_0, (uint8_t *)"8888", BLINK_ON);

	// TODO send arm signal, wait for return
	display_chars(0, LCD_SEG_L1_3_0, (uint8_t *)"8888", BLINK_OFF);
	display_chars(0, LCD_SEG_L2_4_0, (uint8_t *)"LAUNCH", SEG_SET);
	
	rocket_state.armed = 1;

	// TODO warble buzzer
	// This is problematic in buzzer.c
	buzzer_start(10, 100, 100);
}

static void rocket_disarm()
{
	// TODO send safe signal, wait for return
	display_chars(0, LCD_SEG_L1_3_0, (uint8_t *)"SAFE", SEG_SET);
	display_chars(0, LCD_SEG_L1_3_0, (uint8_t *)"8888", BLINK_OFF);
	display_clear(0, 2);

	rocket_state.armed = 0;

	buzzer_stop();
}

static void rocket_launch()
{
	// TODO send launch code, wait for return
	rocket_disarm();
}

static void rocket_activated()
{
	rocket_disarm();
	display_chars(0, LCD_SEG_L2_4_0, (uint8_t *)"ROCKT", SEG_SET);
}

static void rocket_deactivated()
{
	/* clean up screen */
	display_clear(NULL, 1);
}

void rocket_init(void)
{
	menu_add_entry(
			&up_pressed,
			&down_pressed,
			NULL, NULL,
			&num_long_pressed,
			&up_down_pressed,
			&rocket_activated,
			&rocket_deactivated);
}
