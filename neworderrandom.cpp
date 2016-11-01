#include <cstdint>
#include "Types.hpp"
#include "Schema.hpp"
#include "Warehouse.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <tuple>
#include <fstream>
#include <ctime>

using namespace std;

const int32_t warehouses = 5;

//TPCC
TPCC tpcc;

int32_t urand(int32_t min, int32_t max) {
	return (random() % (max - min + 1)) + min;
}

int32_t urandexcept(int32_t min, int32_t max, int32_t v) {
	if (max <= min)
		return min;
	int32_t r = (random() % (max - min)) + min;
	if (r >= v)
		return r + 1;
	else
		return r;
}

int32_t nurand(int32_t A, int32_t x, int32_t y) {
	return ((((random() % A) | (random() % (y - x + 1) + x)) + 42) % (y - x + 1))
			+ x;
}

void addWarehouses() {
	cout << "Adding the Warehouses...";
	//read from warehouse.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_warehouse.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer w_id = Integer::castString(current.c_str(), current.size());

				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<10> w_name = Varchar<10>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> w_street_1 = Varchar<20>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> w_street_2 = Varchar<20>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> w_city = Varchar<20>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<2> w_state = Char<2>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<9> w_zip = Char<9>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 4> w_tax = Numeric<4, 4>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line;
				Numeric<12, 2> w_ytd = Numeric<12, 2>::castString(
						current.c_str(), current.length());
				tpcc.warehouse.insert(w_id, w_name, w_street_1 , w_street_2, w_city,
						w_state, w_zip, w_tax, w_ytd);
		}
	}
	cout << "Done." << "\n";
}

void addDistricts() {
	cout << "Adding the Districts...";
	//read from district.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_district.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer d_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer d_w_id = Integer::castString(current.c_str(),
					current.length());

			//verify the primary key
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<10> d_name = Varchar<10>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> d_street_1 = Varchar<20>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> d_street_2 = Varchar<20>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> d_city = Varchar<20>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<2> d_state = Char<2>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<9> d_zip = Char<9>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 4> d_tax = Numeric<4, 4>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<12, 2> d_ytd = Numeric<12, 2>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line;
				Integer d_next_o_id = Integer::castString(current.c_str(),
						current.length());
				tpcc.district.insert(d_id, d_w_id, d_name, d_street_1, d_street_2,
						d_city, d_state, d_zip, d_tax, d_ytd, d_next_o_id);
		}
	}
	cout << "Done." << "\n";
}

void addCustomers() {
	cout << "Adding the Customers...";
	//read from customer.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_customer.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer c_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer c_d_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer c_w_id = Integer::castString(current.c_str(),
					current.length());

				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<16> c_first = Varchar<16>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<2> c_middle = Char<2>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<16> c_last = Varchar<16>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> c_street_1 = Varchar<20>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> c_street_2 = Varchar<20>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<20> c_city = Varchar<20>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<2> c_state = Char<2>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<9> c_zip = Char<9>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<16> c_phone = Char<16>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Timestamp c_since = Timestamp::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<2> c_credit = Char<2>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<12, 2> c_credit_lim = Numeric<12, 2>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 4> c_discount = Numeric<4, 4>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<12, 2> c_balance = Numeric<12, 2>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<12, 2> c_ytd_paymenr = Numeric<12, 2>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 0> c_payment_cnt = Numeric<4, 0>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 0> c_delivery_cnt = Numeric<4, 0>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<500> c_data = Varchar<500>::castString(current.c_str(),
						current.length());
				tpcc.customer.insert( c_id, c_d_id, c_w_id, c_first, c_middle, c_last,
						c_street_1, c_street_2, c_city, c_state, c_zip, c_phone,
						c_since, c_credit, c_credit_lim, c_discount, c_balance,
						c_ytd_paymenr, c_payment_cnt, c_delivery_cnt, c_data);
		}
	}
	cout << "Done." << "\n";
}

void addHistory() {
	cout << "Adding the History...";
	//read from history.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_history.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer h_c_id = Integer::castString(current.c_str(),
					current.size());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer h_c_d_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer h_c_w_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer h_d_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer h_w_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Timestamp h_date = Timestamp::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Numeric<6, 2> h_amount = Numeric<6, 2>::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Varchar<24> h_data = Varchar<24>::castString(current.c_str(),
					current.length());
			tpcc.history.insert(h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date,
					h_amount, h_data);
		}
	}
	cout << "Done." << "\n";
}

void addNewOrders() {
	cout << "Adding the new Orders...";
	//read from neworder.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_neworder.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer no_o_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer no_d_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer no_w_id = Integer::castString(current.c_str(),
					current.length());

				tpcc.newOrder.insert(no_o_id, no_d_id, no_w_id);
		}
	}
	cout << "Done." << "\n";
}

