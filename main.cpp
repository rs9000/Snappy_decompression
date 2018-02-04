#include "bitop.h"
#include <bitset>

using namespace std;

int main(int argc, char *argv[]) {

	if (argc != 3) {
		cout << "Use: ./snappy_decomp <input_file> <output_file> \n";
		return 1;
	}

	bitop bitoperator;
	bitoperator.read(string(argv[1]));
	
	vector<uint8_t> file = bitoperator.snappy_decode();
	bitoperator.write(file, string(argv[2]));

	return 0;

}