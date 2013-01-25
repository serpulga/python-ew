/*  Generic ring reader function for Earthworm 
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
    
#include <transport.h>
#include <swap.h>
#include <time_ew.h>
#include <earthworm.h>
#include <string.h>

#include "ringreader.h"

int read_ring(char ** params, int max_lenth, char ** raw_data)
{
	long RingKey;
	SHM_INFO region;
	unsigned char type, ModWildcard, InstWildcard;
	MSG_LOGO getlogo[1], logo;
	char msg[max_lenth];
	long gotsize;
	static unsigned char instId;
	unsigned char sequence_number = 0;
	char * ring_name;
	char * data_type;
	char * module_id;

	ring_name = params[0];
	data_type = params[1];
	module_id = params[2];

	RingKey = GetKey(ring_name);

	if (RingKey == -1) 
 	{
      fprintf(stderr, "Error getting ring; exiting!\n" );
      return -1;
	}

	tport_attach(&region, RingKey);

	if (GetLocalInst(&instId) != 0)
    {
      fprintf(stderr, "Error getting local installation id; exiting!\n" );
      return -1;
    }

    if (GetType(data_type, &type) != 0)
	{
      fprintf(stderr, "Error getting message type; exiting!\n" );
      return -1;
	}
	
	if (GetModId(module_id, &ModWildcard) != 0)
    {
      fprintf(stderr, "Invalid mod id; exiting!\n" );
      return -1;		
    }

	if (GetInst("INST_WILDCARD", &InstWildcard) != 0)
	{
      fprintf(stderr, "Invalid installation; exiting!\n" );
      return -1;
	}
    
    getlogo[0].type = type;
    getlogo[0].instid = InstWildcard;
    getlogo[0].mod = ModWildcard;  

	while(tport_getmsg(&region, getlogo, 1, &logo, &gotsize, (char *) &msg, max_lenth) != GET_NONE) {}

	int rc = -1;
	int i = 0;
	int k;
	int j;
	for(k = 0; k < 1000; k++) {
        if (tport_getflag(&region) == TERMINATE) {
            break;
        }
    
		rc = tport_copyfrom(&region, getlogo, 1, &logo, &gotsize, (char *) &msg, max_lenth, &sequence_number);

		if (rc == GET_NONE)
		{
			continue;
		}

		if (logo.type == type) {
			raw_data[i] = (char *) malloc(max_lenth * sizeof(char));
	
			for (j = 0; j < max_lenth; j++) {			
				raw_data[i][j] = msg[j];
			}

			fflush (stdout);
			i++;
		}
    }
    
    tport_detach(&region);
    
	return i;
}

