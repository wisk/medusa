Medusa
======

Medusa is a disassembler designed to be both modular and interactive.
It runs on Windows and Linux, it should be the same on OSX.
This project is organized as a library. To disassemble a file you have to use
medusa_dummy or qMedusa.
wxMedusa and pydusa are not available anymore.

Prerequisites
=============

Medusa requires the following libraries: `boost <http://www.boost.org>`_ (system, filesystem, thread, date_time), `OGDF <http://www.ogdf.net>`_, and `Qt5 <http://qt-project.org>`_ for the GUI.
You also need `CMake <http://www.cmake.org>`_ for compilation.

Feature
=======

+-------------------------------------------------------------------------------------------+
| Loader                                                                                    |
+---------+---------+--------+--------+---------+-------------------------------------------+
| Name    | Mapping | Import | Export | Symbols | Notes                                     |
+=========+=========+========+========+=========+===========================================+
| ELF     | yes     | yes    | no     | no      | Some kind of reloc are not handled        |
+---------+---------+--------+--------+---------+-------------------------------------------+
| PE      | yes     | yes    | no     | no      |                                           |
+---------+---------+--------+--------+---------+-------------------------------------------+
| GameBoy | yes     | yes    | yes    | yes     | - GameBoy Color registers are not handled |
|         |         |        |        |         | - All mappers are not handled             |
+---------+---------+--------+--------+---------+-------------------------------------------+

+----------------------------------------------------------------+
| Architecture                                                   |
+------+---------+-------------+----------+----------------------+
| Name | Mode    | Disassembly | Semantic | Notes                |
+======+=========+=============+==========+======================+
| x86  | 16-bit  | yes         | yes      |                      |
+------+---------+-------------+----------+----------------------+
| x86  | 32-bit  | yes         | partial  | Support until SEE4.2 |
+------+---------+-------------+----------+----------------------+
| x86  | 64-bit  | yes         | partial  |                      |
+------+---------+-------------+----------+----------------------+
| z80  | gameboy | yes         | yes      |                      |
+------+---------+-------------+----------+----------------------+
| avr8 |         | yes         | no       |                      |
+------+---------+-------------+----------+----------------------+
| arm  | normal  | partial     | no       | Experimental         |
+------+---------+-------------+----------+                      |
| arm  | thumb   | partial     | no       |                      |
+------+---------+-------------+----------+                      |
| arm  | thumb2  | partial     | no       |                      |
+------+---------+-------------+----------+----------------------+

+--------------------------------------------------------------------------+
| User interface                                                           |
+--------+---------------+---------+-------+---------+----------+----------+
| Name   | interactivity | Comment | Label | Graph   | Database | Notes    |
+========+===============+=========+=======+=========+==========+==========+
| qt     | partial       | yes     | yes   | yes     | partial  |          |
+--------+---------------+---------+-------+---------+----------+----------+
| dummy  | no            | no      | no    | no      | no       | outdated |
+--------+---------------+---------+-------+---------+----------+----------+
| python | no            | no      | no    | no      | no       | broken   |
+--------+---------------+---------+-------+---------+----------+----------+

+-----------------------+
| Database              |
+------+---------+------+
| Name | Save    | Load |
+======+=========+======+
| text | partial | no   |
+------+---------+------+

+-------------------------------+
| Operating System              |
+---------+---------------------+
| Name    | Notes               |
+=========+=====================+
| UNIX    | Not yet implemented |
+---------+                     |
| Windows |                     |
+---------+---------------------+


Roadmap
=======

Each versions of Medusa bring a new specific feature.

* 0.1 the beginning of the project
* 0.2 new qt interface
* 0.3 flow graph
* *0.4 load/save database*
* 0.5 semantic support
* 0.6 better python binding
* 0.7 debugger

Compilation
===========

UNIX
----

TODO

Windows
-------

TODO

Screenshots
===========

TODO

Acknowledgements
================

My schoolmates: Edouard, François and François-Xavier.
