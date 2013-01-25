/*  Python wrapper for heartbeating into an Earthworm ring.
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

#include <ringwriter.h>
#include <ringreader.h>

static PyObject * ring_write(PyObject * self, PyObject * args, PyObject *kws)
{
    char * unix_time, * ring, * module, * sequence;
	char *keywords[] = {"unix_time", "ring", "module", "sequence", NULL};
	int parsed;

	parsed = PyArg_ParseTupleAndKeywords(args, kws, "ssss", keywords, &unix_time, &ring, &module, &sequence);
	if (!parsed) {
		printf("HeartBeat: Wrong parameters! \n");
		return NULL;		
	}

	else {
	    char * raw_data;
		char *params[] = {ring, "TYPE_HEARTBEAT", module, sequence};

		write_ring(unix_time, params, strlen(unix_time));
	}

	return Py_BuildValue("s", "Ok!");
}

static PyMethodDef HeartBeatMethods[] = {
	{"ring_write", (void*)ring_write, METH_VARARGS | METH_KEYWORDS, "Writes a TYPE_HEARTBEAT message into the a ring."},
	{NULL, NULL, (void*)0, NULL}
};

PyMODINIT_FUNC
initheartbeatmodule(void)
{
    (void) Py_InitModule("heartbeatmodule", HeartBeatMethods);
}