void addOrders() {
	cout << "Adding the Orders...";
	//read from order.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_order.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer o_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer o_d_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer o_w_id = Integer::castString(current.c_str(),
					current.length());

				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Integer o_c_id = Integer::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Timestamp o_entry_d = Timestamp::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Integer o_carrier_id = Integer::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<2, 0> o_ol_cnt = Numeric<2, 0>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<1, 0> o_all_local = Numeric<1, 0>::castString(
						current.c_str(), current.length());
				tpcc.order.insert(o_id, o_d_id, o_w_id, o_c_id, o_entry_d,
						o_carrier_id, o_ol_cnt, o_all_local);
		}
	}
	cout << "Done." << "\n";
}

void addOrderLines() {
	cout << "Adding the Order Lines...";
	//read from orderline.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_orderline.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer ol_o_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer ol_d_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer ol_w_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer ol_number = Integer::castString(current.c_str(),
					current.length());

				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Integer ol_i_id = Integer::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Integer ol_supply_w_id = Integer::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Timestamp ol_delivery_d = Timestamp::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<2, 0> ol_quantity = Numeric<2, 0>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<6, 2> ol_amount = Numeric<6, 2>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> ol_dist_info = Char<24>::castString(current.c_str(),
						current.length());
				tpcc.orderLine.insert(ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id,
						ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount,
						ol_dist_info);
		}
	}
	cout << "Done." << "\n";
}

void addItems() {
	cout << "Adding the Items...";
	//read from item.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open("tpcc_item.tbl"); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer i_id = Integer::castString(current.c_str(), current.size());

				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Integer i_im_id = Integer::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<24> i_name = Varchar<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<5, 2> i_price = Numeric<5, 2>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<50> i_data = Varchar<50>::castString(current.c_str(),
						current.length());
				tpcc.item.insert(i_id, i_im_id, i_name, i_price, i_data);
		}
	}
	cout << "Done." << "\n";
}

