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

#include "strings.h"

#include <string.h>
#include <stdlib.h>

#define SPACE	' '
#define DIR_SEP	'/'
#define DIR_SEP_STRING	"/"

extern void strings_trim(char* source)
{
	if(!source)return;

	int i=0;
	int k=0;

	while(source[k]==SPACE)k++; //space at the beginning

	while(source[k]!=EOS)
	{
		while(source[k]==SPACE && source[k+1]==SPACE)k++;

		source[i++] = source[k++];
	}

	if(source[i-1]==SPACE) source[i-1]=EOS; //space at the end

	while(i<k) source[i++] = EOS;	//fill with EOS at the end
}

extern void strings_copy(char* destination, const char* source, int source_start, int source_end)
{
	for(int i=0 ; i<(source_end-source_start) ; i++)
		destination[i] = source[source_start + i];
}

extern int strings_nb_token(const char* text, char sep)
{
	int i=0;
	int res = 0;
	if(!text)return res;
	if(text[i]==EOS)return res;
	if(text[i]==sep)
	{
		res++;
		i++;
	}

	while(text[i] != EOS)
	{
		res++;
		while(text[i] != sep && text[i] != EOS)i++;

		if(text[i] == sep)i++;
	}

	return res;
}

strings_t strings_split(const char* text, char sep)
{
	int start = 0;
	int end = 0;
	strings_t res;

	res.size = strings_nb_token(text, sep);
	if(res.size)
		res.strings = malloc(sizeof(char*)*res.size);

	for(int i=0 ; i<res.size ; i++)
	{
		while(text[end] != EOS && text[end] != sep) end++;

		if(end>start)
		{
			char* token = calloc(1,sizeof(char)*(end-start+1));
			strings_copy(token, text, start, end);

			res.strings[i] = token;
		}
		else if(end == 0) // '/' at first
		{
			char* token = calloc(1,sizeof(char)*2);
			strings_copy(token, text, start, 1);

			res.strings[i] = token;
		}

		end++;
		start = end;
	}

	return res;
}

void strings_free_strings_t(strings_t* s)
{
	for(int i=0 ; i<s->size ; i++)
	{
		if(s->strings[i])
		{
			free(s->strings[i]);
			s->strings[i] = 0;
		}
	}

	s->size=0;
}
