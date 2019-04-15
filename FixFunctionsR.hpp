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

// void UpdateContractFromSecdef(FuturesContract *fp, string secdef_line);

// TODO: LoadContractFromSecdef("GEZ9")
// Check if database has entry "GEZ9"
// If yes, update current entry.
// If no, create temp object A.
// Parse secdef file until "\x0155=GEZ9\x01" is found and return line number
// Parse line and populate A with relevant values.
// Write A to database.
// Delete A.

#endif // FUTURESLIB_FIXFUNCTIONSR_H