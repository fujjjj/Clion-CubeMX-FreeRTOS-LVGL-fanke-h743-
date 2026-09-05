# Clion_h7_01 — STM32H743 MP3 播放器

基于反客 FK743M1 核心板的彩色屏 **MP3 播放器**:7 寸 RGB LCD(LVGL)显示 TF 卡
文件列表,STM32H743 用 **minimp3 软件解码**,经 **SAI1 + DMA** 送给 **ES8311
CODEC**,喇叭出声。系统为 FreeRTOS(CMSIS-RTOS v2),构建用 CLion + CMake + Ninja。

> 播放链路已跑通:TF 卡扫描 → MP3 解码 → 双缓冲无缝 DMA 播放 → ES8311 出声,
> 连续流畅、无卡顿、无杂音。

## 功能特性 / Features

- 7 寸 800x480 RGB LCD + LVGL 8.3 界面,SquareLine Studio 生成的 UI
- TF 卡(FAT32/FatFs)递归扫描,过滤常见音频后缀,长文件名自动换行
- **MP3 流式解码**(minimp3,公共领域单头文件)
- **SAI1_B TX DMA 双缓冲乒乓播放**,中断内无缝切换,DMA 永不停止
- ES8311 CODEC(I2C 控制 + I2S 数据),NS4150B 功放驱动喇叭
- FreeRTOS 多任务:UI/触摸/LED 与音频解码互不干扰
- 内部 Flash + QSPI(W25Q64)双烧录,`flash.bat` 一键完成

## 硬件接线 / Wiring

### ES8311 音频模块

| 模块脚 | MCU 脚 | 功能 |
|---|---|---|
| SCL | PB10 | I2C2_SCL(AF4) |
| SDA | PB11 | I2C2_SDA(AF4) |
| MCLK | PF7 | SAI1_MCLK_B(AF6) |
| SCLK | PF8 | SAI1_SCK_B(AF6) |
| LRCK | PF9 | SAI1_FS_B(AF6) |
| DIN | PF6 | SAI1_SD_B(AF6,播放数据) |
| DOUT | 悬空 | 麦克风回传,不用 |
| 5V/GND | 板载 | 供电 |

⚠️ ES8311 I2C 地址由 CE 引脚决定:代码用 `0x18`(CE 低电平)。

### TF 卡(SDMMC1)

PC8~PC12(SDMMC1_D0~D3/CK)+ PD2(CMD),4-bit 模式,SD 时钟 10MHz
(`ClockDiv=12`,20MHz 在此板读大文件不稳定)。

## 播放架构 / Audio Pipeline

```text
TF 卡 MP3 ──> SDMMC 轮询读 ──> minimp3 解码(CPU)──> PCM 双缓冲(AXI SRAM)
                                                          │ DMA2
                                                          ▼
                               喇叭 <── NS4150B <── ES8311 <── SAI1_B(I2S)
```

关键设计:

- **CPU 只解码,不搬数据**:SD 读与 SAI 播都由硬件(DMA/控制器)完成
- **双缓冲乒乓**:两个 PCM 缓冲各 26ms,解码提前在后台做;DMA 播完一段的瞬间,
  由中断(ISR)无缝切到另一段——**DMA 永不停止**,消除段间间隙
- **就绪标志 + 信号量同步**:ISR 只播"已解好"的缓冲,任务只填"刚播完"的缓冲;
  EOF 自然收尾,欠载时干净停止
- **Cache 一致性**:解码后的 PCM 显式 `SCB_CleanDCache_by_Addr` 写回内存,
  DMA 才能读到完整数据(否则沙沙声)

## 任务划分 / Tasks

| 任务 | 优先级 | 职责 |
|---|---|---|
| `audio` | AboveNormal | 打开 MP3、解码 PCM(不碰 LVGL) |
| `defaultTask` | Normal | 文件扫描、LVGL 渲染、触摸、LED、显示播放状态 |

音频任务只写共享状态文字,由 UI 任务轮询显示,避免 LVGL 线程安全问题。

## 踩坑与修复 / Lessons Learned

这个项目几乎把 STM32H7 音频播放的经典坑都踩了一遍,记录如下(对维护者极有价值):

1. **DMA 缓冲不能放 DTCM**:DMA2(D2 域)访问 DTCM(D1 域)不可靠,传输报错。
   PCM 缓冲放在 AXI SRAM(`0x24000000`,链接脚本 `.audio_buf` 段)。
