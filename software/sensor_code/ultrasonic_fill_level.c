#include <stdint.h>

#define TRIG_PIN   5
#define ECHO_PIN   6

#define BIN_DEPTH_CM  80
#define THRESHOLD_PCT 80

void gpio_set(uint8_t pin, uint8_t value);
uint8_t gpio_read(uint8_t pin);
void delay_us(uint32_t us);
uint32_t timer_get_us(void);

uint8_t get_fill_level_percent(void)
{
    uint32_t start, end;
    uint32_t duration;
    uint32_t distance_cm;
    uint8_t fill_pct;

    gpio_set(TRIG_PIN, 0);
    delay_us(2);
    gpio_set(TRIG_PIN, 1);
    delay_us(10);
    gpio_set(TRIG_PIN, 0);

    while (!gpio_read(ECHO_PIN));
    start = timer_get_us();

    while (gpio_read(ECHO_PIN));
    end = timer_get_us();

    duration = end - start;
    distance_cm = duration / 58;

    if (distance_cm >= BIN_DEPTH_CM)
        return 0;

    fill_pct = ((BIN_DEPTH_CM - distance_cm) * 100) / BIN_DEPTH_CM;
    return fill_pct;
}

uint8_t is_bin_near_full(void)
{
    uint8_t fill = get_fill_level_percent();
    return (fill >= THRESHOLD_PCT);
}