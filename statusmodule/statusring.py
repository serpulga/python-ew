"""
Status python interface for Earthworm 
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
import re

import statusmodule

# ring.py must on parent folder,
# Adding parent folder to the path.
cwd = os.getcwd()
ringdir = cwd[:cwd.rfind('/')]
sys.path.append(ringdir)

from ring import Ring

class StatusRing(Ring):
    """
    Class for Reading Status messages from a ring.
    """
    def __init__(self, ring_name, module_id):
        Ring.__init__(self, ring_name, module_id, StatusMessage)

    def module_write(self, **args):
        raise TypeError('Cannot write status type messages.')

    def module_read(self, **params):
        """
        Implementing base class method, simply
        calling the 'ring_read' function from
        the statusmodule module.
        """
        params['sequence'] = str(self.sequence)
        raw_status = statusmodule.ring_read(**params)
        self.sequence += 1

        return [StatusRing.processStatus(raw_status)]
    
    @staticmethod
    def processStatus(raw_status):
        status_dict = {}
        status_dict['raw_status'] = raw_status
        status_dict['modules'] = []
        status_lines = raw_status.split('\n')

        found_modules = False
        
        for line in status_lines:
                
            #Removes whitespace at the begining of the line.        
            whitespace = re.compile('^\s+')
            line = re.sub(whitespace, "", line)
            
            if found_modules:
                module_status = re.compile('^([\S]+)(\s+)(\d+)(\s+)(\w+)')
                
                match = re.match(module_status, line)
                        
                # Group 1: Stuff (module name).
                # Group 2: whitespace.
                # Group 3: Decimal numbers (Process id).
                # Group 4: whitespace.
                # Group 5: Alphanumeric (module status)
                
                try:
                    module_name = match.group(1)
                    module_id = match.group(3)
                    status = match.group(5)

                    module = {}
                    module['module_name'] = module_name
                    module['module_id'] = module_id
                    module['status'] = status

                    status_dict['modules'].append(module)

                except AttributeError:
                    continue
                    
            elif line.startswith('startstop'):
                found_modules = True
                continue

        return status_dict

class StatusMessage():
    def __init__(self, status_dict):
        self.status_dict = status_dict
       
    @staticmethod
    def fromDict(status_dict):
        return StatusMessage(status_dict)

    @staticmethod
    def toDict(status_message):
        return status_message.status_dict
