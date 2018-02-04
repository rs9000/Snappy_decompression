#pragma once

#include <string>
#include <vector>
#include <iterator> 
#include <iostream>
#include <fstream>



class bitop
{
	std::vector<uint8_t>data;
	std::string stream;

public:
	bitop();
	~bitop();
	void read(std::string path);
	void write(std::string path);
	void write(std::vector<uint8_t> &mydata, std::string path);
	std::string read_data(unsigned int &index, unsigned int n_bytes, bool littlend);
	std::vector<uint8_t> snappy_decode();

};