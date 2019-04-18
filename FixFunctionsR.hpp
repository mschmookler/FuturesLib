/*
FuturesLib is a package for interacting with futures and options on futures.

Version 0.0.0

Copyright(C) 2019 Matthew A Schmookler

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License v3 as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef FUTURESLIB_FIXFUNCTIONSR_H
#define FUTURESLIB_FIXFUNCTIONSR_H

#include "FuturesContract.hpp"
#include "J:/SCHMOOKLER/src/hffix.hpp"
#include <fstream>		// For writing log file

namespace flf {

int atoint(char* begin, char* end)
{
	int val(0);

	for (; begin < end; ++begin) {
		val *= 10u;
		val += (int)(*begin - '0');
	}
	return val;
}

string GetTagValue(int tag, string FIX_msg)
{
	string str_to_find = '\x01' + std::to_string(tag) + '=';
	int begin, end;
	if (FIX_msg.find(str_to_find) == FIX_msg.npos)
		return "";
	else {
		begin = FIX_msg.find(str_to_find) + str_to_find.length();
		end = FIX_msg.find('\x01', begin);
	}
	return FIX_msg.substr(begin, end - begin);
}

// Given a string containing a FIX msg and an integer tag
// returns the value (as ASCII) of the first occurrence of 
// the given tag.
string getTagValue(int tag, string FIX_msg)
{
	char *str = const_cast<char *>(FIX_msg.c_str());
	char *tok, *next_tok, *sub_tok, *next_sub_tok;
	tok = strtok_s(str, "\x01", &next_tok);
	while (tok != NULL) {
		sub_tok = strtok_s(tok, "=", &next_sub_tok);
		if (tag == atoint(sub_tok, next_sub_tok - 1)) {
			string ret(next_sub_tok, next_tok - next_sub_tok);
			return ret;
		}
		tok = strtok_s(NULL, "\x01", &next_tok);
	}
}

char* get_tag_value(char* tag_list, char* tag)
{
	char* tag_list_copy = malloc(strlen(tag_list) + 1);
	char* result = 0;
	char* s;

	strcpy(tag_list_copy, tag_list);
	s = strtok(tag_list_copy, "\x01");
	while (s)
	{
		char * equals_sign = strchr(s, '=');
		if (equals_sign)
		{
			*equals_sign = 0;
			if (0 == strcmp(s, tag))
			{
				equals_sign++;
				result = malloc(strlen(equals_sign) + 1);
				strcpy(result, equals_sign);
			}
		}
		s = strtok(NULL, "\x01");
	}
	free(tag_list_copy);
	return result;
}


// void UpdateContractFromSecdef(FuturesContract *fp, string secdef_line);

// TODO: LoadContractFromSecdef("GEZ9")
// Check if database has entry "GEZ9"
// If yes, update current entry.
// If no, create temp object A.
// Parse secdef file until "\x0155=GEZ9\x01" is found and return line number
// Parse line and populate A with relevant values.
// Write A to database.
// Delete A.

} // namespace flf

#endif // FUTURESLIB_FIXFUNCTIONSR_H