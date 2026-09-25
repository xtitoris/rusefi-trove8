#include "pch.h"
#include "board_overrides.h"

// trove8 custom ECU, STM32F427VGTx, based on a "uaefi-shaped" hardware layout:
//   8x VNLD5160 low-side injector drivers (U8..U11)
//   8x SN74LVC4245 level-shifted ignition outputs via polyfuses (U7)
//   6x VNLD5160 aux low-side outputs (U12/U13/U27)
//   2x knock channels via RC filter + MCP6004 (PA2, PC1 - see knock_config.h)
//   on-board MEMS (PB6), SD card (PB7);

Gpio getCommsLedPin() {
	return Gpio::D4; // LED1
}

Gpio getRunningLedPin() {
	return Gpio::D7; // LED2
}

Gpio getWarningLedPin() {
	return Gpio::E7; // LED3
}

static void trove8SetupSpi() {
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->spi1sckPin = Gpio::B3;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->sdCardCsPin = Gpio::B7;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	engineConfiguration->isSdCardEnabled = true;
}

static void trove8SetupAnalogSensors() {
	// 12V/crank/cam sense etc all buffered through MCP6004 op-amps off a 2:1 (4.7k/4.7k) divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;
	// VBAT sense: 48.7k/10k divider (R96/R97) -> MCP6004 buffer (U29B) -> 2:1 divider
	// (R101/R105) to PA5 => total 11.74:1, full-scale 29.35V at 2.5V VREF
	engineConfiguration->vbattDividerCoeff = (48.7 + 10.0) / 10.0 * 2.0;
	engineConfiguration->adcVcc = 2.5f; // VREF+ (U26-21) = 2.5V from buffered 5VA/2 divider, not VDDA
	engineConfiguration->vbattAdcChannel = EFI_ADC_5; // VIGN sense on PA5

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10; // PC0
	engineConfiguration->clt.adcChannel = EFI_ADC_12;       // PC2
	engineConfiguration->iat.adcChannel = EFI_ADC_13;       // PC3

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4; // PA4
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_8; // PB0

	// pedal (PPS1/PPS2) and O2 are deliberately unassigned by default - assign
	// in TunerStudio once wiring is decided. PPS channels: PA3 (EFI_ADC_3), PC4 (EFI_ADC_14)

	// PA1/PA0 exposed as generic aux analog inputs instead of narrowband AFR
	engineConfiguration->auxAnalogInputs[0] = EFI_ADC_1; // PA1
	engineConfiguration->auxAnalogInputs[1] = EFI_ADC_0; // PA0
}

// aux low-side outputs (PWM1..PWM6) are unassigned by default - assign
// fuel pump / main relay / fan etc. in TunerStudio once wiring is decided

// board-specific configuration setup
static void customBoardDefaultConfiguration() {
	engineConfiguration->injectionPins[0] = Gpio::D3;  // INJ1
	engineConfiguration->injectionPins[1] = Gpio::A9;  // INJ2
	engineConfiguration->injectionPins[2] = Gpio::D11; // INJ3
	engineConfiguration->injectionPins[3] = Gpio::D10; // INJ4
	engineConfiguration->injectionPins[4] = Gpio::D2;  // INJ5
	engineConfiguration->injectionPins[5] = Gpio::A8;  // INJ6
	engineConfiguration->injectionPins[6] = Gpio::D15; // INJ7
	engineConfiguration->injectionPins[7] = Gpio::D12; // INJ8
	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->ignitionPins[0] = Gpio::C13; // IGN1
	engineConfiguration->ignitionPins[1] = Gpio::E5;  // IGN2
	engineConfiguration->ignitionPins[2] = Gpio::E4;  // IGN3
	engineConfiguration->ignitionPins[3] = Gpio::E3;  // IGN4
	engineConfiguration->ignitionPins[4] = Gpio::E2;  // IGN5
	engineConfiguration->ignitionPins[5] = Gpio::B8;  // IGN6
	engineConfiguration->ignitionPins[6] = Gpio::B9;  // IGN7
	engineConfiguration->ignitionPins[7] = Gpio::E6;  // IGN8
	engineConfiguration->ignitionPinMode = OM_DEFAULT;

	// external hall/digital conditioned trigger - crank on MCU_IN_D1 (PE12), cam on MCU_IN_D2 (PE13)
	engineConfiguration->triggerInputPins[0] = Gpio::E12;
	engineConfiguration->camInputs[0] = Gpio::E13;

	trove8SetupAnalogSensors();
	trove8SetupSpi();

	// on-board knock (RC filter + MCP6004), two channels
	engineConfiguration->enableSoftwareKnock = true;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	setCrankOperationMode();
	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

#ifndef EFI_BOOTLOADER
	// CLT/IAT conditioned through 4.7k bias + 2:1 divider, numbers below get
	// refined during calibration
	setCommonNTCSensor(&engineConfiguration->clt, 4700);
	setCommonNTCSensor(&engineConfiguration->iat, 4700);
#endif // EFI_BOOTLOADER
}

static Gpio OUTPUTS[] = {
	Gpio::D3,  // INJ1
	Gpio::A9,  // INJ2
	Gpio::D11, // INJ3
	Gpio::D10, // INJ4
	Gpio::D2,  // INJ5
	Gpio::A8,  // INJ6
	Gpio::D15, // INJ7
	Gpio::D12, // INJ8
	Gpio::D13, // PWM1 (aux low side)
	Gpio::C6,  // PWM2 (aux low side)
	Gpio::C7,  // PWM3 (aux low side)
	Gpio::C8,  // PWM4 (aux low side)
	Gpio::C9,  // PWM5 (aux low side)
	Gpio::D14, // PWM6 (aux low side)
	Gpio::C13, // IGN1 level-shifted to connector
	Gpio::E5,  // IGN2
	Gpio::E4,  // IGN3
	Gpio::E3,  // IGN4
	Gpio::E2,  // IGN5
	Gpio::B8,  // IGN6
	Gpio::B9,  // IGN7
	Gpio::E6,  // IGN8
};

static int boardGetMetaOutputsCount() {
	return efi::size(OUTPUTS);
}

static int boardGetMetaLowSideOutputsCount() {
	// 8 injector + 6 aux low-side outputs; the trailing 8 are ignition outputs
	return getBoardMetaOutputsCount() - 8;
}

static Gpio* boardGetMetaOutputs() {
	return OUTPUTS;
}

void setup_custom_board_overrides() {
	custom_board_DefaultConfiguration = customBoardDefaultConfiguration;
	custom_board_getMetaOutputsCount = boardGetMetaOutputsCount;
	custom_board_getMetaLowSideOutputsCount = boardGetMetaLowSideOutputsCount;
	custom_board_getMetaOutputs = boardGetMetaOutputs;
}