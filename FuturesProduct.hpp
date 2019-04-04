/*
FuturesLib is a package for interacting with futures and options on futures.
This is my first Github project with the main goal being to practice coding
structure, style, and readability. The first version will be developed solely
to CME Globex. All suggestions are welcome.

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

#ifndef FUTURESLIB_FUTURESPRODUCT_H
#define FUTURESLIB_FUTURESPRODUCT_H

#include <iostream>



// Struct to store data for one futures product.
// A futures product is a non-date-specific futures contract.
// Ex. Corn future (ZC) is a futures product, while ZCN9 (specific date)
// is a futures contract. Additionally, Corn calendar spread (ZC-ZC) 
// would be a separate product. This struct is rarely used by itself.
struct FuturesProduct
{
public:
	// Constructor TODO:
	// 1. Construct from secdef.dat
	// 2. Construct from incoming FIX


	char* GetExchange() {
		return exchange;
	}

	int GetComplex() {
		return product_complex;
	}

	void print_exchange() {
		char* p = exchange;
		while (*p) {
			std::cout << *p++;
		}
		std::cout << '\n';
	}

	void print_security_group() {
		char* p = security_group;
		while (*p) {
			std::cout << *p++;
		}
		std::cout << '\n';
	}

protected:
	// Sometimes a specific futures contract will need to override
	// default MinPriceIncrement. Ex. When a GE contract becomes front month.
	// TODO: SetMinPriceIncrement
	void SetMinPriceIncrement(float f) {
		min_price_increment = f;
	}

private:
	// tag 207 = SecurityExchange
	// Exchange code: XCME, XCBT, XNYM, XCEC, etc.
	char exchange[8] = {};

	// tag 462 = UnderlyingProduct
	// Aka product complex or asset class.
	// Valid values are:
	//  2 = Commodity/Agriculture
	//  4 = Currency
	//  5 = Equity
	// 12 = Other
	// 14 = Interest Rate
	// 15 = FX Cash
	// 16 = Energy
	// 17 = Metals
	int product_complex = 0;

	// tag 1151 = Security Group
	// Exchange symbol for the given product group.
	// Not to be confused with tag 6937 = Asset.
	// Ex. ES, ZN, ZC, ZE (GE Opt), GE
	// Corresponds to tag 55 = Symbol for iLink Order Entry
	char security_group[8] = {};

	// tag 762 = SecuritySubType
	// Indicates spread or combo type.
	// Ex. SP, BF, DF, CF, VT
	char security_type[4] = { '\0' };

	// tag 15 = Currency
	// Currency used in price.
	char currency[4] = "USD";

	// tag 1142 = MatchAlgorithm
	// CME assigned values:
	// F, K, C, A, T, O, S, Q, Y
	char match_algorithm = '\0';

	// tag 1147 = UnitOfMeasureQty
	// Contract size for each instrument. Use with tag 996 = UOM.
	float unit_of_measure_qty = 0;

	// tag 969 = MinPriceIncrement
	// Minimum tick value for this futures product. If front month
	// contract has a different min tick, this is reflected in the
	// derived class, FuturesContract.
	float min_price_increment = 0;

	// tag 9787 = DisplayFactor
	// Muliplier to convert display price to conventional price.
	// Ex. ES: 0.010000000. ZT: 1.000000000
	float display_factor = 0;

	// True if price ticks in fractions. Ex. notes, bonds
	bool is_fractional_price = false;

	// tag 37702 = MainFraction
	// Denominator of main fraction. Ex. 32 for notes, bonds
	unsigned int main_fraction = 0;

	// tag 37703 = SubFraction
	// Denominator of sub fraction. Ex. 8 for ZT, 4 for ZF, 2 for ZN
	unsigned int sub_fraction = 0;

	// tag 9800 = PriceDisplayFormat
	// Number of digits to the right of tick mark or location of
	// tick mark between whole and non-whole numbers.
	int price_display_format = 0;
};

namespace pcomplex {
	enum {
		CommodityAg = 2,
		Currency = 4,
		Equity = 5,
		Other = 12,
		InterestRate = 14,
		FXCash = 15,
		Energy = 16,
		Metals = 17
	};
}

#endif // FUTURESLIB_FUTURESPRODUCT_H