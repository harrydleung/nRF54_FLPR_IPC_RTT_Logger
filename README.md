# nRF54 IPC FLPR Logger for RTT

Although you can attach the VS Code debugger or Ozone to FLPR (VPR), debugging it is still pretty messy and intrusive. One option is to use a UART for debug log output, but this requires reserving pins on your PCB. If you cannot spare those pins or prefer another way to view FLPR logs, this approach may be useful.

This is a part of my graduate project, where I want to utilise the FLPR to control a magnetometer.

The core is pretty simple: it forwards Zephyr logs from the FLPR RISC-V core to the CPUAPP Arm core over IPC, then prints them through CPUAPP's SEGGER RTT console. In order to capture messages as soon as the program enters `main()` (the Zephyr initialisation stage will be skipped, as it needs IPC), the program will wait until the remote IPC endpoint is bound. 

The purpose is to make FLPR log messages visible with the main core logs in one terminal via RTT.

## How it works

```text
FLPR: LOG_INF(...)
        |
        v
Custom Zephyr logging backend
  Format text with timestamp and level
        |
        v
IPC service / ICBMSG / shared RAM
        |
        v
CPUAPP receive callback
  printk("FLPR DBG:...")
        |
        v
CPUAPP SEGGER RTT console
```

The IPC endpoint is named `dbgmsg_ep` on both cores. FLPR waits for the endpoint to bind before entering its periodic logging loop. This implementation transports formatted text; it does not use Zephyr's built-in multidomain logging protocol.

## Requirements

- nRF Connect SDK v3.4.1 and its environment
- A nRF54L15 board.
- A J-Link (if you use your own custom board) and its software so you can use the RTT terminal.


Board targets:

| Image | Target |
| --- | --- |
| CPUAPP | `debug_flpr_ipc/nrf54l15/cpuapp` |
| FLPR | `debug_flpr_ipc/nrf54l15/cpuflpr` |

Review the board files before using different hardware. By the way, this board file makes FLPR run the program in FLASH, so an additional step will be required after you build it.

## Step

1. Build the program.
 2. Even though it uses sysbuild, you will still need to flash the `FLPR_FW` target first, so the image will be programmed in the flash. Otherwise, there is no executable in the flash. 
 3. Open the RTT terminal and observe the result.
 4. Have fun and do what you want to do.
  
  
  
