#pragma once
#include <iostream>


class BoolVector {
private:
	int lenght_;
	int memory_;
	unsigned char* vector_;
public:
	explicit BoolVector();
	explicit BoolVector(int lenght, bool value);
	explicit BoolVector(const char* string);
	BoolVector(const BoolVector& copyVector);
	virtual ~BoolVector();

	int lenght()const { return lenght_; };
	int memory()const { return memory_; }
	char byte(int index)const { return vector_[index]; }
	bool bit(int index)const;
	int weight()const;

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
	BoolVector& operator>>=(int value);
	BoolVector operator>>(int value)const;
	BoolVector& operator<<=(int value);
	BoolVector operator<<(int value)const;
	BoolVector& operator|=(const BoolVector& vector);
	BoolVector operator|(const BoolVector& vector)const;
	BoolVector& operator&=(const BoolVector& vector);
	BoolVector operator&(const BoolVector& vector)const;
	BoolVector& operator^=(const BoolVector& vector);
	BoolVector operator^(const BoolVector& vector)const;

	explicit operator int();
	explicit operator char* ();

	friend std::istream& operator>>(std::istream& input, const BoolVector& vector);
};

std::ostream& operator<<(std::ostream& output, const BoolVector& vector);