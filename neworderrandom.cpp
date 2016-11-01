#include <cstdint>
#include "Types.hpp"
#include "Schema.hpp"
#include "Warehouse.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <tuple>
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
	tpcc.populateDataBase();
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
