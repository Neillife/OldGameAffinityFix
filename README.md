# OldGameAffinityFix

OldGameAffinityFix is a non-invasive tool designed to improve performance for old games on Intel CPU (13th or 14th).

Inspired by Process Lasso, which demonstrates the benefits of adjusting process affinity and priority,  
This tool applies a similar concept to old games, reducing lag caused by high-frequency loops. (like old MapleStory)

## How it works

- The tool automatically detects Intel hybrid CPUs (P-core + E-core architecture).  
- It calculates the optimal performance-core (P-core) affinity mask to pin the process to the high-performance cores.  
- It also increases the process priority to reduce interruptions from other threads or background tasks.  
- On AMD or non-hybrid CPUs, it safely only adjusts process priority without restricting CPU cores.  
- The tool does **not** modify any game or system binaries (non-invasive).
    - > No game files are modified
    - > No hooks or injections into game logic
    - > No memory editing
    - > No gameplay or logic changes
    - > This tool only adjusts CPU affinity and process priority, similar to Process Lasso.

## Usage

After the target process is running, use the `LoadLibrary` API or other DLL injection methods to load the DLL.

The DLL only calls standard Windows APIs (`SetProcessAffinityMask` and `SetPriorityClass`) to adjust CPU affinity and process priority.

## Features

- Targets old games and old applications.
- Automatically applies optimal P-core affinity on Intel hybrid CPUs.
- Adjusts process priority for smoother performance.
- Safe and non-invasive for all other CPUs.

This approach improves performance for old games that were not designed with hybrid CPUs in mind, especially Intel 13th and 14th CPUs, by avoiding performance drops caused by threads being scheduled on efficiency cores (E-cores).
