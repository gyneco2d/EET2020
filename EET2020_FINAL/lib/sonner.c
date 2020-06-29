/*
 * sonner.c
 *
 * Created: 2020/06/29 5:37:58
 *  Author: gyneco2d
 */ 

#include <atmel_start.h>

#define HIGH true
#define LOW  false

static bool is_first = true;
static struct timer_task TIMER_0_task1;
static int distance = 0;

int getDistance()
{
	return distance;
}

static void sonner_timer_cb()
{
	gpio_set_pin_level(PIN_PB07, HIGH);
	delay_us(10);
	gpio_set_pin_level(PIN_PB07, LOW);
}

static void sonner_timer_init(void)
{
	TIMER_0_task1.interval = 500;
	TIMER_0_task1.cb       = sonner_timer_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	
	timer_add_task(&TIMER_0, &TIMER_0_task1);
}

static void interrupt_PB06_cb()
{
	if (is_first) {
		pwm_enable(&PWM_0);
	} else {
		uint32_t usec = hri_tccount32_read_COUNT_reg(TC6);
		pwm_disable(&PWM_0);
		int dist = (int)usec * (340.0 / 1000.0) / 2;
		distance = dist;
		// printf("distance: %d.%dcm\n", dist/10, dist%10);
	}
	is_first = !is_first;
}

static void interrupt_PB06_init()
{
	gpio_set_pin_pull_mode(PIN_PB06, GPIO_PULL_DOWN);
	// Mask interrupt
	ext_irq_disable(PIN_PB06);
	ext_irq_register(PIN_PB06, interrupt_PB06_cb);
	// Both-edge detection
	hri_eic_clear_CONFIG_reg(EIC, 0, EIC_CONFIG_SENSE6_Msk);
	hri_eic_set_CONFIG_reg(EIC, 0, EIC_CONFIG_SENSE6_BOTH);
	// Accept interrupt
	ext_irq_enable(PIN_PB06);
}

static void trig_pin_init(void)
{
	gpio_set_pin_level(PIN_PB07, LOW);
	gpio_set_pin_direction(PIN_PB07, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PIN_PB07, GPIO_PIN_FUNCTION_OFF);
}

void sonner_init()
{
	trig_pin_init();
	interrupt_PB06_init();
	sonner_timer_init();
}
