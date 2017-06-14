/**
 * Copyright 2017 University of Applied Sciences Western Switzerland / Fribourg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This module is based on the software library developped by Texas Instruments
 * Incorporated - http://www.ti.com/ for its AM335x starter kit.
 *
 * Project:		HEIA-FR / Embedded Systems 1 Laboratory
 * 						Personal Operating System
 *
 * Abstract:	Implementation of a basic cooperative Operating System
 *
 * Purpose:		This module implement strings operations.
 *
 * Author:		Daniel Gachet (first author)
 * 				Alan Sueur
 * 				Nicolas Fuchs
 *
 * Date: 		27.04.2017
 * Last update:	28.04.2017
 *
 */

#ifndef UTILS_STRINGS_H_
#define UTILS_STRINGS_H_

#define EOS		'\0'

typedef struct _strings_t{
		int size;
		char** strings;
}strings_t;

extern void strings_trim(char* source);
extern void strings_copy(char* destination, const char* source, int source_start, int source_end);
extern strings_t strings_split(const char* text, char sep);
extern int strings_nb_token(const char* text, char sep);
extern void strings_free_strings_t(strings_t* s);

#endif /* UTILS_STRINGS_H_ */
