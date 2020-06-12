#include "stl_hash_fun.h"
#include "stl_hashtable.h"
#include <functional>
#include "stl_function.h"


int main() {

	mySTL::hashtable<int, int, std::hash<int>, mySTL::identity<int>, mySTL::equal_to<int>>
		ht(50, std::hash<int>(), mySTL::equal_to<int>());

	ht.insert_equal(1);
	ht.insert_unique(1);
	ht.find_or_insert(1);
	ht.find_or_insert(2);
	std::cout << ht.size() << std::endl;

	ht.erase(ht.begin(), ht.end());

	return 0;
}
