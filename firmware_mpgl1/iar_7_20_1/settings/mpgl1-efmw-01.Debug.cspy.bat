@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


"E:\EiE\common\bin\cspybat" "E:\EiE\arm\bin\armproc.dll" "E:\EiE\arm\bin\armjlink.dll"  %1 --plugin "E:\EiE\arm\bin\armbat.dll" --device_macro "E:\EiE\arm\config\debugger\Atmel\SAM3U.dmac" --flash_loader "E:\EiE\arm\config\flashloader\Atmel\sam3u2c\sam3u2c-flash.board" --backend -B "--endian=little" "--cpu=Cortex-M3" "--fpu=None" "-p" "E:\EiE\arm\CONFIG\debugger\Atmel\ATSAM3U2C.ddf" "--semihosting" "--device=ATSAM3U2C" "--drv_communication=USB0" "--jlink_speed=auto" "--jlink_initial_speed=32" "--jlink_reset_strategy=0,0" "--drv_catch_exceptions=0x000" "--drv_swo_clock_setup=72000000,0,2000000" 


