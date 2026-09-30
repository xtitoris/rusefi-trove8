#include "pch.h"
#include "board_overrides.h"
#include <array>

static std::array<Gpio, 26> OUTPUTS = {
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
    return static_cast<int>(OUTPUTS.size());
}

static int boardGetMetaLowSideOutputsCount() {
	// 8 injector + 6 aux low-side outputs; the trailing 8 are ignition outputs
	return boardGetMetaOutputsCount() - 8;
}

static Gpio* boardGetMetaOutputs() {
    return OUTPUTS.empty() ? nullptr : OUTPUTS.data();
}

static int boardGetMetaDcOutputsCount() {
    return 0;
}

void setupBoardHardwareTestOverrides() {
    custom_board_getMetaOutputsCount = boardGetMetaOutputsCount;
    custom_board_getMetaLowSideOutputsCount = boardGetMetaLowSideOutputsCount;
    custom_board_getMetaOutputs = boardGetMetaOutputs;
    custom_board_getMetaDcOutputsCount = boardGetMetaDcOutputsCount;
}
