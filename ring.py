    """
    Generic python ring interface for Earthworm 
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


class Ring():
    """
    Ring class defines a generic interface tha will be extended 
    to use with each specific type.
    """
    def __init__(self, ring_name, module_id, data_type):
        self.module_id = module_id
        self.ring = ring_name
        self.sequence = 0
        self.data_type = data_type

    def module_write(self):
        raise NotImplementedError("Method not implemented in base class.")

    def module_read(self):
        raise NotImplementedError("Method not implemented in base class.")

    def write(self, data):
        """
        Write interface method. Creates the needed parameters
        dict and writes the data into the ring.
        """
        data_dict = self.data_type.toDict(data)
        args = self.completeWriteDict(data_dict)

        try:
            self.module_write(**args)
            self.sequence += 1
            return 0, args

        except Exception, err:
            print 'Caught exception %s' % str(err)
            print "Unexpected error:", sys.exc_info()[0]
            return 1, args

    def read(self):
        """
        Read interface method. Creates the needed parameters
        dict and reads from the ring.
        """
        try:
            params = self.createReadDict()
            data = self.module_read(**params)

            data_list = []
    
            for item in data:
                data_list.append(self.data_type.fromDict(item))
        
            return data_list, params

        except Exception, err:
            print 'Caught exception %s' % str(err)
            print "Unexpected error:", sys.exc_info()[0]
            return [], params

    def completeWriteDict(self, data_dict):
        """
        Completes 'data_dict' including the parameters 
        needed for a write operation.
        """
        data_dict['ring'] = self.ring
        data_dict['module'] = self.module_id
        data_dict['sequence'] = str(self.sequence)

        return data_dict

    def createReadDict(self):
        """
        Creates a dict including the parameters 
        needed for a write operation.
        """
        read_dict = {}

        read_dict['ring'] = self.ring
        read_dict['module'] = self.module_id

        return read_dict

