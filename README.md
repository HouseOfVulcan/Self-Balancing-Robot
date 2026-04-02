# STM32 Self-Balancing Robot

An embedded systems course project — a two-wheeled self-balancing robot built on the **Yahboom** platform using an **STM32F103RCT6** microcontroller. The firmware is written from scratch module-by-module using the Yahboom source as a reference implementation.

---
## Project Status

| Module | Status |
|---|---|
| Architecture | ✅ Complete |
| Motors (PWM) | ✅ Complete |
| Encoders | ✅ Complete |
| OLED Display | ✅ Complete |
| IMU / DMP | 🔧 In Progress |
| PID Control | ⬜ Not Started |
| PS2 Wireless | ⬜ Not Started |

---

## Hardware

| Component | Details |
|---|---|
| MCU | STM32F103RCT6 (Cortex-M3, 72 MHz) |
| IMU | MPU-6050 (gyro + accelerometer) |
| Motors | DC motors with quadrature encoders |
| Display | SSD1306 OLED (I²C, 128×64) |
| Controller | PS2 wireless gamepad |
| Flashing | FlyMCU via CH340K USB-serial |
| Debugger | ST-LINK SWD (clone) |

---

## Firmware Architecture

The firmware is structured as layered BSP + application modules built on top of STM32 HAL:
```
main.c
├── BSP Layer
│   ├── bsp_timer.c      — TIM6 10ms tick, TIM8 PWM
│   ├── bsp_oled.c       — SSD1306 display driver
│   ├── bsp.c            — Board support init
│   └── delay.c          — us/ms delay utilities
├── Sensor
│   ├── IOI2C.c/h        — Bit-bang software I²C (PB8/9 OLED, PB10/11 IMU)
│   ├── mpu6050.c        — MPU-6050 register interface
│   ├── inv_mpu.c        — InvenSense DMP driver
│   └── inv_mpu_dmp_motion_driver.c — DMP firmware loader
├── Motion
│   ├── motor.c          — TIM8 PWM motor control
│   ├── encoder.c        — TIM3/TIM4 quadrature encoder read
│   └── pid_control.c    — Balance PD + Velocity PI + Turn PD
└── Input
    └── bsp_ps2.c        — PS2 wireless controller
```

---

## Key Peripherals

| Peripheral | Function | Pins |
|---|---|---|
| TIM8 | PWM motor drive (ARR=7199) | PC6/7/8/9 |
| TIM3 | Left encoder (ARR=65535) | PA6/PA7 |
| TIM4 | Right encoder (ARR=65535) | PB6/PB7 |
| I²C (bit-bang) | OLED | PB8 (SCL) / PB9 (SDA) |
| I²C (bit-bang) | MPU-6050 IMU | PB10 (SCL) / PB11 (SDA) |
| EXTI PC12 | DMP data-ready interrupt | PC12 |
| TIM6 | 10ms housekeeping tick | — |
| USART | Debug / Bluetooth | — |
| ADC | Battery voltage monitor | — |

> **Note:** Hardware I²C is intentionally avoided. A TIM4 initialization glitch on PB8 latches the STM32F1 hardware I²C peripheral into a permanent BUSY state. Both I²C buses use software bit-bang drivers instead.

---

## Angle Estimation

The MPU-6050 is driven via InvenSense's **DMP** (Digital Motion Processor), which runs an onboard sensor fusion algorithm and outputs quaternions via FIFO at 200 Hz. Roll and Pitch angles are computed from the quaternion output and displayed on the OLED.

`GET_Angle_Way` selects the algorithm:
- `1` — DMP quaternion (primary)
- `2` — Kalman filter
- `3` — Complementary filter

---

## PID Control

Three cascaded controllers run on the 10ms tick:

1. **Balance PD** — drives motors to maintain upright angle
2. **Velocity PI** — integrates wheel speed error to correct steady-state lean
3. **Turn PD** — handles yaw from PS2 input or Bluetooth command

---

## Build & Flash

1. Open project in **STM32CubeIDE**
2. Build (`Ctrl+B`)
3. Flash using **FlyMCU**:
   - Port: COM port (CH340K)
   - Baud: 115200
   - DTR low → reset; RTS high → bootloader entry
   - Load `.hex` from `Debug/` output folder

---



## Known Issues / Design Notes

- **SDA/SCL swap on IMU bus:** PB10/PB11 are physically swapped on the PCB; the bit-bang macros account for this intentionally.
- **`NVIC_SystemReset()` in DMP init:** Removed to prevent infinite reset loop on WHO_AM_I failure during debugging.
- **ST-LINK SWD non-functional:** Suspected PB3/JTDO conflict in CubeMX pin configuration; OLED used as primary debug output.

---

