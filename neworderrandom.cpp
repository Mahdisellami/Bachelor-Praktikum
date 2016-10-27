#include <cstdint>
#include "Types.hpp"
#include "Schema.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <tuple>
#include <fstream>
#include <ctime>

using namespace std;

const int32_t warehouses = 5;

namespace std {
template<>
struct hash<Integer> {
	size_t operator()(const Integer& k) const {
		return hash<int32_t>()(k.value);
	}
};

template<>
struct hash<tuple<Integer, Integer>> {
	size_t operator()(tuple<Integer, Integer> const& k) const {
		size_t h1 = hash<Integer>()(get<0>(k));
		size_t h2 = hash<Integer>()(get<1>(k));
		return h1 ^ (h2 << 1);
	}
};

template<>
struct hash<tuple<Integer, Integer, Integer>> {
	size_t operator()(tuple<Integer, Integer, Integer> const& k) const {
		size_t h1 = hash<Integer>()(get<0>(k));
		size_t h2 = hash<Integer>()(get<1>(k));
		size_t h3 = hash<Integer>()(get<2>(k));
		return h1 ^ ((h2 ^ (h3 << 1)) << 1);
	}
};

template<>
struct hash<tuple<Integer, Integer, Integer, Integer>> {
	size_t operator()(
			tuple<Integer, Integer, Integer, Integer> const& k) const {
		size_t h1 = hash<Integer>()(get<0>(k));
		size_t h2 = hash<Integer>()(get<1>(k));
		size_t h3 = hash<Integer>()(get<2>(k));
		size_t h4 = hash<Integer>()(get<3>(k));
		return h1 ^ ((h2 ^ ((h3 ^ (h4 << 1)) << 1)) << 1);
	}
};
}
//Warehouses
vector<Warehouse> warehouses_2;
//Warehouses primary key
unordered_map<Integer, Tid> w_p_k;
//Districts
vector<District> districts;
//Warehouses primary key
unordered_map<tuple<Integer, Integer>, Tid> d_p_k;
//Customers
vector<Customer> customers;
//Customers primary key
unordered_map<tuple<Integer, Integer, Integer>, Tid> c_p_k;
//History
vector<History> history;
//NewOrders
vector<NewOrder> newOrders;
//NewOrders primary key
unordered_map<tuple<Integer, Integer, Integer>, Tid> no_p_k;
//Orders
vector<Order> orders;
//Orders primary key
unordered_map<tuple<Integer, Integer, Integer>, Tid> o_p_k;
//OrderLines
vector<OrderLine> orderLines;
//OrderLines primary key
unordered_map<tuple<Integer, Integer, Integer, Integer>, Tid> ol_p_k;
//Items
vector<Item> items;
//Items primary key
unordered_map<Integer, Tid> i_p_k;
//Stock
vector<Stock> stock;
//Stock primary key
unordered_map<tuple<Integer, Integer>, Tid> s_p_k;

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

			//verify the primary key

			if (w_p_k.find(w_id) == w_p_k.end()) {
				w_p_k.insert( { w_id, warehouses_2.size() });
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
				Warehouse w = { w_id, w_name, w_street_1, w_street_2, w_city,
						w_state, w_zip, w_tax, w_ytd, };
				warehouses_2.push_back(w);
			} else {
				cout << "Primary key must be unique!\n";
			}
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

			if (d_p_k.find( { d_id, d_w_id }) == d_p_k.end()) {
				d_p_k.insert( { { d_id, d_w_id }, districts.size() });
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
				District d = { d_id, d_w_id, d_name, d_street_1, d_street_2,
						d_city, d_state, d_zip, d_tax, d_ytd, d_next_o_id, };
				districts.push_back(d);
			} else {
				cout << "Primary key must be unique!\n";
			}
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

			//verify the primary key

			if (c_p_k.find( { c_id, c_d_id, c_w_id }) == c_p_k.end()) {
				c_p_k.insert( { { c_id, c_d_id, c_w_id }, customers.size() });
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
				Customer c = { c_id, c_d_id, c_w_id, c_first, c_middle, c_last,
						c_street_1, c_street_2, c_city, c_state, c_zip, c_phone,
						c_since, c_credit, c_credit_lim, c_discount, c_balance,
						c_ytd_paymenr, c_payment_cnt, c_delivery_cnt, c_data, };
				customers.push_back(c);
			} else {
				cout << "Primary key must be unique!\n";
			}
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
			History h = { h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date,
					h_amount, h_data, };
			history.push_back(h);
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

			//verify the primary key

			if (no_p_k.find( { no_o_id, no_d_id, no_w_id }) == no_p_k.end()) {
				no_p_k.insert(
						{ { no_o_id, no_d_id, no_w_id }, newOrders.size() });
				NewOrder no = { no_o_id, no_d_id, no_w_id, };
				newOrders.push_back(no);
			} else {
				cout << "Primary key must be unique!\n";
			}
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

			//verify the primary key

			if (o_p_k.find( { o_id, o_d_id, o_w_id }) == o_p_k.end()) {
				o_p_k.insert( { { o_id, o_d_id, o_w_id }, orders.size() });
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
				Order o = { o_id, o_d_id, o_w_id, o_c_id, o_entry_d,
						o_carrier_id, o_ol_cnt, o_all_local, };
				orders.push_back(o);
			} else {
				cout << "Primary key must be unique!\n";
			}
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

			//verify the primary key

			if (ol_p_k.find( { ol_o_id, ol_d_id, ol_w_id, ol_number })
					== ol_p_k.end()) {
				ol_p_k.insert( { { ol_o_id, ol_d_id, ol_w_id, ol_number },
						orderLines.size() });
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
				OrderLine ol = { ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id,
						ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount,
						ol_dist_info, };
				orderLines.push_back(ol);
			} else {
				cout << "Primary key must be unique!\n";
			}
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

			//verify the primary key

			if (i_p_k.find(i_id) == i_p_k.end()) {
				i_p_k.insert( { i_id, items.size() });
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
				Item i = { i_id, i_im_id, i_name, i_price, i_data, };
				items.push_back(i);
			} else {
				cout << "Primary key must be unique!\n";
			}
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

			if (s_p_k.find( { s_i_id, s_w_id }) == d_p_k.end()) {
				s_p_k.insert( { { s_i_id, s_w_id }, stock.size() });
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
				Stock s = { s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02,
						s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07,
						s_dist_08, s_dist_09, s_dist_10, s_ytd, s_order_cnt,
						s_remote_cnt, s_data, };
				stock.push_back(s);
			} else {
				cout << "Primary key must be unique!\n";
			}
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

	uint64_t tid = w_p_k.at(w_id);
	Numeric<4, 4> w_tax = warehouses_2.at(tid).w_tax;

	tid = c_p_k.at( { c_id, d_id, w_id });
	Numeric<4, 4> c_discount = customers.at(tid).c_discount;

	tid = d_p_k.at( { d_id, w_id });
	Integer o_id = districts.at(tid).d_next_o_id;
	Numeric<4, 4> d_tax = districts.at(tid).d_tax;
	districts.at(tid).d_next_o_id = o_id + 1;

	Integer all_local = 1;
	for (int32_t index = 0; index < ol_cnt - 1; index++) {
		if (w_id != supware[index])
			all_local = 0;
	}
	if (o_p_k.find( { o_id, d_id, w_id }) == o_p_k.end()) {
		o_p_k.insert( { { o_id, d_id, w_id }, orders.size() });
		Order o = { o_id, d_id, w_id, c_id, now, 0, ol_cnt, all_local, };
		orders.push_back(o);
	}

	if (no_p_k.find( { o_id, d_id, w_id }) == no_p_k.end()) {
		no_p_k.insert( { { o_id, d_id, w_id }, newOrders.size() });
		NewOrder no = { o_id, d_id, w_id, };
		newOrders.push_back(no);
	}

	for (int32_t index = 0; index < ol_cnt - 1; index++) {
		tid = i_p_k.at(itemid[index]);
		Numeric<5, 2> i_price = items.at(tid).i_price;

		tid = s_p_k.at( { itemid[index], supware[index] });
		Numeric<4, 0> s_quantity = stock.at(tid).s_quantity;
		Numeric<4, 0> s_remote_cnt = stock.at(tid).s_remote_cnt;
		Numeric<4, 0> s_order_cnt = stock.at(tid).s_order_cnt;
		Char<24> s_dist;
		switch (d_id) {
		case 1:
			s_dist = stock.at(tid).s_dist_01;
			break;
		case 2:
			s_dist = stock.at(tid).s_dist_02;
			break;
		case 3:
			s_dist = stock.at(tid).s_dist_03;
			break;
		case 4:
			s_dist = stock.at(tid).s_dist_04;
			break;
		case 5:
			s_dist = stock.at(tid).s_dist_05;
			break;
		case 6:
			s_dist = stock.at(tid).s_dist_06;
			break;
		case 7:
			s_dist = stock.at(tid).s_dist_07;
			break;
		case 8:
			s_dist = stock.at(tid).s_dist_08;
			break;
		case 9:
			s_dist = stock.at(tid).s_dist_09;
			break;
		case 10:
			s_dist = stock.at(tid).s_dist_10;
			break;
		}
		if (s_quantity > qty[index]) {
			stock.at(tid).s_quantity = s_quantity - qty[index];
		} else {
			stock.at(tid).s_quantity = s_quantity + 91 - qty[index];
		}
		tid = s_p_k.at( { itemid[index], w_id });
		if (supware[index] != w_id) {
			stock.at(tid).s_remote_cnt = s_remote_cnt + 1;
		} else {
			stock.at(tid).s_order_cnt = s_order_cnt + 1;
		}
		int64_t ol_amount = qty[index] * i_price.value
				* (1.0 + w_tax.value + d_tax.value) * (1.0 - c_discount.value);
		if (ol_p_k.find( { o_id, d_id, w_id, index + 1 }) == ol_p_k.end()) {
			ol_p_k.insert(
					{ { o_id, d_id, w_id, index + 1 }, orderLines.size() });
			OrderLine ol = { o_id, d_id, w_id, index + 1, itemid[index],
					supware[index], 0, qty[index], ol_amount, s_dist, };
			orderLines.push_back(ol);
		}
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
	cout << "Orders: " << orders.size() << "\n";
	cout << "New Orders: " << newOrders.size() << "\n";
	cout << "Order Lines: " << orderLines.size() << "\n";
	clock_t begin = clock();
	for (int i = 0; i < 1000000; i++) {
		newOrderRandom();
	}
	clock_t end = clock();
	double elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC) / 100;
	double transactions = 10000.0 / elapsed_secs;
	cout << transactions << " Transactions per second.\n";
	cout << "Orders: " << orders.size() << "\n";
	cout << "New Orders: " << newOrders.size() << "\n";
	cout << "Order Lines: " << orderLines.size() << "\n";
	return 0;
}
