Medusa
======

Medusa is a disassembler designed to be both modular and interactive.
It runs on Windows and Linux, it should be the same on OSX.
This project is organized as a library. To disassemble a file you have to use
medusa_text or qMedusa.
An experimental emulator is also available.

Build-bot
=========


+--------+-------------------------------------------------------------+-----------------------------------------------------------------------------------------------+
| branch | Travis                                                      | AppVeyor                                                                                      |
+========+=============================================================+===============================================================================================+
| dev    | |unix_build|                                                | |windows_build|                                                                               |
+--------+-------------------------------------------------------------+-----------------------------------------------------------------------------------------------+

Prerequisites
=============

Medusa requires the following libraries: `boost >= 1.55 <http://www.boost.org>`_ (system, filesystem, thread, date_time), `OGDF <http://www.ogdf.net>`_ (required git), and `Qt5 >= 5.2 <http://qt-project.org>`_ for the GUI.
You also need `CMake <http://www.cmake.org>`_ for compilation and a C++11 compiler (VS2013 update 4 on Windows).
Git is optional but allows to clone remote repository for specific features, see *Compilation/Options*.

Feature
=======

+-------------------------------------------------------------------------------------------+
| Loader                                                                                    |
+---------+---------+--------+--------+---------+-------------------------------------------+
| Name    | Mapping | Import | Export | Symbols | Notes                                     |
+=========+=========+========+========+=========+===========================================+
| ELF     | yes     | yes    | no     | no      | Some kind of reloc are not handled        |
+---------+---------+--------+--------+---------+-------------------------------------------+
| PE      | yes     | yes    | yes    | no      | Reloc are not handled                     |
+---------+---------+--------+--------+---------+-------------------------------------------+
| Mach-O  | yes     | yes    | no     | no      | - Doesn't support FAT binary              |
|         |         |        |        |         | - on X86, esi as glbptr is not handled    |
+---------+---------+--------+--------+---------+-------------------------------------------+
| GameBoy | yes     | yes    | yes    | yes     | - GameBoy Color registers are not handled |
|         |         |        |        |         | - All mappers are not handled             |
+---------+---------+--------+--------+---------+-------------------------------------------+

+----------------------------------------------------------------+
| Architecture                                                   |
+------+---------+-------------+----------+----------------------+
| Name | Mode    | Disassembly | Semantic | Notes                |
+======+=========+=============+==========+======================+
| x86  | 16-bit  | yes         | partial  |                      |
+------+---------+-------------+          |                      |
| x86  | 32-bit  | yes         |          | Support until SEE4.2 |
+------+---------+-------------+          |                      |
| x86  | 64-bit  | yes         |          |                      |
+------+---------+-------------+----------+----------------------+
| z80  | gameboy | yes         | yes      |                      |
+------+---------+-------------+----------+----------------------+
| avr8 |         | no          | no       | Broken               |
+------+---------+-------------+----------+----------------------+
| arm  | normal  | partial     | partial  |                      |
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
| qt     | partial       | yes     | yes   | yes     | yes      |          |
+--------+---------------+---------+-------+---------+----------+----------+
| text   | no            | no      | no    | no      | yes      |          |
+--------+---------------+---------+-------+---------+----------+----------+

+----------------+
| Binding        |
+--------+-------+
| Name   | Notes |
+========+=======+
| Python | WIP   |
+--------+-------+

+------------------------------------+
| Database                           |
+------+---------+------+------------+
| Name | Save    | Load | Notes      |
+======+=========+======+============+
| text | yes     | yes  | Incomplete |
+------+---------+------+------------+

+------------------+
| Operating System |
+---------+--------+
| Name    | Notes  |
+=========+========+
| UNIX    | WIP    |
+---------+        |
| Windows |        |
+---------+--------+


Roadmap
=======

Each versions of Medusa bring a new specific feature.

* 0.1 the beginning of the project
* 0.2 new qt interface
* 0.3 flow graph
* 0.4 load/save database
* *0.5 semantic support*
* 0.6 better python binding
* 0.7 debugger

Compilation
===========

First off, you need boost libraries; you can either download a built version or compile yourself. Boost is available `here <http://www.boost.org/users/history/version_1_55_0.html>`_

Now, make sure you have installed Qt5 if you need a graphical user interface (and I'm pretty sure you do ;)). Medusa requires at least the version 5.2, be sure to pick the good version on the `official website <http://qt-project.org/downloads>`_ or use your package manager.

Finally, we're ready to retrieve and compile medusa:

.. code-block:: bash

  git clone https://github.com/wisk/medusa.git
  mkdir build
  cd build
  # UNIX users should define CMAKE_BUILD_TYPE e.g. -DCMAKE_BUILD_TYPE=Release to compile Medusa with optimizatin
  cmake -DBOOST_ROOT:PATH=<path to the boost directory> -DQT5_CMAKE_PATH:PATH=<Path to Qt5 cmake scripts directory> ..

  # for UNIX users
  make && cd bin && ./qMedusa

  # for Windows users
  explorer Medusa.sln


Note: If CMake is unable to find Boost on Windows, try to define **BOOST_LIBRARYDIR**. This variable must be set to the library directory (e.g.: *C:\\boost_1_55_0\\lib64-msvc-11.0*).
In my configuration, **QT5_CMAKE_PATH** is set to */usr/lib/cmake* on ArchLinux and *%USERPROFILE%\\Sources\\qt-5.3.0-x64-msvc2012-compact\\lib\\cmake* on Windows.
For Windows users, you should probably add **-G"Visual Studio XX Win64"** where *XX* is your Visual Studio version and *Win64* if you build medusa in 64-bit.
To run the Qt interface on Windows, you may have to add the folder *%QTDIR%\\bin* to your *%PATH%* and copy the folder *%QTDIR%\\plugins\\platforms*.
By default, Medusa searches modules in the current folder, so you should run medusa executables from the folder where modules are located (e.g. *build/bin* on UNIX or *build\\bin\\{Debug,Release,...}* on Windows).

Options
-------

* MEDUSA_BUILD_WITH_OGDF: clone OGDF project and build it, this library allows qMedusa to display graph


Screenshots
===========

Main interface
--------------

.. image:: https://raw.github.com/wisk/medusa/dev/img/shots/main_interface.png

Control flow graph
------------------

.. image:: https://raw.github.com/wisk/medusa/dev/img/shots/cfg.png

Interactivity
-------------

.. image:: https://raw.github.com/wisk/medusa/dev/img/shots/interactivity.png

.. image:: https://raw.github.com/wisk/medusa/dev/img/shots/label.png

Binding
-------

.. image:: https://raw.github.com/wisk/medusa/dev/img/shots/python_binding.png


Contacts
========

 * IRC: irc://freenode.net/#medusa_disasm
 * Twitter: https://twitter.com/@medusa_disasm

Acknowledgements
================

* My schoolmates: epieddy, flalande and FX.
* My workmate: gg, w1gz.
* Yusuke Kamiyamane for his `icons <http://p.yusukekamiyamane.com>`_
* gunmetal313, saeschdivara, kangjoni76, KarlVogel for their contributions.

.. |unix_build| image:: https://img.shields.io/travis/wisk/medusa/dev.svg?style=flat-square&label=unix%20build
    :target: http://travis-ci.org/wisk/medusa
    :alt: Build status of the dev branch on Mac/Linux

.. |windows_build|  image:: https://img.shields.io/appveyor/ci/wisk/medusa.svg?style=flat-square&label=windows%20build
    :target: https://ci.appveyor.com/project/wisk/medusa
    :alt: Build status of the dev branch on Windows
