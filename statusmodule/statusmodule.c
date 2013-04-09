/*  Python wrapper for getting the current Earthworm status.
    Copyright (C) 2013, OSOP SA Panama

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */
    
#include <stdio.h>
#include <stdlib.h>

#include <Python.h>
#include <transport.h>
#include <earthworm.h>

#include <ringwriter.h>
#include <ringreader.h>

#define MAX_BYTES_STATUS MAX_BYTES_PER_EQ

static PyObject * ring_read(PyObject * self, PyObject * args, PyObject *kws)
{	
    char * ring, * module, *sequence;
    char *keywords[] = {"ring", "module", "sequence", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kws, "sss", keywords, &ring, &module, &sequence)) {
    printf("Status: Wrong parameters! \n");
    return NULL;		
    }

    unsigned char items = 0;
    char *params[] = {ring, "TYPE_STATUS", module};
    char *raw_data[1000];
    char *write_params[] = {ring, "TYPE_REQSTATUS", module, sequence};

    // Request status before reading.
    write_ring(module, write_params, strlen(module));
    
    sleep(10);

    items = read_ring(params, MAX_BYTES_STATUS, raw_data);

    if (items > 0)
    return Py_BuildValue("s", raw_data[0]);

    else 
    return NULL;
}

static PyMethodDef StatusMethods[] = {
    {"ring_read", (void*)ring_read, METH_VARARGS | METH_KEYWORDS, "Reads TYPE_STATUS messages from the ring."},
    {NULL, NULL, (void*)0, NULL}
};

PyMODINIT_FUNC
initstatusmodule(void)
{
    (void) Py_InitModule("statusmodule", StatusMethods);
}
