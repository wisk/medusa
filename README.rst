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

First, we need to retrieve and compile the library OGDF (Windows users must execute cmake command from *Visual Studio (...) Command Prompt*):

.. code-block:: bash

  git clone https://github.com/ogdf/ogdf
  cd ogdf

  mkdir _release
  cd _release
  cmake -DCMAKE_BUILD_TYPE=Release ..
  make  # if you use Makefile (usually UNIX users)
  nmake # if you use NMakefile (Windows users)

  cd ..

  mkdir _debug
  cd _debug
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  make or nmake


Next step is the Boost installation, you can either download a built version or compile yourself. Boost is available `here <http://www.boost.org/users/history/version_1_55_0.html>`_

Now, make sure you have installed Qt5 if you need a graphical user interface (and I'm pretty you do ;)). Pick the right version on the `official website <http://qt-project.org/downloads>`_ or use your package manager.

Finally, we're ready to retrieve and compile medusa:

.. code-block:: bash

  git clone https://github.com/wisk/medusa
  mkdir build
  cd build
  cmake -DOGDF_PATH:PATH=<Path to the OGDF directory> -DBOOST_ROOT:PATH=<path to the boost directory> -DQT5_CMAKE_PATH:PATH=<Path to Qt5 cmake scripts directory> ..

Note: If CMake is unable to find Boost, try to define **BOOST_LIBRARYDIR** instead. This variable must be set to the library directory (e.g.: *C:\\boost_1_55_0\\lib64-msvc-11.0*).
In my configuration, **QT5_CMAKE_PATH** is set to */usr/lib/cmake* on ArchLinux and *C:\\Qt\\Qt5.0.2\\5.0.2\\msvc2012_64\\lib\\cmake* on Windows.
For Windows users, you should probably add **-G"Visual Studio XX <Win64>"** where *XX* is your Visual Studio version and *<Win64>* if you build medusa in 64-bit.

Screenshots
===========

Main interface
--------------

.. image:: https://raw.github.com/wisk/medusa/master/img/shots/main_interface.png

Control flow graph
------------------

.. image:: https://raw.github.com/wisk/medusa/master/img/shots/cfg.png

Interactivity
-------------

.. image:: https://raw.github.com/wisk/medusa/master/img/shots/interactivity.png


Contacts
========

 * IRC: irc://freenode.net/#medusa_disasm
 * Twitter: https://twitter.com/@medusa_disasm

Acknowledgements
================

My schoolmates: epieddy, François and FX.
Yusuke Kamiyamane for his `icons <http://p.yusukekamiyamane.com>`_
