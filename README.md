# NAND2Tetris Implementation in C++
___
My self-learning implementation of the NAND2Tetris course projcts created by Noam Nisan and Shimon Schocken (via https://www.nand2tetris.org/).

I did this out of a keen interest in how computers work from the hardware level all the way up to how programs we write in high-level language are executed on the hardware, and this free course was the perfect opportunity to get hands-on experience while practicing C++.

I highly suggest it to anyone who also has an interest in understanding lower level computing with:
1. Starting from logic chip design
2. Designing a fully functional CPU and memory bank
3. Translating Assembly programs into equivalent machine-readable binary opcode
4. Creating a portable instruction set similar to Java's JVM
5. Parsing and compiling a custom high-level language down to functional machine-readable code
6. Designing the high-level Operating System that further helps abstract certain processes from the main program being written

## How to Run Programs
___

The programs are mainly run on the official web IDE of the NAND2Tetris project (https://nand2tetris.github.io/web-ide/chip/). Each chapter corresponds do a certain set of emulators:

- Chapters 1-5 are executed and tested in the Hardware Simulator, each building up to create a working CPU with a set of basic ALU operations
- Chapters 6-12 are combined as a single application, but correspond to different sections of the compilation scheme
    - In the Parsing folder of the software programs:
        - Hack Assembly - Chapter 6 (CPU Emulator)
        - Virtual Machine Instructions - Chapters 7-8 (VM Emulator, CPU Emulator)
        - Jack Compiler - Chapters 9-11 (Jack Compiler, VM Emulator)
        - Operating System Library - Chapter 12 (Jack Compiler) <- Current todo!

To run the Jack compiler, run `jackmain.cpp` in the software folder and enter the filename (or folder name) of the Jack file/s to compile into VM instructions.

To run the VM translator, run `main.cpp` in the software folder and enter the filename (or folder name) of the VM file/s to translate into binary opcodes corresponding to Hack assembly commands.

### Todo List
___
- Complete Chapter 12 - Operating System Classes
- Review code and refactor as necessary (Feel free to suggest ~~and judge~~!)
- Create an executable program for the Jack compiler using CMake
- Test hardware implementation on an FPGA board!