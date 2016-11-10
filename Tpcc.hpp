#ifndef H_Tpcc
#define H_Tpcc
//---------------------------------------------------------------------------
#include "Types.hpp"
#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

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
//---------------------------------------------------------------------------
//TPCC
//---------------------------------------------------------------------------
// TPCC class
class TPCC {
	//---------------------------------------------------------------------------
	//Warehouse
	//---------------------------------------------------------------------------
	// Warehouse class
	class Warehouse {
		/// Warehouse Row
		struct Warehouse_Row {
			Integer w_id;
			Varchar<10> w_name;
			Varchar<20> w_street_1;
			Varchar<20> w_street_2;
			Varchar<20> w_city;
			Char<2> w_state;
			Char<9> w_zip;
			Numeric<4, 4> w_tax;
			Numeric<12, 2> w_ytd;
			// primary key (w_id)
		};
	public:
		//Warehouse
		vector<Warehouse_Row> warehouse;
		//Warehouse primary key
		unordered_map<Integer, Tid> w_p_k;

		Warehouse() {}

		inline void insert(
				Integer w_id,
				Varchar<10> w_name,
				Varchar<20> w_street_1,
				Varchar<20> w_street_2,
				Varchar<20> w_city,
				Char<2> w_state,
				Char<9> w_zip,
				Numeric<4,4> w_tax,
				Numeric<12,2> w_ytd)
		{
			if (w_p_k.find(w_id) == w_p_k.end()) {
				w_p_k.insert( { w_id, warehouse.size() });
				Warehouse_Row w = { w_id, w_name, w_street_1, w_street_2, w_city,
						w_state, w_zip, w_tax, w_ytd, };
				warehouse.push_back(w);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline uint64_t lookup(Integer w_id){ return w_p_k.at(w_id);}
		inline Integer get_w_id(Tid tid){ return warehouse.at(tid).w_id;}
		inline void set_w_id(Tid tid, Integer w_id){ warehouse.at(tid).w_id = w_id;}
		inline Varchar<10> get_w_name(Tid tid){ return warehouse.at(tid).w_name;}
		inline void set_w_name(Tid tid, Varchar<10> w_name){ warehouse.at(tid).w_name = w_name;}
		inline Varchar<20> get_w_street_1(Tid tid){ return warehouse.at(tid).w_street_1;}
		inline void set_w_street_1(Tid tid, Varchar<20> w_street_1){ warehouse.at(tid).w_street_1 = w_street_1;}
		inline Varchar<20> get_w_street_2(Tid tid){ return warehouse.at(tid).w_street_2;}
		inline void set_w_street_2(Tid tid, Varchar<20> w_street_2){ warehouse.at(tid).w_street_2 = w_street_2;}
		inline Varchar<20> get_w_city(Tid tid){ return warehouse.at(tid).w_city;}
		inline void set_w_city(Tid tid, Varchar<20> w_city){ warehouse.at(tid).w_city = w_city;}
		inline Char<2> get_w_state(Tid tid){ return warehouse.at(tid).w_state;}
		inline void set_w_state(Tid tid, Char<2> w_state){ warehouse.at(tid).w_state = w_state;}
		inline Char<9> get_w_zip(Tid tid){ return warehouse.at(tid).w_zip;}
		inline void set_w_zip(Tid tid, Char<9> w_zip){ warehouse.at(tid).w_zip = w_zip;}
		inline Numeric<4,4> get_w_tax(Tid tid){ return warehouse.at(tid).w_tax;}
		inline void set_w_tax(Tid tid, Numeric<4,4> w_tax){ warehouse.at(tid).w_tax = w_tax;}
		inline Numeric<12,2> get_w_ytd(Tid tid){ return warehouse.at(tid).w_ytd;}
		inline void set_w_ytd(Tid tid, Numeric<12,2> w_ytd){ warehouse.at(tid).w_ytd = w_ytd;}
	};

	//---------------------------------------------------------------------------
	//District
	//---------------------------------------------------------------------------
	// District class
	class District {
		/// District Row
		struct District_Row {
			Integer d_id;
			Integer d_w_id;
			Varchar<10> d_name;
			Varchar<20> d_street_1;
			Varchar<20> d_street_2;
			Varchar<20> d_city;
			Char<2> d_state;
			Char<9> d_zip;
			Numeric<4,4> d_tax;
			Numeric<12,2> d_ytd;
			Integer d_next_o_id;
			// primary key (d_w_id,d_id)
		};
	public:
		//District
		vector<District_Row> district;
		//District primary key
		unordered_map<tuple<Integer, Integer>, Tid> d_p_k;

		District() {}

		inline void insert(
				Integer d_id,
				Integer d_w_id,
				Varchar<10> d_name,
				Varchar<20> d_street_1,
				Varchar<20> d_street_2,
				Varchar<20> d_city,
				Char<2> d_state,
				Char<9> d_zip,
				Numeric<4,4> d_tax,
				Numeric<12,2> d_ytd,
				Integer d_next_o_id)
		{
			if (d_p_k.find( { d_w_id, d_id }) == d_p_k.end()) {
					d_p_k.insert( { { d_w_id, d_id }, district.size() });
					District_Row d = { d_id, d_w_id, d_name, d_street_1, d_street_2,
											d_city, d_state, d_zip, d_tax, d_ytd, d_next_o_id, };
									district.push_back(d);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline uint64_t lookup(Integer d_w_id, Integer d_id){ return d_p_k.at({d_w_id, d_id});}
		inline Integer get_d_id(Tid tid){ return district.at(tid).d_id;}
		inline void set_d_id(Tid tid, Integer d_id){ district.at(tid).d_id = d_id;}
		inline Integer get_d_w_id(Tid tid){ return district.at(tid).d_w_id;}
		inline void set_d_w_id(Tid tid, Integer d_w_id){ district.at(tid).d_w_id = d_w_id;}
		inline Varchar<10> get_d_name(Tid tid){ return district.at(tid).d_name;}
		inline void set_d_name(Tid tid, Varchar<10> d_name){ district.at(tid).d_name = d_name;}
		inline Varchar<20> get_d_street_1(Tid tid){ return district.at(tid).d_street_1;}
		inline void set_d_street_1(Tid tid, Varchar<20> d_street_1){ district.at(tid).d_street_1 = d_street_1;}
		inline Varchar<20> get_d_street_2(Tid tid){ return district.at(tid).d_street_2;}
		inline void set_d_street_2(Tid tid, Varchar<20> d_street_2){ district.at(tid).d_street_2 = d_street_2;}
		inline Varchar<20> get_d_city(Tid tid){ return district.at(tid).d_city;}
		inline void set_d_city(Tid tid, Varchar<20> d_city){ district.at(tid).d_city = d_city;}
		inline Char<2> get_d_state(Tid tid){ return district.at(tid).d_state;}
		inline void set_d_state(Tid tid, Char<2> d_state){ district.at(tid).d_state = d_state;}
		inline Char<9> get_d_zip(Tid tid){ return district.at(tid).d_zip;}
		inline void get_d_zip(Tid tid, Char<9> d_zip){ district.at(tid).d_zip = d_zip;}
		inline Numeric<4,4> get_d_tax(Tid tid){ return district.at(tid).d_tax;}
		inline void set_d_tax(Tid tid, Numeric<4,4> d_tax){ district.at(tid).d_tax = d_tax;}
		inline Numeric<12,2> get_d_ytd(Tid tid){ return district.at(tid).d_ytd;}
		inline void set_d_ytd(Tid tid, Numeric<12,2> d_ytd){ district.at(tid).d_ytd = d_ytd;}
		inline Integer get_d_next_o_id(Tid tid){ return district.at(tid).d_next_o_id;}
		inline void set_d_next_o_id(Tid tid, Integer d_next_o_id){ district.at(tid).d_next_o_id = d_next_o_id;}
	};

	//---------------------------------------------------------------------------
	//Customer
	//---------------------------------------------------------------------------
	// Customer class
	class Customer {
	public:
		/// Customer Row
		struct Customer_Row {
			Integer c_id;
			Integer c_d_id;
			Integer c_w_id;
			Varchar<16> c_first;
			Char<2> c_middle;
			Varchar<16> c_last;
			Varchar<20> c_street_1;
			Varchar<20> c_street_2;
			Varchar<20> c_city;
			Char<2> c_state;
			Char<9> c_zip;
			Char<16> c_phone;
			Timestamp c_since;
			Char<2> c_credit;
			Numeric<12,2> c_credit_lim;
			Numeric<4,4> c_discount;
			Numeric<12,2> c_balance;
			Numeric<12,2> c_ytd_paymenr;
			Numeric<4,0> c_payment_cnt;
			Numeric<4,0> c_delivery_cnt;
			Varchar<500> c_data;
			//primary key (c_w_id,c_d_id,c_id)
		};
		//Customer
		vector<Customer_Row> customer;
		//Customer primary key
		unordered_map<tuple<Integer, Integer, Integer>, Tid> c_p_k;

		Customer() {}

		inline void insert(
				Integer c_id,
				Integer c_d_id,
				Integer c_w_id,
				Varchar<16> c_first,
				Char<2> c_middle,
				Varchar<16> c_last,
				Varchar<20> c_street_1,
				Varchar<20> c_street_2,
				Varchar<20> c_city,
				Char<2> c_state,
				Char<9> c_zip,
				Char<16> c_phone,
				Timestamp c_since,
				Char<2> c_credit,
				Numeric<12,2> c_credit_lim,
				Numeric<4,4> c_discount,
				Numeric<12,2> c_balance,
				Numeric<12,2> c_ytd_paymenr,
				Numeric<4,0> c_payment_cnt,
				Numeric<4,0> c_delivery_cnt,
				Varchar<500> c_data)
		{
			if (c_p_k.find( { c_w_id,c_d_id,c_id }) == c_p_k.end()) {
					c_p_k.insert( { { c_w_id,c_d_id,c_id }, customer.size() });
					Customer_Row c = { c_id, c_d_id, c_w_id, c_first, c_middle, c_last,
							c_street_1, c_street_2, c_city, c_state, c_zip, c_phone,
							c_since, c_credit, c_credit_lim, c_discount, c_balance,
							c_ytd_paymenr, c_payment_cnt, c_delivery_cnt, c_data, };
					customer.push_back(c);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline uint64_t lookup(Integer c_w_id, Integer c_d_id, Integer c_id){ return c_p_k.at({c_w_id,c_d_id,c_id});}
		inline Integer get_c_id(Tid tid){ return customer.at(tid).c_id;}
		inline void set_c_id(Tid tid, Integer c_id){ customer.at(tid).c_id = c_id;}
		inline Integer get_c_d_id(Tid tid){ return customer.at(tid).c_d_id;}
		inline void set_c_d_id(Tid tid, Integer c_d_id){ customer.at(tid).c_d_id = c_d_id;}
		inline Integer get_c_w_id(Tid tid){ return customer.at(tid).c_w_id;}
		inline void set_c_w_id(Tid tid, Integer c_w_id){ customer.at(tid).c_w_id = c_w_id;}
		inline Varchar<16> get_c_first(Tid tid){ return customer.at(tid).c_first;}
		inline void set_c_first(Tid tid, Varchar<16> c_first){ customer.at(tid).c_first = c_first;}
		inline Char<2> get_c_middle(Tid tid){ return customer.at(tid).c_middle;}
		inline void set_c_middle(Tid tid, Char<2> c_middle){ customer.at(tid).c_middle = c_middle;}
		inline Varchar<16> get_c_last(Tid tid){ return customer.at(tid).c_last;}
		inline void set_c_last(Tid tid, Varchar<16> c_last){ customer.at(tid).c_last = c_last;}
		inline Varchar<20> get_c_street_1(Tid tid){ return customer.at(tid).c_street_1;}
		inline void set_c_street_1(Tid tid, Varchar<20> c_street_1){ customer.at(tid).c_street_1 = c_street_1;}
		inline Varchar<20> get_c_street_2(Tid tid){ return customer.at(tid).c_street_2;}
		inline void set_c_street_2(Tid tid, Varchar<20> c_street_2){ customer.at(tid).c_street_2 = c_street_2;}
		inline Varchar<20> get_c_city(Tid tid){ return customer.at(tid).c_city;}
		inline void set_c_city(Tid tid, Varchar<20> c_city){ customer.at(tid).c_city = c_city;}
		inline Char<2> get_c_state(Tid tid){ return customer.at(tid).c_state;}
		inline void set_c_state(Tid tid, Char<2> c_state){ customer.at(tid).c_state = c_state;}
		inline Char<9> get_c_zip(Tid tid){ return customer.at(tid).c_zip;}
		inline void get_c_zip(Tid tid, Char<9> c_zip){ customer.at(tid).c_zip = c_zip;}
		inline Char<16> get_c_phone(Tid tid){ return customer.at(tid).c_phone;}
		inline void get_c_phone(Tid tid, Char<16> c_phone){ customer.at(tid).c_phone = c_phone;}
		inline Timestamp get_c_since(Tid tid){ return customer.at(tid).c_since;}
		inline void get_c_since(Tid tid, Timestamp c_since){ customer.at(tid).c_since = c_since;}
		inline Char<2> get_c_credit(Tid tid){ return customer.at(tid).c_credit;}
		inline void set_c_credit(Tid tid, Char<2> c_credit){ customer.at(tid).c_credit = c_credit;}
		inline Numeric<12,2> get_c_credit_lim(Tid tid){ return customer.at(tid).c_credit_lim;}
		inline void set_c_credit_lim(Tid tid, Numeric<12,2> c_credit_lim){ customer.at(tid).c_credit_lim = c_credit_lim;}
		inline Numeric<4,4> get_c_discount(Tid tid){ return customer.at(tid).c_discount;}
		inline void set_c_discount(Tid tid, Numeric<4,4> c_discount){ customer.at(tid).c_discount = c_discount;}
		inline Numeric<12,2> get_c_balance(Tid tid){ return customer.at(tid).c_balance;}
		inline void set_c_balance(Tid tid, Numeric<12,2> c_balance){ customer.at(tid).c_balance = c_balance;}
		inline Numeric<12,2> get_c_ytd_paymenr(Tid tid){ return customer.at(tid).c_ytd_paymenr;}
		inline void set_c_ytd_paymenr(Tid tid, Numeric<12,2> c_ytd_paymenr){ customer.at(tid).c_ytd_paymenr = c_ytd_paymenr;}
		inline Numeric<4,0> get_c_payment_cnt(Tid tid){ return customer.at(tid).c_payment_cnt;}
		inline void set_c_payment_cnt(Tid tid, Numeric<4,0> c_payment_cnt){ customer.at(tid).c_payment_cnt = c_payment_cnt;}
		inline Numeric<4,0> get_c_delivery_cnt(Tid tid){ return customer.at(tid).c_delivery_cnt;}
		inline void set_c_delivery_cnt(Tid tid, Numeric<4,0> c_delivery_cnt){ customer.at(tid).c_delivery_cnt = c_delivery_cnt;}
		inline Varchar<500> get_c_data(Tid tid){ return customer.at(tid).c_data;}
		inline void set_c_data(Tid tid, Varchar<500> c_data){ customer.at(tid).c_data = c_data;}
	};

	//---------------------------------------------------------------------------
	//History
	//---------------------------------------------------------------------------
	// History class
	class History {
		/// History Row
		struct History_Row {
			Integer h_c_id;
			Integer h_c_d_id;
			Integer h_c_w_id;
			Integer h_d_id;
			Integer h_w_id;
			Timestamp h_date;
			Numeric<6,2> h_amount;
			Varchar<24> h_data;
		};
	public:
		//History
		vector<History_Row> history;

		History() {}

		inline void insert(
				Integer h_c_id,
				Integer h_c_d_id,
				Integer h_c_w_id,
				Integer h_d_id,
				Integer h_w_id,
				Timestamp h_date,
				Numeric<6,2> h_amount,
				Varchar<24> h_data)
		{
			History_Row h = { h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date,
					h_amount, h_data, };
			history.push_back(h);
		}
		inline Integer get_h_c_id(Tid tid){ return history.at(tid).h_c_id;}
		inline void set_h_c_id(Tid tid, Integer h_c_id){ history.at(tid).h_c_id = h_c_id;}
		inline Integer get_h_c_d_id(Tid tid){ return history.at(tid).h_c_d_id;}
		inline void set_h_c_d_id(Tid tid, Integer h_c_d_id){ history.at(tid).h_c_d_id = h_c_d_id;}
		inline Integer get_h_c_w_id(Tid tid){ return history.at(tid).h_c_w_id;}
		inline void set_h_c_w_id(Tid tid, Integer h_c_w_id){ history.at(tid).h_c_w_id = h_c_w_id;}
		inline Integer get_h_d_id(Tid tid){ return history.at(tid).h_d_id;}
		inline void set_h_d_id(Tid tid, Integer h_d_id){ history.at(tid).h_d_id = h_d_id;}
		inline Integer get_h_w_id(Tid tid){ return history.at(tid).h_w_id;}
		inline void set_h_w_id(Tid tid, Integer h_w_id){ history.at(tid).h_w_id = h_w_id;}
		inline Timestamp get_h_date(Tid tid){ return history.at(tid).h_date;}
		inline void set_h_date(Tid tid, Timestamp h_date){ history.at(tid).h_date = h_date;}
		inline Numeric<6,2> get_h_amount(Tid tid){ return history.at(tid).h_amount;}
		inline void set_h_amount(Tid tid, Numeric<6,2> h_amount){ history.at(tid).h_amount = h_amount;}
		inline Varchar<24> get_h_data(Tid tid){ return history.at(tid).h_data;}
		inline void set_d_ytd(Tid tid, Varchar<24> h_data){ history.at(tid).h_data = h_data;}
	};

	//---------------------------------------------------------------------------
	//NewOrder
	//---------------------------------------------------------------------------
	// NewOrder class
	class NewOrder {
		/// NewOrder Row
		struct NewOrder_Row {
			Integer no_o_id;
			Integer no_d_id;
			Integer no_w_id;
			//primary key (no_w_id,no_d_id,no_o_id)
		};
	public:
		//NewOrder
		vector<NewOrder_Row> newOrder;
		//NewOrder primary key
		map<tuple<Integer, Integer, Integer>, Tid> no_p_k;

		NewOrder() {}

		inline void insert(
				Integer no_o_id,
				Integer no_d_id,
				Integer no_w_id)
		{
			if (no_p_k.find( { no_w_id,no_d_id,no_o_id }) == no_p_k.end()) {
					no_p_k.insert( { { no_w_id,no_d_id,no_o_id }, newOrder.size() });
					NewOrder_Row no = { no_o_id, no_d_id, no_w_id, };
					newOrder.push_back(no);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline void deleteElement(Tid tid){
			if (tid < newOrder.size()){
				newOrder.at(tid) = newOrder.at(newOrder.size() - 1);
				newOrder.pop_back();
				map<tuple<Integer, Integer, Integer>, Tid>::iterator it = no_p_k.begin();
 				for (Tid i=0; i<tid; i++)
					it++;
				no_p_k.erase(it);
				it = no_p_k.end();
				it--;
				it->second = tid;
			}

		}
		inline uint64_t lookup(Integer no_w_id, Integer no_d_id, Integer no_o_id){ return no_p_k.at({no_w_id,no_d_id,no_o_id});}
		inline Integer get_no_o_id(Tid tid){ return newOrder.at(tid).no_o_id;}
		inline void set_no_o_id(Tid tid, Integer no_o_id){ newOrder.at(tid).no_o_id = no_o_id;}
		inline Integer get_no_d_id(Tid tid){ return newOrder.at(tid).no_d_id;}
		inline void set_no_d_id(Tid tid, Integer no_d_id){ newOrder.at(tid).no_d_id = no_d_id;}
		inline Integer get_no_w_id(Tid tid){ return newOrder.at(tid).no_w_id;}
		inline void set_no_w_id(Tid tid, Integer no_w_id){ newOrder.at(tid).no_w_id = no_w_id;}
	};

	//---------------------------------------------------------------------------
	//Order
	//---------------------------------------------------------------------------
	// Order class
	class Order {
	public:
		/// Order Row
		struct Order_Row {
			Integer o_id;
			Integer o_d_id;
			Integer o_w_id;
			Integer o_c_id;
			Timestamp o_entry_d;
			Integer o_carrier_id;
			Numeric<2,0> o_ol_cnt;
			Numeric<1,0> o_all_local;
			//primary key (o_w_id,o_d_id,o_id)
		};
		//Order
		vector<Order_Row> order;
		//Order primary key
		unordered_map<tuple<Integer, Integer, Integer>, Tid> o_p_k;

		Order() {}

		inline void insert(
				Integer o_id,
				Integer o_d_id,
				Integer o_w_id,
				Integer o_c_id,
				Timestamp o_entry_d,
				Integer o_carrier_id,
				Numeric<2,0> o_ol_cnt,
				Numeric<1,0> o_all_local)
		{
			if (o_p_k.find( { o_w_id,o_d_id,o_id }) == o_p_k.end()) {
				o_p_k.insert( { { o_w_id,o_d_id,o_id }, order.size() });
				Order_Row o = { o_id, o_d_id, o_w_id, o_c_id, o_entry_d,
						o_carrier_id, o_ol_cnt, o_all_local, };
				order.push_back(o);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline uint64_t lookup(Integer o_w_id,Integer o_d_id,Integer o_id){ return o_p_k.at({o_w_id,o_d_id,o_id});}
		inline Integer get_o_id(Tid tid){ return order.at(tid).o_id;}
		inline void set_o_id(Tid tid, Integer o_id){ order.at(tid).o_id = o_id;}
		inline Integer get_o_d_id(Tid tid){ return order.at(tid).o_d_id;}
		inline void set_o_d_id(Tid tid, Integer o_d_id){ order.at(tid).o_d_id = o_d_id;}
		inline Integer get_o_w_id(Tid tid){ return order.at(tid).o_w_id;}
		inline void set_o_w_id(Tid tid, Integer o_w_id){ order.at(tid).o_w_id = o_w_id;}
		inline Integer get_o_c_id(Tid tid){ return order.at(tid).o_c_id;}
		inline void set_o_c_id(Tid tid, Integer o_c_id){ order.at(tid).o_c_id = o_c_id;}
		inline Timestamp get_o_entry_d(Tid tid){ return order.at(tid).o_entry_d;}
		inline void set_o_entry_d(Tid tid, Timestamp o_entry_d){ order.at(tid).o_entry_d = o_entry_d;}
		inline Integer get_o_carrier_id(Tid tid){ return order.at(tid).o_carrier_id;}
		inline void set_o_carrier_id(Tid tid, Integer o_carrier_id){ order.at(tid).o_carrier_id = o_carrier_id;}
		inline Numeric<2,0> get_o_ol_cnt(Tid tid){ return order.at(tid).o_ol_cnt;}
		inline void set_o_ol_cnt(Tid tid, Numeric<2,0> o_ol_cnt){ order.at(tid).o_ol_cnt = o_ol_cnt;}
		inline Numeric<1,0> get_o_all_local(Tid tid){ return order.at(tid).o_all_local;}
		inline void set_o_all_local(Tid tid, Numeric<1,0> o_all_local){ order.at(tid).o_all_local = o_all_local;}
	};

	//---------------------------------------------------------------------------
	//OrderLine
	//---------------------------------------------------------------------------
	// OrderLine class
	class OrderLine {
	public:
		/// OrderLine Row
		struct OrderLine_Row {
			Integer ol_o_id;
			Integer ol_d_id;
			Integer ol_w_id;
			Integer ol_number;
			Integer ol_i_id;
			Integer ol_supply_w_id;
			Timestamp ol_delivery_d;
			Numeric<2,0> ol_quantity;
			Numeric<6,2> ol_amount;
			Char<24> ol_dist_info;
			//primary key (ol_w_id,ol_d_id,ol_o_id,ol_number)
		};
		//OrderLine
		vector<OrderLine_Row> orderLine;
		//OrderLine primary key
		unordered_map<tuple<Integer, Integer, Integer, Integer>, Tid> ol_p_k;

		OrderLine() {}

		inline void insert(
				Integer ol_o_id,
				Integer ol_d_id,
				Integer ol_w_id,
				Integer ol_number,
				Integer ol_i_id,
				Integer ol_supply_w_id,
				Timestamp ol_delivery_d,
				Numeric<2,0> ol_quantity,
				Numeric<6,2> ol_amount,
				Char<24> ol_dist_info)
		{
			if (ol_p_k.find( { ol_w_id,ol_d_id,ol_o_id,ol_number }) == ol_p_k.end()) {
				ol_p_k.insert( { { ol_w_id,ol_d_id,ol_o_id,ol_number }, orderLine.size() });
				OrderLine_Row ol = { ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id,
						ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount,
						ol_dist_info, };
				orderLine.push_back(ol);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline uint64_t lookup(Integer ol_w_id, Integer ol_d_id, Integer ol_o_id, Integer ol_number){ return ol_p_k.at({ol_w_id,ol_d_id,ol_o_id,ol_number});}
		inline Integer get_ol_o_id(Tid tid){ return orderLine.at(tid).ol_o_id;}
		inline void set_ol_o_id(Tid tid, Integer ol_o_id){ orderLine.at(tid).ol_o_id = ol_o_id;}
		inline Integer get_ol_d_id(Tid tid){ return orderLine.at(tid).ol_d_id;}
		inline void set_ol_d_id(Tid tid, Integer ol_d_id){ orderLine.at(tid).ol_d_id = ol_d_id;}
		inline Integer get_ol_w_id(Tid tid){ return orderLine.at(tid).ol_w_id;}
		inline void set_ol_w_id(Tid tid, Integer ol_w_id){ orderLine.at(tid).ol_w_id = ol_w_id;}
		inline Integer get_ol_number(Tid tid){ return orderLine.at(tid).ol_number;}
		inline void set_ol_number(Tid tid, Integer ol_number){ orderLine.at(tid).ol_number = ol_number;}
		inline Integer get_ol_i_id(Tid tid){ return orderLine.at(tid).ol_i_id;}
		inline void set_ol_i_id(Tid tid, Integer ol_i_id){ orderLine.at(tid).ol_i_id = ol_i_id;}
		inline Integer get_ol_supply_w_id(Tid tid){ return orderLine.at(tid).ol_supply_w_id;}
		inline void set_ol_supply_w_id(Tid tid, Integer ol_supply_w_id){ orderLine.at(tid).ol_supply_w_id = ol_supply_w_id;}
		inline Timestamp get_ol_delivery_d(Tid tid){ return orderLine.at(tid).ol_delivery_d;}
		inline void set_ol_delivery_d(Tid tid, Timestamp ol_delivery_d){ orderLine.at(tid).ol_delivery_d = ol_delivery_d;}
		inline Numeric<2,0> get_ol_quantity(Tid tid){ return orderLine.at(tid).ol_quantity;}
		inline void set_ol_quantity(Tid tid, Numeric<2,0> ol_quantity){ orderLine.at(tid).ol_quantity = ol_quantity;}
		inline Numeric<6,2> get_ol_amount(Tid tid){ return orderLine.at(tid).ol_amount;}
		inline void set_ol_amount(Tid tid, Numeric<6,2> ol_amount){ orderLine.at(tid).ol_amount = ol_amount;}
		inline Char<24> get_ol_dist_info(Tid tid){ return orderLine.at(tid).ol_dist_info;}
		inline void set_ol_dist_info(Tid tid, Char<24> ol_dist_info){ orderLine.at(tid).ol_dist_info = ol_dist_info;}
	};

	//---------------------------------------------------------------------------
	//Item
	//---------------------------------------------------------------------------
	// Item class
	class Item {
		/// Item Row
		struct Item_Row {
			Integer i_id;
			Integer i_im_id;
			Varchar<24> i_name;
			Numeric<5,2> i_price;
			Varchar<50> i_data;
			//primary key (i_id)
		};
	public:
		//Item
		vector<Item_Row> item;
		//Item primary key
		unordered_map<Integer, Tid> i_p_k;

		Item() {}

		inline void insert(
				Integer i_id,
				Integer i_im_id,
				Varchar<24> i_name,
				Numeric<5,2> i_price,
				Varchar<50> i_data)
		{
			if (i_p_k.find(i_id) == i_p_k.end()) {
				i_p_k.insert( { i_id, item.size() });
				Item_Row i = { i_id, i_im_id, i_name, i_price, i_data, };
				item.push_back(i);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline uint64_t lookup(Integer i_id){ return i_p_k.at(i_id);}
		inline Integer get_i_id(Tid tid){ return item.at(tid).i_id;}
		inline void set_i_id(Tid tid, Integer i_id){ item.at(tid).i_id = i_id;}
		inline Integer get_i_im_id(Tid tid){ return item.at(tid).i_im_id;}
		inline void set_i_im_id(Tid tid, Integer i_im_id){ item.at(tid).i_im_id = i_im_id;}
		inline Varchar<24> get_i_name(Tid tid){ return item.at(tid).i_name;}
		inline void set_w_name(Tid tid, Varchar<24> i_name){ item.at(tid).i_name = i_name;}
		inline Numeric<5,2> get_i_price(Tid tid){ return item.at(tid).i_price;}
		inline void set_i_price(Tid tid, Numeric<5,2> i_price){ item.at(tid).i_price = i_price;}
		inline Varchar<50> get_i_data(Tid tid){ return item.at(tid).i_data;}
		inline void set_i_data(Tid tid, Varchar<50> i_data){ item.at(tid).i_data = i_data;}
	};

	//---------------------------------------------------------------------------
	//Stock
	//---------------------------------------------------------------------------
	// Stock class
	class Stock {
		/// Stock Row
		struct Stock_Row {
			Integer s_i_id;
			Integer s_w_id;
			Numeric<4,0> s_quantity;
			Char<24> s_dist_01;
			Char<24> s_dist_02;
			Char<24> s_dist_03;
			Char<24> s_dist_04;
			Char<24> s_dist_05;
			Char<24> s_dist_06;
			Char<24> s_dist_07;
			Char<24> s_dist_08;
			Char<24> s_dist_09;
			Char<24> s_dist_10;
			Numeric<8,0> s_ytd;
			Numeric<4,0> s_order_cnt;
			Numeric<4,0> s_remote_cnt;
			Varchar<50> s_data;
			//primary key (s_w_id,s_i_id)
		};
	public:
		//Stock
		vector<Stock_Row> stock;
		//Stock primary key
		unordered_map<tuple<Integer, Integer>, Tid> s_p_k;

		Stock() {}

		inline void insert(
				Integer s_i_id,
				Integer s_w_id,
				Numeric<4,0> s_quantity,
				Char<24> s_dist_01,
				Char<24> s_dist_02,
				Char<24> s_dist_03,
				Char<24> s_dist_04,
				Char<24> s_dist_05,
				Char<24> s_dist_06,
				Char<24> s_dist_07,
				Char<24> s_dist_08,
				Char<24> s_dist_09,
				Char<24> s_dist_10,
				Numeric<8,0> s_ytd,
				Numeric<4,0> s_order_cnt,
				Numeric<4,0> s_remote_cnt,
				Varchar<50> s_data)
		{
			if (s_p_k.find( { s_w_id,s_i_id }) == s_p_k.end()) {
				s_p_k.insert( { { s_w_id,s_i_id }, stock.size() });
				Stock_Row s = { s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02,
						s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07,
						s_dist_08, s_dist_09, s_dist_10, s_ytd, s_order_cnt,
						s_remote_cnt, s_data, };
				stock.push_back(s);
			} else {
				cerr << "Primary key must be unique!\n";
			}
		}
		inline uint64_t lookup(Integer s_w_id, Integer s_i_id){ return s_p_k.at({s_w_id,s_i_id});}
		inline Integer get_s_i_id(Tid tid){ return stock.at(tid).s_i_id;}
		inline void set_s_i_id(Tid tid, Integer s_i_id){ stock.at(tid).s_i_id = s_i_id;}
		inline Integer s_w_id(Tid tid){ return stock.at(tid).s_w_id;}
		inline void set_s_w_id(Tid tid, Integer s_w_id){ stock.at(tid).s_w_id = s_w_id;}
		inline Numeric<4,0> get_s_quantity(Tid tid){ return stock.at(tid).s_quantity;}
		inline void set_s_quantity(Tid tid, Numeric<4,0> s_quantity){ stock.at(tid).s_quantity = s_quantity;}
		inline Char<24> get_s_dist_01(Tid tid){ return stock.at(tid).s_dist_01;}
		inline void set_s_dist_01(Tid tid, Char<24> s_dist_01){ stock.at(tid).s_dist_01 = s_dist_01;}
		inline Char<24> get_s_dist_02(Tid tid){ return stock.at(tid).s_dist_02;}
		inline void set_s_dist_02(Tid tid, Char<24> s_dist_02){ stock.at(tid).s_dist_02 = s_dist_02;}
		inline Char<24> get_s_dist_03(Tid tid){ return stock.at(tid).s_dist_03;}
		inline void set_s_dist_03(Tid tid, Char<24> s_dist_03){ stock.at(tid).s_dist_03 = s_dist_03;}
		inline Char<24> get_s_dist_04(Tid tid){ return stock.at(tid).s_dist_04;}
		inline void set_s_dist_04(Tid tid, Char<24> s_dist_04){ stock.at(tid).s_dist_04 = s_dist_04;}
		inline Char<24> get_s_dist_05(Tid tid){ return stock.at(tid).s_dist_05;}
		inline void set_s_dist_05(Tid tid, Char<24> s_dist_05){ stock.at(tid).s_dist_05 = s_dist_05;}
		inline Char<24> get_s_dist_06(Tid tid){ return stock.at(tid).s_dist_06;}
		inline void set_s_dist_06(Tid tid, Char<24> s_dist_06){ stock.at(tid).s_dist_06 = s_dist_06;}
		inline Char<24> get_s_dist_07(Tid tid){ return stock.at(tid).s_dist_07;}
		inline void set_s_dist_07(Tid tid, Char<24> s_dist_07){ stock.at(tid).s_dist_07 = s_dist_07;}
		inline Char<24> get_s_dist_08(Tid tid){ return stock.at(tid).s_dist_08;}
		inline void set_s_dist_08(Tid tid, Char<24> s_dist_08){ stock.at(tid).s_dist_08 = s_dist_08;}
		inline Char<24> get_s_dist_09(Tid tid){ return stock.at(tid).s_dist_09;}
		inline void set_s_dist_09(Tid tid, Char<24> s_dist_09){ stock.at(tid).s_dist_09 = s_dist_09;}
		inline Char<24> get_s_dist_10(Tid tid){ return stock.at(tid).s_dist_10;}
		inline void set_s_dist_10(Tid tid, Char<24> s_dist_10){ stock.at(tid).s_dist_10 = s_dist_10;}
		inline Numeric<8,0> get_s_ytd(Tid tid){ return stock.at(tid).s_ytd;}
		inline void set_s_ytd(Tid tid, Numeric<8,0> s_ytd){ stock.at(tid).s_ytd = s_ytd;}
		inline Numeric<4,0> get_s_order_cnt(Tid tid){ return stock.at(tid).s_order_cnt;}
		inline void set_s_order_cnt(Tid tid, Numeric<4,0> s_order_cnt){ stock.at(tid).s_order_cnt = s_order_cnt;}
		inline Numeric<4,0> get_s_remote_cnt(Tid tid){ return stock.at(tid).s_remote_cnt;}
		inline void set_s_remote_cnt(Tid tid, Numeric<4,0> s_remote_cnt){ stock.at(tid).s_remote_cnt = s_remote_cnt;}
		inline Varchar<50> get_s_data(Tid tid){ return stock.at(tid).s_data;}
		inline void set_s_data(Tid tid, Varchar<50> s_data){ stock.at(tid).s_data = s_data;}
	};

public:

	////Warehouse
	Warehouse warehouse;
	////District
	District district;
	////Customer
	Customer customer;
	////History
	History history;
	////NewOrder
	NewOrder newOrder;
	////Order
	Order order;
	////OrderLine
	OrderLine orderLine;
	////Item
	Item item;
	////Stock
	Stock stock;

	TPCC() {}

	inline void populateDataBase() {
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

	inline void addWarehouses() {
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
					warehouse.insert(w_id, w_name, w_street_1 , w_street_2, w_city,
							w_state, w_zip, w_tax, w_ytd);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addDistricts() {
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
					district.insert(d_id, d_w_id, d_name, d_street_1, d_street_2,
							d_city, d_state, d_zip, d_tax, d_ytd, d_next_o_id);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addCustomers() {
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
					customer.insert( c_id, c_d_id, c_w_id, c_first, c_middle, c_last,
							c_street_1, c_street_2, c_city, c_state, c_zip, c_phone,
							c_since, c_credit, c_credit_lim, c_discount, c_balance,
							c_ytd_paymenr, c_payment_cnt, c_delivery_cnt, c_data);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addHistory() {
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
				history.insert(h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date,
						h_amount, h_data);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addNewOrders() {
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

					newOrder.insert(no_o_id, no_d_id, no_w_id);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addOrders() {
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
					order.insert(o_id, o_d_id, o_w_id, o_c_id, o_entry_d,
							o_carrier_id, o_ol_cnt, o_all_local);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addOrderLines() {
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
					orderLine.insert(ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id,
							ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount,
							ol_dist_info);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addItems() {
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
					item.insert(i_id, i_im_id, i_name, i_price, i_data);
			}
		}
		cout << "Done." << "\n";
	}

	inline void addStock(string filename) {
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
					stock.insert( s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02,
							s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07,
							s_dist_08, s_dist_09, s_dist_10, s_ytd, s_order_cnt,
							s_remote_cnt, s_data);
			}
		}
	}

	inline void addStock() {
		cout << "Adding the Stock...";
		addStock("tpcc_stock.tbl");
		addStock("tpcc_stock_2.tbl");
		cout << "Done." << "\n";
	}

	double query(){
		unordered_multimap<tuple<Integer,Integer,Integer>, TPCC::Customer::Customer_Row> IndexCustomer;
		for (TPCC::Customer::Customer_Row c : customer.customer){
			if (c.c_last.value[0]=='B'){
				IndexCustomer.insert({{c.c_w_id, c.c_d_id, c.c_id}, c});
			}
		}
		std::vector<tuple<TPCC::Order::Order_Row,TPCC::Customer::Customer_Row>> result;
		for (TPCC::Order::Order_Row o : order.order){
			if (IndexCustomer.count({o.o_w_id, o.o_d_id, o.o_c_id})){
				auto range = IndexCustomer.equal_range({o.o_w_id, o.o_d_id, o.o_c_id});
			    for (auto it = range.first; it != range.second; ++it) {
			    	result.push_back({o,it->second});};
			    }
			}

		unordered_multimap<tuple<Integer,Integer,Integer>, tuple<TPCC::Order::Order_Row ,TPCC::Customer::Customer_Row>> IndexOrderCustomer;
		for (tuple<TPCC::Order::Order_Row,TPCC::Customer::Customer_Row> o_c : result){
				IndexOrderCustomer.insert({{get<0>(o_c).o_w_id, get<0>(o_c).o_d_id, get<0>(o_c).o_id}, o_c});
		}
		std::vector<tuple<TPCC::OrderLine::OrderLine_Row,TPCC::Order::Order_Row,TPCC::Customer::Customer_Row>> endResult;
		for (TPCC::OrderLine::OrderLine_Row ol : orderLine.orderLine){
			if (IndexOrderCustomer.count({ol.ol_w_id, ol.ol_d_id, ol.ol_o_id})){
				auto range = IndexOrderCustomer.equal_range({ol.ol_w_id, ol.ol_d_id, ol.ol_o_id});
			    for (auto it = range.first; it != range.second; ++it) {
			    	endResult.push_back({ol, get<0>(it->second), get<1>(it->second)});};
			    }
			}
		double sum =0.0;
		for (tuple<TPCC::OrderLine::OrderLine_Row,TPCC::Order::Order_Row,TPCC::Customer::Customer_Row> r : endResult){
			sum = sum + get<0>(r).ol_quantity.value * 0.01 * get<0>(r).ol_amount.value - get<2>(r).c_balance.value * 0.01 * get<1>(r).o_ol_cnt.value;
		}
		return sum;


	}


};

#endif
