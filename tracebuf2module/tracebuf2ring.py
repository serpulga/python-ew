"""
Tracebuf2 python interface for Earthworm 
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
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import os
import sys

import tracebuf2module


# ring.py must on parent folder,
# Adding parent folder to the path.
cwd = os.getcwd()
ringdir = cwd[:cwd.rfind('/')]
sys.path.append(ringdir)

from ring import Ring

class Tracebuf2Ring(Ring):
    """
    Class for Reading/Writing Tracebuf2 messages from/into a ring.
    """
    def __init__(self, ring_name, module_id):
        Ring.__init__(self, ring_name, module_id, Tracebuf2Message)

    def module_write(self, **args):
        """
        Implementing base class method, simply
        calling the 'ring_write' function from
        the tracebuf2module module.
        """
        return tracebuf2module.ring_write(**args)

    def module_read(self, **params):
        """
        Implementing base class method, simply
        calling the 'ring_read' function from
        the tracebuf2module module.
        """
        return tracebuf2module.ring_read(**params)

class Tracebuf2Message():
    def __init__(self, pinno, nsamp, starttime, endtime,
                samprate, sta, net, chan, loc,
                version, datatype, quality, pad, samples):
    
        self.pinno = pinno
        self.nsamp = nsamp
        self.starttime = starttime
        self.endtime = endtime
        self.samprate = samprate
        self.sta = sta
        self.net = net
        self.chan = chan
        self.loc = loc
        self.version = version
        self.datatype = datatype
        self.quality = quality
        self.pad = pad
        self.samples = samples
    
    def __str__(self):
        return str(Tracebuf2Message.toDict(self))
    
    __repr__ = __str__        

    @staticmethod    
    def toDict(trace_message):
        trace_dict = {}
    
        trace_dict['pinno'] = trace_message.pinno
        trace_dict['nsamp'] = trace_message.nsamp
        trace_dict['starttime'] = trace_message.starttime
        trace_dict['endtime'] = trace_message.endtime
        trace_dict['samprate'] = trace_message.samprate
        trace_dict['sta'] = trace_message.sta
        trace_dict['net'] = trace_message.net
        trace_dict['chan'] = trace_message.chan
        trace_dict['loc'] = trace_message.loc
        trace_dict['version'] = trace_message.version
        trace_dict['datatype'] = trace_message.datatype
        trace_dict['quality'] = trace_message.quality
        trace_dict['pad'] = trace_message.pad
        trace_dict['samples'] = trace_message.samples

        return trace_dict

    @staticmethod
    def fromDict(trace_dict):
        try:        
            pinno = trace_dict.get('pinno')
            nsamp = trace_dict.get('nsamp')
            starttime = trace_dict.get('starttime')
            endtime = trace_dict.get('endtime')
            samprate = trace_dict.get('samprate')
            sta = trace_dict.get('sta')
            net = trace_dict.get('net')
            chan = trace_dict.get('chan')
            loc = trace_dict.get('loc')
            version = trace_dict.get('version')
            datatype = trace_dict.get('datatype')
            quality = trace_dict.get('quality')
            pad = trace_dict.get('pad')
            samples = trace_dict.get('samples')

            return Tracebuf2Message(pinno, nsamp, starttime, endtime,
                                    samprate, sta, net, chan, loc,
                                    version, datatype, quality, pad, samples) 

        except KeyError:
            print 'Invalid dictionary.'
            return None
