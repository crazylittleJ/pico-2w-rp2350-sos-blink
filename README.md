# sos_blink — Pico 2 W FreeRTOS SOS LED demo

A FreeRTOS task running on a Raspberry Pi Pico 2 W (RP2350) that blinks the
onboard LED (wired to the CYW43439 wifi chip) in an SOS Morse pattern.

## Dependencies

- [pico-sdk](https://github.com/raspberrypi/pico-sdk) >= 2.1.0 (needs RP2350 support)
- [raspberrypi/FreeRTOS-Kernel](https://github.com/raspberrypi/FreeRTOS-Kernel)
  (note: NOT the official FreeRTOS/FreeRTOS-Kernel repo, which currently has no
  RP2350_ARM_NTZ port)
- arm-none-eabi-gcc toolchain

Both are vendored as git submodules under [`pico-sdk/`](pico-sdk) and
[`FreeRTOS-Kernel/`](FreeRTOS-Kernel), pinned to the commits this project is
built against. Clone with submodules:

```bash
git clone --recursive <this-repo-url>
```

Or, if you already cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

## Build

```bash
export PICO_SDK_PATH=$(pwd)/pico-sdk
export FREERTOS_KERNEL_PATH=$(pwd)/FreeRTOS-Kernel

mkdir build && cd build
cmake -DPICO_BOARD=pico2_w ..
make -j$(nproc)
```

(`PICO_SDK_PATH` / `FREERTOS_KERNEL_PATH` can instead point at any other
checkout if you want to build against a different version.)

Flashing: drag the resulting `sos_blink.uf2` onto a Pico 2 W in BOOTSEL mode.

## Viewing logs

Defaults to UART0 (GP0 TX / GP1 RX, 115200 baud). Connect a USB-UART adapter:

```bash
screen /dev/ttyUSB0 115200
```

## Gotchas already handled in this config

The RP2350_ARM_NTZ FreeRTOS port requires quite a few more mandatory settings
than the RP2040 port. Getting these wrong in `FreeRTOSConfig.h` causes either a
build failure or a silent runtime crash:

- `configENABLE_FPU` / `configENABLE_MPU` / `configENABLE_TRUSTZONE` — missing
  any one of these triggers an immediate `#error`
- `configRUN_FREERTOS_SECURE_ONLY` — not guarded by an `#error`, but omitting
  it means the very first context switch in `vTaskStartScheduler()` takes the
  wrong exception vector
- `configUSE_TIMERS` + `INCLUDE_xTimerPendFunctionCall` — this port's doorbell
  interrupt / spinlock notification mechanism depends on the software timer
  task internally; enabling only one of the two still fails at link time
  (`xTimerPendFunctionCallFromISR` undefined)
- `vApplicationStackOverflowHook` / `vApplicationMallocFailedHook` — once you
  enable the corresponding `configCHECK_FOR_STACK_OVERFLOW` /
  `configUSE_MALLOC_FAILED_HOOK`, you must supply the implementation yourself;
  the kernel has no default

## License

- pico-sdk: BSD 3-Clause (Raspberry Pi Ltd)
- FreeRTOS-Kernel: MIT (Amazon Web Services, Inc.)

This example code itself is free for you to use, modify, and distribute.
