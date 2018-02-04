#include "bitop.h"

#include <bitset>
#include <algorithm>
#include <iostream>
#include <math.h>    

using std::vector;
using std::string;


bitop::bitop()
{
}


bitop::~bitop()
{
}


//Leggo n bytes 
string bitop::read_data(unsigned int &index, unsigned int n_bytes, bool littlend) {

	string read_s;

	for (size_t i = 0; i < n_bytes; i++) {
		if (littlend) {
			read_s = std::bitset<8>(data[index + i]).to_string() + read_s;
		}
		else {
			read_s += std::bitset<8>(data[index + i]).to_string();
		}
		
	}

	index += n_bytes;

	return read_s;

}


//Leggo un byte (8 bit) alla volta e lo salvo come elemento di un vettore
void bitop::read(string path) {


	std::ifstream is(path, std::ios::binary);
	if (!is) {
		std::cout << "Errore lettura file \n";
	}

	is.unsetf(std::ifstream::skipws);

	while (is.good()) {
		data.push_back(is.get());
	}
	

	is.close();

}

void bitop::write(string path) {

	std::ofstream fp;
	fp.open(path, std::ios::out | std::ios::binary);
	std::copy(data.begin(), data.end(), std::ostream_iterator<uint8_t>(fp));

}

void bitop::write(vector<uint8_t> &mydata, string path) {

	std::ofstream fp;
	fp.open(path, std::ios::out | std::ios::binary);
	std::copy(mydata.begin(), mydata.end(), std::ostream_iterator<uint8_t>(fp));

}


vector<uint8_t> bitop::snappy_decode() {

	vector<uint8_t>decode;
	unsigned int index = 0;
	string str1;

	//salta preamble
	do {
		str1 = read_data(index, 1, false);
	} while (str1[0] != '0');
	

	while(index < data.size() -1 ){

		str1 = read_data(index, 1, false);
		string tag_b = str1.substr(6, 2);

		//Literals
		if (tag_b == "00") {
			string len_s = str1.substr(0, 6);
			unsigned int len = std::bitset<6>(len_s).to_ulong();

			if (len >= 60 && len <= 63) {
				len_s = read_data(index, len-59, true);
				len = std::bitset<32>(len_s).to_ulong();
			}

			for (size_t i = 0; i != len+1; ++i) {
				decode.push_back(data[index]);
				index += 1;
			}

		}
		//Copy with 1b-offset
		else if (tag_b == "01") {
			string len_s = str1.substr(3, 3);
			unsigned int len = std::bitset<3>(len_s).to_ulong() +4;
			string offset_s = str1.substr(0, 3) + read_data(index,1,false);
			unsigned int offset = std::bitset<11>(offset_s).to_ulong();

			//decode
			for (size_t i = 0; i != len; ++i) {
				decode.push_back(decode[decode.size() - offset]);
			}

		}
		//Copy with 2b-offset
		else if (tag_b == "10") {
			string len_s = str1.substr(0, 6);
			unsigned int len = std::bitset<6>(len_s).to_ulong() + 1;
			string offset_s = read_data(index, 2, true);
			unsigned int offset = std::bitset<16>(offset_s).to_ulong();
	

			//decode
			for (size_t i = 0; i != len; ++i) {
				decode.push_back(decode[decode.size() - offset]);
			}

		}
		//Copy with 4b-offset
		else if (tag_b == "11") {
			string len_s = str1.substr(0, 6);
			unsigned int len = std::bitset<6>(len_s).to_ulong() + 1;
			string offset_s = read_data(index, 4, true);
			unsigned int offset = std::bitset<32>(offset_s).to_ulong();
		

			//decode
			for (size_t i = 0; i != len; ++i) {
				decode.push_back(decode[decode.size() - offset]);
			}

		}
	}
	
	return decode;
}