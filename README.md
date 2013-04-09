PythonEw
=========

By OSOP 

http://www.osop.com.pa

![OSOP LOGO](http://www.osop.com.pa/wp-content/uploads/2012/02/ss.png)

Python wrapper for accessing an Earthworm shared memory ring.
Earthworm is a set of opensource programs, tools and libraries that are used in the development of software for maintaining seismic networks, research and other seismological and geophysical applications. 
http://vps.isti.com/trac/ew/

The main objective of these wrapper-modules is to make it easier for people to contribute to the Earthworm project; developing Earthworm modules using these wrappers is truly a joy as it enables developers to have directy access to memory rings whilst enjoying all the functionality that *Python* itself brings to the table. Introducing *Python* into the Earthworm picture will allow new people to come in and contribute.

Although these modules are far from complete, they are currently quite functional and usable; the ```Tracebuf2``` module specially, is already being used in production scenarios, and it has proven to be a very usefull tool.

**MODULE OVERVIEW**

A module consists of one *C* and two *Python* sources. The *C* source are basically Python-wrapped functions that allow Read/Write operations of a certain Earthworm datatype. One of the *Python* source contains interfaces to the *C* wrapped functions; the other *Python* source is a basic setup tool.

Example:
```
Tracebuf2
    tracebuf2module/
        tracebuf2module.c
        tracebuf2ring.py
        trace_setup.py
```
In order to use a module from *Python*, the *C* source, in this case ```tracebuf2module.c```, needs to be compiled into a dynamically linked library that *Python* can load.

**HOW TO INSTALL**

In order to follow theses steps, Earthworm needs to be previously
installed in your computer. Also, note that these are steps for GNU/Linux - UNIX only.

1. Use the makefile included in ```ring_access/Makefile``` to create ```ringwriter.o``` and ```ringreader.o```. Once created, copy them into the Earthworm's ```/bin/``` directory. Note that you need to edit the ```INCLUDE``` and ```LIBRARIES``` variables in the makefile to match your Earthworm installation. These are libraries and binaries needed by all the modules.

2. Copy ```ringwriter.h``` and ```ringreader.h``` into the Earthworm's ```/include/``` directory.

3. Choose the module you want to compile and simply run the corresponding python setup script.
Example: 

    ```python trace_setup.py build```

This will create a dynamically linked library corresponding to the module, and it's the one
that the python module will try to load.
