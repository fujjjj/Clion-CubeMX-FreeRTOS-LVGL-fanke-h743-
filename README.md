# Clion_h7_01 — CubeMX + FreeRTOS + LVGL   (fanke反客h743)

## 项目结构 / How it is organized

```text
CubeMX 只负责"生成代码"（工具链设为 Makefile，实际构建用 CMake）：
  Core/       main、外设初始化、FreeRTOS、中断（USER CODE 区受保护）
  Drivers/    HAL 库（CubeMX 生成的公共库）
  Middlewares FreeRTOS 内核

自维护部分（CubeMX 永远不碰）：
  CMakeLists.txt                  我们自己的构建文件（自动收编 Core/Src 新文件）
  CMakePresets.json               Debug / Release 预设
  cmake/arm-none-eabi-gcc.cmake   工具链
  stm32h743_qspi.ld               带 .qspi_ui 段的链接脚本
  config/                         覆盖 hal_conf.h / FreeRTOSConfig.h（include 优先）
  LVGL/  ui/  Drivers/User/  flash.bat  老工程移植内容
```

## 关键约定 / Key rules

- **LTDC、SDRAM、QSPI、触摸** 由 `Drivers/User/` 手写驱动负责（自带 MSP 初始化），
  **不进 .ioc、不在 CubeMX 里配**。CubeMX 生成的 `ltdc.c` 会被构建自动排除。
- **config/ 优先**：include 顺序里 `config/` 在最前，所以我们的
  `stm32h7xx_hal_conf.h`（SDRAM/QSPI/DMA2D/UART 使能）和
  `FreeRTOSConfig.h`（堆 64KB）永远生效，重新生成不会覆盖。
- **默认任务栈 4096 字**写在 `.ioc`（FreeRTOS 任务配置）里，重新生成保持。

## 日常流程 / Daily workflow

1. 改代码（USER CODE 区 / ui / Drivers/User）
2. 构建：CLion 选 Debug/Release，点锤子；或 `cmake --preset Debug && cmake --build build/Debug`
3. 烧录：双击 `flash.bat`（Debug）或 `flash.bat Release`
4. 连不上板子时：按住板子复位键，回车重试，2-3 秒后松开

## 在 CubeMX 里加外设（串口/DMA/新引脚）— 现在安全了

1. 在 CubeMX 里勾选外设、配引脚/参数
2. Generate Code（USER CODE 保留；构建文件不再被覆盖）
3. 构建 → 烧录

注意：新外设的 HAL 驱动若不在构建里（链接报未定义），把它加到
`CMakeLists.txt` 的 `HAL_SRC` 列表即可（一般同系列 .c 都已存在）。

## 烧录原理 / Flash

`flash.bat` 调用 STM32CubeProgrammer CLI，经 ST-Link/SWD：
- 内部 Flash：`Clion_h7_01-flash.bin` @ 0x08000000
- 外部 QSPI：`Clion_h7_01-qspi.bin` @ 0x90000000（外部加载器驱动 W25Q64）
![实际运行](https://github.com/fujjjj/Clion_h7_01/blob/main/bb7795c882a403c2e70a1e06638a1c55.jpg)