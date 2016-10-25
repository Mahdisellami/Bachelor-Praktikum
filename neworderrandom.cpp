#include <cstdint>
#include "Types.hpp"
#include "Schema.hpp"
#include <vector>
#include <unordered_map>
using namespace std;

const int32_t warehouses=5;
//Warehouses
vector<Warehouse> warehouses_2;
// primary key Warehouses
unordered_map<Integer, Tid> w_p_k;

int32_t urand(int32_t min,int32_t max) {
   return (random()%(max-min+1))+min;
}

int32_t urandexcept(int32_t min,int32_t max,int32_t v) {
   if (max<=min)
      return min;
   int32_t r=(random()%(max-min))+min;
   if (r>=v)
      return r+1; else
      return r;
}

int32_t nurand(int32_t A,int32_t x,int32_t y) {
   return ((((random()%A)|(random()%(y-x+1)+x))+42)%(y-x+1))+x;
}

void newOrder(int32_t w_id, int32_t d_id, int32_t c_id, int32_t ol_cnt,int32_t supware[15], int32_t itemid[15], int32_t qty[15], Timestamp now) {

}

void newOrderRandom() {
   Timestamp now(0);
   int32_t w_id=urand(1,warehouses);
   int32_t d_id=urand(1,10);
   int32_t c_id=nurand(1023,1,3000);
   int32_t ol_cnt=urand(5,15);

   int32_t supware[15];
   int32_t itemid[15];
   int32_t qty[15];
   for (int32_t i=0; i<ol_cnt; i++) {
      if (urand(1,100)>1)
         supware[i]=w_id; else
         supware[i]=urandexcept(1,warehouses,w_id);
      itemid[i]=nurand(8191,1,100000);
      qty[i]=urand(1,10);
   }

   newOrder(w_id,d_id,c_id,ol_cnt,supware,itemid,qty,now);
}

int main(){
}
