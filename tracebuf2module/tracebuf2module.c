/*  Python wrapper for reading and writing Earthwrom Tracebuf2 data into a ring
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
#include <trace_buf.h>

#include <ringwriter.h>
#include <ringreader.h>

typedef struct{
	TRACE2_HEADER header;
	char samples[MAX_TRACEBUF_SIZ - sizeof(TRACE2_HEADER)];
} TRACE2_HEADER_AND_SAMPS;

static PyObject * ring_write(PyObject * self, PyObject * args, PyObject *kws)
{
	int pinno, nsamp;
	PyObject * sample_list;
	double starttime, endtime, samprate;
	char  * sta, * net, * chan, * loc;
    char * version, * datatype, * quality, * pad, * ring, * module, * sequence;
	char  *keywords[] = {"pinno", "nsamp", "starttime", "endtime", "samprate", "sta", "net", "chan", 
						"loc", "version", "datatype", "quality", "pad", "samples", "ring", "module", "sequence", NULL};
	int parsed;

	parsed = PyArg_ParseTupleAndKeywords(args, kws, "iidddssssssssOsss", keywords, &pinno, &nsamp, &starttime,
										&endtime, &samprate, &sta, &net, &chan, &loc, &version,
										&datatype, &quality, &pad, &sample_list, &ring, &module, &sequence);	

	if (!parsed) {
		printf("Tracebuf2: Wrong args! \n");
		return NULL;		
	}
	
	else {
	    int tracesize = sizeof(TRACE2_HEADER) + (sizeof(int) * nsamp); 
	    if (tracesize > MAX_TRACEBUF_SIZ) {
	        return Py_BuildValue("s", "Traceubuf2: Message too long");
	    }
		
		int i;
		int samples_int[nsamp];
		char * samples;
		for (i = 0; i < nsamp; i++){
			int sample;

			PyArg_Parse(PyList_GetItem(sample_list, i), "i", &sample);
			samples_int[i] = sample;
		}

		samples = (char *) samples_int;	
		
	    char * raw_data;
		char *params[] = {ring, "TYPE_TRACEBUF2", module, sequence};
		TRACE2_HEADER trace_header;
		TRACE2_HEADER_AND_SAMPS trace_data;

		trace_header.pinno = pinno;
		trace_header.nsamp = nsamp;
		trace_header.starttime = starttime;
		trace_header.endtime = endtime;
		trace_header.samprate = samprate;

		strcpy(trace_header.sta, sta);
		strcpy(trace_header.net, net);
		strcpy(trace_header.chan, chan);
		strcpy(trace_header.loc, loc);
		strncpy(trace_header.version, version, 1);
		strcpy(trace_header.datatype, datatype);
		strncpy(trace_header.quality, quality, 1);
		strncpy(trace_header.pad, pad, 1);

		trace_data.header = trace_header;
		for (i = 0; i < sizeof(int) * nsamp; i++)
			trace_data.samples[i] = samples[i];	

		raw_data = (char *) &trace_data;
		write_ring(raw_data, params, tracesize);
	}

	return Py_BuildValue("s", "Ok!");
}

static PyObject * ring_read(PyObject * self, PyObject * args, PyObject *kws)
{	
	char * ring, * module;
	char *keywords[] = {"ring", "module", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kws, "ss", keywords, &ring, &module)) {
		printf("Tracebuf2: Wrong args! \n");
		return NULL;		
	}

	unsigned char items = 0;
	char *params[] = {ring, "TYPE_TRACEBUF2", module};
	char *raw_data[1000];
	PyObject * trace_list;

	items = read_ring(params, MAX_TRACEBUF_SIZ, raw_data);
	trace_list = PyList_New(items);
	
	int i;
	int k;
	for (i = 0; i < items; i++) {
        PyObject * sample_list;
		PyObject * trace;

		TRACE2_HEADER * trace_data;
        trace_data = (TRACE2_HEADER *) raw_data[i];

	    trace = Py_BuildValue("{s:i,s:i,s:d,s:d,s:d,s:s,s:s,s:s,s:s,s:s,s:s,s:s,s:s}",
					   	"pinno", trace_data->pinno, "nsamp", trace_data->nsamp, "starttime", trace_data->starttime,
					   	"endtime", trace_data->endtime, "samprate", trace_data->samprate, "sta", trace_data->sta, 
						"net", trace_data->net, "chan", trace_data->chan, "loc", trace_data->loc,
						"version", trace_data->version, "datatype", trace_data->datatype, "quality", trace_data->quality, 
						"pad", trace_data->pad);

		int * long_data = (int *)(raw_data[i] + sizeof(TRACE2_HEADER));
    	short * short_data = (short *)(raw_data[i] + sizeof(TRACE2_HEADER));

		sample_list = PyList_New(trace_data->nsamp);

		if ((strcmp(trace_data->datatype, "s2") == 0) || (strcmp(trace_data->datatype, "i2") == 0)) {
        	for (k = 0; k < trace_data->nsamp; k++ )
            	PyList_SetItem(sample_list, k, Py_BuildValue("i", *(short_data + k)));
		}

        else if ((strcmp (trace_data->datatype, "s4") == 0) || (strcmp(trace_data->datatype, "i4") == 0)) {
        	for (k = 0; k < trace_data->nsamp; k++)
            	PyList_SetItem(sample_list, k, Py_BuildValue("i", *(long_data + k)));
		}
		
		PyDict_SetItem(trace, Py_BuildValue("s", "samples"), sample_list);
		PyList_SetItem(trace_list, i, trace);
	}

	return trace_list;
}

static PyMethodDef TracebufMethods[] = {
	{"ring_write", (void*)ring_write, METH_VARARGS | METH_KEYWORDS, "Writes a TYPE_TRACEBUF2 message into the a ring."},
	{"ring_read", (void*)ring_read, METH_VARARGS | METH_KEYWORDS, "Reads TYPE_TRACEBUF2 messages from the ring."},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
inittracebuf2module(void)
{
    (void) Py_InitModule("tracebuf2module", TracebufMethods);
}
