======
Medusa
======

Medusa is a disassembler designed to be both modular and interactive.
It runs on Windows and Linux, it should be the same on OSX.
This project is organized as a library. To disassemble a file you have to use
medusa_dummy or qMedusa.
wxMedusa is not available anymore.

Prerequisites
=============

Medusa needs boost with the following components: system, filesystem, thread, date_time.
Qt4 is an optional dependency for qMedusa, ditto with boost-python for pydusa.
This project uses cmake to compile.

For windows users, you must compile Qt4 with /Zc:wchar_t.

Feature
=======

Since Medusa uses modules to handle different kind of executable.
Loader modules allow to interpret executable format and Architecture modules are
able to disassemble instructions.

Loader

* elf
* pe
* gameboy ROM

Architecture

* x86 (16 / 32 / 64)
* gameboy zilog 80
* avr8

Roadmap
=======

Each versions of Medusa bring a new specific feature.

Version

* 0.1 the beginning of the project
* 0.2 new qt interface
* 0.3 flow graph
* 0.4 load/save database
* 0.5 better python binding
* 0.6 debugger
* ...

Build
=====

In the project directory, make a new folder named build. Then use cmake to generate
adapted make file.
Compiled files will be in 'bin' folder.

Thanks
======

My schoolmates: Edouard, François and François-Xavier.
