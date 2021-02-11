#pragma once
#include <iostream>


class BoolVector {
private:
	int lenght_;
	int memory_;
	unsigned char* vector_;
public:
	const int BYTE = 8;
	BoolVector() : lenght_(0), memory_(1), vector_(new unsigned char[memory_]) { vector_[0] = 0; }
	BoolVector(int lenght, bool value);
	BoolVector(const char* string);
	BoolVector(const BoolVector& copyVector);
	~BoolVector();
	int lenght()const { return lenght_; };
	int memory()const { return memory_; }
	char byte(int index)const { return vector_[index]; }
	int weight()const;
	bool bit(int index)const;
	friend std::istream& operator>>(std::istream& input, const BoolVector& vector);
	void set0(int index);
	void set1(int index);
	void fullSet0();
	void fullSet1();
	void set0(int index, int value);
	void set1(int index, int value);
	void fullInversion();
	void componentInversion(int index);
	BoolVector operator~()const;
	BoolVector& operator=(const BoolVector& vector);
};


std::ostream& operator<<(std::ostream& output, const BoolVector& vector) {
	unsigned char mask;
	for (int i = 0; i < vector.memory(); ++i) {
		mask = 128; // 10000000
		for (int j = 0; j < vector.BYTE; ++j, mask >>= 1)
			if (vector.byte(i) & mask)
				output << 1;
			else
				output << 0;
		output << " ";
	}
	return output;
}