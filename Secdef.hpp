/*
FuturesLib is a package for interacting with futures and options on futures.

Version 0.2.0

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

/*!
 * \file
 * FuturesLib SecDef
 * Functions for reading and processing secdef.dat file.
 */

#ifndef FUTURESLIB_SECDEF_H
#define FUTURESLIB_SECDEF_H

#include "FixRead.hpp"
#include <string>

#include <vector>
 

/*! \brief Generates CSV file containing all futures instruments
 *
 *  Given a list of product symbols, uses secdef files to create a CSV of all
 *  futures instruments belonging to the list.
 *  Secdef file is found at ftp://ftp.cmegroup.com/SBEFix/Production/secdef.dat.gz
 *
void DatabaseCSVfromSecdef(std::vector<std::string> product_list)
{
	int tag_list[] = {
	55,
	48,
	207,
	462,
	1151,
	6937,
	762,
	15,
	1142,
	1147,
	9787,
	200,
	969
	};

	std::vector<std::string> file_list;

	// Loop through product_list
	for (int i = 0; i < product_list.size(); i++)
	{
		// Name of file containing all futures in a specific asset.
		std::string filename1 = "H:\\secdef\\secdef_fut_" + product_list[i] + ".txt";

		// Generates a text file for each product
		FilterSecdef("H:\\secdef\\secdef_fut.txt", filename1, 6937, product_list[i]);

		// Add generated file to file_list
		file_list.push_back(filename1);
	}

	// Open target file for writing
	std::ofstream outfile("H:\\secdef\\all_futures_csv.txt");

	// Loop through file_list
	for (int i = 0; i < file_list.size(); i++)
	{
		std::ifstream readfile(file_list[i]);
		std::string line;

		if (readfile.is_open() && outfile.is_open())
		{
			std::cout << "Generating futures from " << file_list[i] << '\n';
			while (std::getline(readfile, line))
			{
				std::string newline = "";
				for (int j = 0; j < 13; j++)	// size of tag_list
				{
					newline.append(flf::get_tag_value(tag_list[j], line));
					newline.push_back(',');
				}
				newline.append(flf::get_activation_time(line));
				newline.push_back(',');
				newline.append(flf::get_expiration_time(line));
				outfile << newline << '\n';
			}
		}
	}
}*/

/*! \brief Class for reading and processing secdef.dat file.
 *
 *  
 *
 *
 */
class Secdef_Reader
{
public:

	/*!
	\brief Construct from secdef.dat
	\param file Full path to secdef.dat file
	*/
	Secdef_Reader(std::string file)
	{
		readfile.open(file);
	}


	/*!
	\brief Closes secdef.dat
	*/
	~Secdef_Reader()
	{
		readfile.close();
	}

	/*!
	\brief True if secdef.dat is open.
	*/
	bool is_open()
	{
		return readfile.is_open();
	}

	/*void getline()
	{
		readfile.getline(buffer_, 2000);
		buffer_size_ = readfile.gcount();	// num of chars read
	}*/

	
	/*!
	\brief Filters secdef data file according to a matching criterion.
	
	Given an output filename, and a tag value pair, filters the full
	product list into products that match the filter.
	Note: If the output file already exists, this function does nothing.

	\param[in] outfile Full path for output file
	\param[in] tag_num Integer tag number
	\param[in] tag_val String tag value
	*/
	void filter(std::string outfile, int tag_num, std::string tag_val)
	{
		/// This snippet exits the function if the output file already exists.
		struct stat buf;
		if (stat(outfile.c_str(), &buf) != -1)
			return;

		std::ofstream writefile(outfile);
		std::string line;

		if (is_open() && writefile.is_open())
		{
			while (std::getline(readfile, line))
			{
				if (flf::get_tag_value(tag_num, line) == tag_val)
					writefile << line << '\n';
			}
		}

		writefile.close();
	}
	
	std::string filterFutures()
	{
		std::string new_filename = filename.substr(0, filename.length() - 4) + "_fut.txt";
		filter(new_filename, 167, "FUT");
		return new_filename;
	}


private:
	std::string filename;
	std::ifstream readfile;

	/*!
	\brief Buffer for line processing
	
	This buffer is used for line processing and should
	be large enough to fit any line in secdef file.
	*/
	//static char buffer_[2000];

	//int buffer_size_;

};


#endif