void addStock(string filename) {
	//read from stock.tbl/stock_2.tbl line wise

	string line;
	ifstream inputFile; //Input file stream object

	inputFile.open(filename); //Opening the file
	if (!inputFile)
		cout << "there's something wrong with the file!\n"; //Throw exception

	while (!inputFile.eof()) {
		getline(inputFile, line);
		if (line.size()) {
			string current = line.substr(0, line.find_first_of('|'));
			Integer s_i_id = Integer::castString(current.c_str(),
					current.length());
			line = line.substr(line.find_first_of('|') + 1,
					line.length() - line.find_first_of('|'));
			current = line.substr(0, line.find_first_of('|'));
			Integer s_w_id = Integer::castString(current.c_str(),
					current.length());

			//verify the primary key
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 0> s_quantity = Numeric<4, 0>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_01 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_02 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_03 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_04 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_05 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_06 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_07 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_08 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_09 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Char<24> s_dist_10 = Char<24>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<8, 0> s_ytd = Numeric<8, 0>::castString(current.c_str(),
						current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 0> s_order_cnt = Numeric<4, 0>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Numeric<4, 0> s_remote_cnt = Numeric<4, 0>::castString(
						current.c_str(), current.length());
				line = line.substr(line.find_first_of('|') + 1,
						line.length() - line.find_first_of('|'));
				current = line.substr(0, line.find_first_of('|'));
				Varchar<50> s_data = Varchar<50>::castString(current.c_str(),
						current.length());
				tpcc.stock.insert( s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02,
						s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07,
						s_dist_08, s_dist_09, s_dist_10, s_ytd, s_order_cnt,
						s_remote_cnt, s_data);
		}
	}
}

void addStock() {
	cout << "Adding the Stock...";
	addStock("tpcc_stock.tbl");
	addStock("tpcc_stock_2.tbl");
	cout << "Done." << "\n";
}

void populateDataBase() {
	cout << "Populating the Data Base..." << "\n";
	addWarehouses();
	addDistricts();
	addCustomers();
	addHistory();
	addNewOrders();
	addOrders();
	addOrderLines();
	addItems();
	addStock();
	cout << "Data is loaded." << "\n";
}

void newOrder(int32_t w_id, int32_t d_id, int32_t c_id, int32_t ol_cnt,
		int32_t* supware, int32_t* itemid, int32_t* qty, Timestamp now) {

	uint64_t tid = tpcc.warehouse.lookup(w_id);
	Numeric<4, 4> w_tax = tpcc.warehouse.get_w_tax(tid);

	tid = tpcc.customer.lookup(w_id, d_id, c_id );
	Numeric<4, 4> c_discount = tpcc.customer.get_c_discount(tid);

	tid = tpcc.district.lookup(w_id, d_id );
	Integer o_id = tpcc.district.get_d_next_o_id(tid);
	Numeric<4, 4> d_tax = tpcc.district.get_d_tax(tid);
	tpcc.district.set_d_next_o_id(tid, o_id + 1);

	Integer all_local = 1;
	for (int32_t index = 0; index < ol_cnt - 1; index++) {
		if (w_id != supware[index])
			all_local = 0;
	}
	tpcc.order.insert(o_id, d_id, w_id, c_id, now, 0, ol_cnt, all_local);

	tpcc.newOrder.insert(o_id, d_id, w_id);

	for (int32_t index = 0; index < ol_cnt - 1; index++) {
		tid = tpcc.item.lookup(itemid[index]);
		Numeric<5, 2> i_price = tpcc.item.get_i_price(tid);

		tid = tpcc.stock.lookup(supware[index], itemid[index]);
		Numeric<4, 0> s_quantity = tpcc.stock.get_s_quantity(tid);
		Numeric<4, 0> s_remote_cnt = tpcc.stock.get_s_remote_cnt(tid);
		Numeric<4, 0> s_order_cnt = tpcc.stock.get_s_order_cnt(tid);
		Char<24> s_dist;
		switch (d_id) {
		case 1:
			s_dist = tpcc.stock.get_s_dist_01(tid);
			break;
		case 2:
			s_dist = tpcc.stock.get_s_dist_02(tid);
			break;
		case 3:
			s_dist = tpcc.stock.get_s_dist_03(tid);
			break;
		case 4:
			s_dist = tpcc.stock.get_s_dist_04(tid);
			break;
		case 5:
			s_dist = tpcc.stock.get_s_dist_05(tid);
			break;
		case 6:
			s_dist = tpcc.stock.get_s_dist_06(tid);
			break;
		case 7:
			s_dist = tpcc.stock.get_s_dist_07(tid);
			break;
		case 8:
			s_dist = tpcc.stock.get_s_dist_08(tid);
			break;
		case 9:
			s_dist = tpcc.stock.get_s_dist_09(tid);
			break;
		case 10:
			s_dist = tpcc.stock.get_s_dist_10(tid);
			break;
		}
		if (s_quantity > qty[index]) {
			tpcc.stock.set_s_quantity(tid, s_quantity - qty[index]);
		} else {
			tpcc.stock.set_s_quantity(tid, s_quantity + 91 - qty[index]);
		}
		tid = tpcc.stock.lookup(w_id ,itemid[index]);
		if (supware[index] != w_id) {
			tpcc.stock.set_s_remote_cnt(tid, s_remote_cnt + 1);
		} else {
			tpcc.stock.set_s_order_cnt(tid, s_order_cnt + 1);
		}
		int64_t ol_amount = qty[index] * i_price.value
				* (1.0 + w_tax.value + d_tax.value) * (1.0 - c_discount.value);
		tpcc.orderLine.insert(o_id, d_id, w_id, index + 1, itemid[index],
					supware[index], 0, qty[index], ol_amount, s_dist);

	}
}

void newOrderRandom() {
	Timestamp now(0);
	int32_t w_id = urand(1, warehouses);
	int32_t d_id = urand(1, 10);
	int32_t c_id = nurand(1023, 1, 3000);
	int32_t ol_cnt = urand(5, 15);

	int32_t supware[15];
	int32_t itemid[15];
	int32_t qty[15];
	for (int32_t i = 0; i < ol_cnt; i++) {
		if (urand(1, 100) > 1)
			supware[i] = w_id;
		else
			supware[i] = urandexcept(1, warehouses, w_id);
		itemid[i] = nurand(8191, 1, 100000);
		qty[i] = urand(1, 10);
	}

	newOrder(w_id, d_id, c_id, ol_cnt, supware, itemid, qty, now);
}

int main() {
	populateDataBase();
	cout << "Orders: " << tpcc.order.order.size() << "\n";
	cout << "New Orders: " << tpcc.newOrder.newOrder.size() << "\n";
	cout << "Order Lines: " << tpcc.orderLine.orderLine.size() << "\n";
	clock_t begin = clock();
	for (int i = 0; i < 1000000; i++) {
		newOrderRandom();
	}
	clock_t end = clock();
	double elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC) / 100;
	double transactions = 10000.0 / elapsed_secs;
	cout << transactions << " Transactions per second.\n";
	cout << "Orders: " << tpcc.order.order.size() << "\n";
	cout << "New Orders: " << tpcc.newOrder.newOrder.size() << "\n";
	cout << "Order Lines: " << tpcc.orderLine.orderLine.size() << "\n";
	return 0;
}
