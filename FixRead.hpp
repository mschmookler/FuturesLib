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

#include "J:/SCHMOOKLER/src/hffix.hpp"
#include <iostream>
#include <fstream>		// For writing log file
#include <sys/stat.h>	// For stat

namespace flf {

/*
int atoint(char* begin, char* end)
{
	int val(0);

	for (; begin < end; ++begin) {
		val *= 10u;
		val += (int)(*begin - '0');
	}
	return val;
}*/

/*! \brief Returns the field value for the FIRST instance of a given tag number
 *
 *  Do not use for repeating groups.
 */
std::string get_tag_value(int tag, std::string FIX_msg)
{
	std::string str_to_find = '\x01' + std::to_string(tag) + '=';
	int begin, end;
	if (FIX_msg.find(str_to_find) == std::string::npos)
	{
		std::cout << "Not found" << '\n';
		return "";
	}
	else {
		begin = FIX_msg.find(str_to_find) + str_to_find.length();
		end = FIX_msg.find('\x01', begin);
	}
	return FIX_msg.substr(begin, end - begin);
}

/*! \brief Filters secdef data file according to a matching criterion.
 *
 *  Given a secdef file containing Globex products, an output filename,
 *  and a tag value pair, filters the full product list into products that
 *  match the filter.
 *  Note: If the output file already exists, this function does nothing.
 */
void FilterSecdef(std::string file_to_filter, std::string output_file,
	int tag_num, std::string tag_val)
{
	/* This snippet exits the function if the output file already exists. */
	struct stat buf;
	if (stat(output_file.c_str(), &buf) != -1)
		return;
	
	std::ifstream readfile(file_to_filter);
	std::ofstream outfile(output_file);
	std::string line;

	if (readfile.is_open() && outfile.is_open())
	{
		while (std::getline(readfile, line))
		{
			if (flf::get_tag_value(tag_num, line) == tag_val)
				outfile << line << '\n';
		}
	}
	readfile.close();
	outfile.close();
}

/* Pseudocode:

for each globex_product_symbol in list
{
	new_file_name = prefix + globex_product_symbol + ".txt";
	FilterSecdef(all_futures.txt, new_file_name, 1151, globex_product_symbol);
}
*/

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