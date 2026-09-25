/**
 * @file knock_config.h
 *
 * trove8: two knock channels on the STM32F427 (ADC3)
 * channel: RC band-pass filter + MCP6004 gain stage -> 4k7 series -> MCU pin
 * knock 1: -> PA2 (ADC3 channel 2)
 * knock 2: -> PC1 (ADC3 channel 11)
 *
 * NOTE: this board has no MAX9927 knock conditioner; knock filtering is done
 * with discrete RC + MCP6004 only. The EFI_SOFTWARE_KNOCK sampling config
 * below matches the signal chain on ADC3.
 *
 * Rev1 board caveat: knock 2 was originally routed to PA6, but PA6 is not
 * available on ADC3 on the STM32F4 (it is ADC1/ADC2-only), while ADC1/ADC2
 * are taken by the slow/fast ADC subsystems. Knock 2 must therefore be
 * bodged to PC1 (ADC3 channel 11) on the first batch of boards.
 */

#pragma once

// Knock is on ADC3
#define KNOCK_ADC ADCD3

// knock 1 - pin PA2
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN2
#define KNOCK_PIN_CH1 Gpio::A2

// knock 2 - pin PC1 (ADC3 channel 11)
#define KNOCK_HAS_CH2 true
#define KNOCK_ADC_CH2 ADC_CHANNEL_IN11
#define KNOCK_PIN_CH2 Gpio::C1

// Sample rate & time - depends on the exact MCU
#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))