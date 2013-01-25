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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Generic ring writer function for Earthworm
// raw_data: char * casted data of what will be put in the ring.
// params: Array of needed parameters:
//     params[0] -> name of the ring
//     params[1] -> earthworm datatype of the data
//     params[2] -> module id
//     params[3] -> sequence of the data packet, ie: "34"
// max_leng: Maximum length of the datatype being written
int write_ring( char * raw_data, char ** params, int max_length);
