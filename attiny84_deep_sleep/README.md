# Deep Sleep

This brings the power consumption below 1 mA. Check the datasheet for supported power modes.

Useful documentation: https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html, https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html.

With `SLEEP_MODE_PWR_DOWN`, `sleep_mode();` disables the clock and therefore Arduino's `millis()` isn't counted up during that time.