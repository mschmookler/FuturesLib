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

#ifndef FUTURESLIB_FUTURESPRODUCT_H
#define FUTURESLIB_FUTURESPRODUCT_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

using std::string;

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


	// Default constructor
	FuturesProduct() = default;

	// Main constructor
	FuturesProduct(string exch, int p_complex, int ms_id,
			string sec_group, string sec_type,
			string curr, char match_algo,
			float uomq, float mpi, float df) :
		exchange(exch),
		product_complex(p_complex),
		mkt_seg_id(ms_id),
		security_group(sec_group),
		security_type(sec_type),
		currency(curr),
		match_algorithm(match_algo),
		unit_of_measure_qty(uomq),
		min_price_increment(mpi),
		display_factor(df){}


	string GetExchange() {
		return exchange;
	}

	int GetComplex() {
		return product_complex;
	}

	int GetMktSegID() {
		return mkt_seg_id;
	}

	string GetSecurityGroup() {
		return security_group;
	}

	string GetSecurityType() {
		return security_type;
	}

	string GetCurrency() {
		return currency;
	}

	char GetMatchAlgo() {
		return match_algorithm;
	}

	float GetUOMQ() {
		return unit_of_measure_qty;
	}

	float TickSize() {
		return min_price_increment;
	}

	float GetDF() {
		return display_factor;
	}

protected:
	
	void SetExchange(string s) {
		exchange = s;
	}

	void SetComplex(int i) {
		product_complex = i;
	}

	void SetMktSegID(int i) {
		mkt_seg_id = i;
	}

	void SetSecurityGroup(string s) {
		security_group = s;
	}

	void SetSecurityType(string s) {
		security_type = s;
	}

	void SetCurrency(string s) {
		currency = s;
	}

	void SetMatchAlgo(char c) {
		match_algorithm = c;
	}

	void SetUOMQ(float f) {
		unit_of_measure_qty = f;
	}

	void SetMPI(float f) {
		min_price_increment = f;
	}

	void SetDF(float f) {
		display_factor = f;
	}


private:
	// tag 207 = SecurityExchange
	// Exchange code: XCME, XCBT, XNYM, XCEC, etc.
	// char exchange[8] = {};
	string exchange = "";

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

	// tag 1300 = MarketSegmentID
	int mkt_seg_id = 0;

	// tag 1151 = Security Group
	// Exchange symbol for the given product group.
	// Not to be confused with tag 6937 = Asset.
	// Ex. ES, ZN, ZC, ZE (GE Opt), GE
	// Corresponds to tag 55 = Symbol for iLink Order Entry
	// char security_group[8] = {};
	string security_group = "";

	// tag 762 = SecuritySubType
	// Indicates spread or combo type.
	// Ex. SP, BF, DF, CF, VT
	// char security_type[4] = { '\0' };
	string security_type = "";

	// tag 15 = Currency
	// Currency used in price.
	// char currency[4] = "USD";
	string currency = "";

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
	// TODO: Store tag 872 in uint32_t.
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