2. **DMA 中断优先级必须 ≤ FreeRTOS 上限(5)**:ISR 里调 `xSemaphoreGiveFromISR`
   有优先级约束,优先级 6 会导致信号量永远给不出去。
3. **SD 读用 DMA+消息队列会 30 秒假死**:读完成消息不达,每次读卡干等超时。
   改为**轮询读**(`HAL_SD_ReadBlocks`)后毫秒级完成。
4. **任务栈要够大**:过小触发 MemManage;defaultTask/audio 均 24KB,
   并开启栈溢出检测钩子。
5. **SAI DMA 用 Normal 而非 Circular**:播放是"一段一传"架构,Circular 下
   SAI 状态永不复位,第二次传输必失败。
6. **Cache 与 DMA 一致性**:解码写 Cache 后必须 Clean 写回,否则 DMA 读到
   旧数据 → 沙沙声。
7. **内部 Flash 与 QSPI 必须一起烧**:QSPI 素材含固件函数指针,只烧一半会
   HardFault。
8. **CubeMX 重新生成会覆盖手改**:SAI 帧格式/MCKEN、SDMMC ClockDiv、SD
   状态缓存等修复放在 USER CODE 区(或生成后补回)。

## 当前状态 / Status

**已可用**:开机自动扫描 TF 卡 → 自动播放第一首 MP3,流畅无卡顿、无杂音。

待办/下一步:

- [ ] 列表点击选择歌曲、上一首/下一首/暂停
- [ ] 音量调节(改 `es8311_voice_volume_set(dev, vol, NULL)`,0~100)
- [ ] 播放进度条 / 时间显示
- [ ] 播完自动切下一首

## 项目结构 / Project Layout

```text
CubeMX 只负责"生成代码"(实际构建用 CMake):
  Core/       main、外设初始化、FreeRTOS、中断(USER CODE 区受保护)
  Drivers/    HAL 库(CubeMX 生成的公共库)
  Middlewares FreeRTOS 内核、FatFs、minimp3 解码器

自维护部分(CubeMX 永远不碰):
  CMakeLists.txt                   构建文件(自动收编源文件)
  config/                          覆盖 hal_conf.h / FreeRTOSConfig.h
  stm32h743_qspi.ld                带 .audio_buf(AXI SRAM)段的链接脚本
  Drivers/User/                    板级驱动 + audio_player.c(播放器核心)
  LVGL/  ui/  flash.bat            老工程移植内容
```

核心文件:

- `Drivers/User/Src/audio_player.c` — 播放器核心(解码、双缓冲、DMA、任务)
- `Core/Src/freertos.c` — SD 扫描、任务启动、栈溢出钩子
- `Core/Src/sai.c` — SAI1_B 初始化(USER CODE 内含 MCKEN/帧格式修复)
- `Core/Src/sdmmc.c` / `FATFS/Target/sd_diskio.c` — SD 驱动(10MHz、轮询读)
- `Core/Src/es8311.c` — ES8311 CODEC 驱动(从 espressif 组件移植)

## 日常流程 / Daily Workflow

1. 改代码(USER CODE 区 / ui / Drivers/User)
2. 构建:CLion 点锤子,或 `cmake --preset Debug && cmake --build build/Debug`
3. 烧录:双击 `flash.bat`(Debug)或 `flash.bat Release`
   (**内部 Flash 与 QSPI 必须一起烧**)
4. 连不上板子时:按住板子复位键,回车重试,2-3 秒后松开

## 在 CubeMX 里加外设(串口/DMA/新引脚)

1. 在 CubeMX 里勾选外设、配引脚/参数
2. Generate Code(USER CODE 保留)
3. 检查手改文件是否被还原:`sai.c`(MCKEN/帧格式)、`sdmmc.c`(ClockDiv)、
   `sd_diskio.c`(SD_status 缓存),被覆盖就补回
4. 构建 → 烧录

## 烧录原理 / Flash.

`flash.bat` 调用 STM32CubeProgrammer CLI,经 ST-Link/SWD:

- 内部 Flash:`Clion_h7_01-flash.bin` @ 0x08000000
- 外部 QSPI:`Clion_h7_01-qspi.bin` @ 0x90000000(W25Q64 外部加载器)

![实际运行](https://github.com/fujjjj/Clion_h7_01/blob/main/bb7795c882a403c2e70a1e06638a1c55.jpg)

<video src="https://raw.githubusercontent.com/fujjjj/Clion-CubeMX-FreeRTOS-LVGL-fanke-h743-/main/dd123406facb8e00e71aa23e5696ffe0.mp4" controls="controls" width="100%" height="auto"></video>
