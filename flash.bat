@echo off
chcp 65001 >nul
rem One-click flash for the CubeMX+FreeRTOS+LVGL project (Clion_h7_01).
rem Programs internal flash + QSPI W25Q64 (UI images/fonts).
rem Requires: STM32CubeProgrammer CLI, connected ST-Link, build first.
setlocal

rem Build configuration: Debug by default, pass Release as argument.
set "CFG=Debug"
if not "%1"=="" set "CFG=%1"

set "PROG=STM32_Programmer_CLI"
where STM32_Programmer_CLI >nul 2>nul
if errorlevel 1 set "PROG=D:\cubeCLT\STM32CubeCLT_1.22.0\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"

set "BIN=%~dp0build\%CFG%\Clion_h7_01-flash.bin"
set "QSPI=%~dp0build\%CFG%\Clion_h7_01-qspi.bin"
set "LOADER_SRC=%~dp0tools\fk743m1_w25q64.stldr"
set "LOADER=%TEMP%\fk743m1_w25q64.stldr"

if not exist "%BIN%" (
  echo [ERROR] %BIN% not found. Build the project first. Config: %CFG%
  pause
  exit /b 1
)
if not exist "%QSPI%" (
  echo [ERROR] %QSPI% not found. Build the project first.
  pause
  exit /b 1
)
if not exist "%LOADER_SRC%" (
  echo [ERROR] W25Q64 external loader not found: %LOADER_SRC%
  pause
  exit /b 1
)

echo.
echo 提示：如果连接失败（Unable to get core ID / Cannot connect to access port /
echo       DEV_TARGET_NOT_HALTED），请按住板子上的复位键，然后按回车重试，
echo       出现连接进度后 2-3 秒再松开复位键。
echo.

rem CLion often leaves ST-LINK_gdbserver running, which holds the ST-Link and
rem blocks programming. Close it first so the probe is free.
echo [0/3] Closing stale ST-LINK_gdbserver if present...
taskkill /IM ST-LINK_gdbserver.exe /F >nul 2>&1
timeout /t 1 /nobreak >nul
tasklist /FI "IMAGENAME eq ST-LINK_gdbserver.exe" 2>nul | find /I "ST-LINK_gdbserver.exe" >nul
if not errorlevel 1 (
  echo.
  echo [ERROR] ST-LINK_gdbserver is still running and holding the ST-Link.
  echo         Close CLion completely, or run this file as administrator.
  pause
  exit /b 1
)

copy /y "%LOADER_SRC%" "%LOADER%" >nul

rem ---- Program internal flash (auto-retry up to 3 times) ----
set "ATTEMPT=0"
:retry_int
set /a ATTEMPT+=1
echo [1/3] Programming internal flash at 0x08000000 (attempt %ATTEMPT%/3)...
"%PROG%" -c port=SWD mode=NORMAL -w "%BIN%" 0x08000000 -v
if errorlevel 1 (
  if %ATTEMPT% lss 3 (
    echo.
    echo 连接失败（第 %ATTEMPT% 次）。按住板子复位键，按回车重试...
    pause
    goto retry_int
  )
  echo.
  echo [ERROR] Internal flash programming failed after 3 attempts.
  echo         连不上时：按住板子复位键，按回车，2-3 秒后松开。
  pause
  exit /b 1
)

rem ---- Program QSPI W25Q64 (auto-retry up to 3 times) ----
set "ATTEMPT=0"
:retry_qspi
set /a ATTEMPT+=1
echo [2/3] Programming QSPI W25Q64 at 0x90000000 (attempt %ATTEMPT%/3)...
echo         This writes the UI images/fonts and can take a few minutes.
"%PROG%" -c port=SWD mode=NORMAL -el "%LOADER%" -w "%QSPI%" 0x90000000 -v -rst
if errorlevel 1 (
  if %ATTEMPT% lss 3 (
    echo.
    echo QSPI 连接失败（第 %ATTEMPT% 次）。按住板子复位键，按回车重试...
    pause
    goto retry_qspi
  )
  echo.
  echo [ERROR] QSPI programming failed after 3 attempts.
  echo         连不上时：按住板子复位键，按回车，2-3 秒后松开。
  pause
  exit /b 1
)

echo [3/3] Done. Board reset and should now be running the firmware.
pause
exit /b 0