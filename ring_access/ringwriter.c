/*  Generic ring writer function for Earthworm 
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
    
#include <earthworm.h>
#include <transport.h>

#include "ringwriter.h"

int write_ring(char * raw_data, char ** params, int max_length)
{  
    SHM_INFO region1;           /* main shared memory region         */
    long ringkey;               /* key to transport ring             */
    MSG_LOGO logo;              /* array of module,type,instid       */
    static char * msg;          /* message from file                 */
    int nread;                  /* number of bytes read from file    */
    long sequence;              /* opt sequence# read from arguments */
    unsigned char seq = 0;      /* sequence# to use in tport_copyto  */
    char * ring_name;
    char * data_type;
    char * module_id;
    char * sequence_array;

    ring_name = params[0];
    data_type = params[1];
    module_id = params[2];
    sequence_array = params[3];

    msg = (char *) malloc(max_length);

    int i;
    for (i = 0; i < max_length; i++)
        msg[i] = raw_data[i];

/* Look up info from earthworm.h tables
   ************************************/
    if((ringkey = GetKey(ring_name)) == -1) {
        printf("Invalid ring name: %s; exiting!\n", ring_name);
        return(-1);
    }
    if (GetLocalInst(&logo.instid) != 0) {
        printf("Invalid local installation; exiting!\n");
        return(-1);
    }
    if (GetModId(module_id, &logo.mod) != 0) {
        printf("Invalid module name: %s; exiting!\n", module_id);
        return(-1);
    }

    if (GetType(data_type, &logo.type) != 0) {
        printf("Invalid message type: %s; exiting!\n", data_type);
        return(-1);
    }

/* Read sequence argument if it exists
 *************************************/ 

    sequence = atol(sequence_array);
    if(sequence < 0) {
        printf("Invalid sequence# <%ld>, must be positive; exiting!\n", sequence);
        return(-1);
    }

    seq = sequence % 256;


/* Send archive message to transport ring
 ****************************************/
    tport_attach(&region1, ringkey);

    if(tport_copyto(&region1, &logo, max_length, msg, seq) != PUT_OK) {
        printf("Error writing message: %s to transport ring.\n", msg);
        return (-1);
    }

    tport_detach(&region1);

    return(0);
}


