#include "BoolVector.h"
#include "Enum.h"


const int BYTE = 8;


BoolVector::BoolVector() {
	lenght_ = 0; 
	vector_ = new unsigned char[memory_ = 1];
	vector_[0] = 0;
}


BoolVector::BoolVector(int lenght, bool value) {
	if (lenght < 0)
		throw Invalid_size;
	lenght_ = lenght;
	vector_ = new unsigned char[memory_ = (lenght_ + BYTE - 1) / BYTE];
	if (value) {
		for (int i = 0; i < memory_; ++i)
			vector_[i] = 255;
		if (lenght_ % BYTE) {
			unsigned char mask = 128;
			for (int i = 0; i < BYTE - lenght_ % BYTE; ++i)
				vector_[0] &= ~mask >> i;
		}
	}
	else
		for (int i = 0; i < memory_; ++i)
			vector_[i] = 0;
}


BoolVector::BoolVector(const char* string) : BoolVector(strlen(string), false) {
	for (int i = 0; i < lenght_; ++i)
		if (string[i] != '0')
			set1(i);
}


BoolVector::BoolVector(const BoolVector& copyVector) {
	lenght_ = copyVector.lenght_;
	vector_ = new unsigned char[memory_ = copyVector.memory_];
	memcpy(vector_, copyVector.vector_, memory_);
}


BoolVector::~BoolVector() {
	delete[] vector_;
}


bool BoolVector::bit(int index)const {
	if (index < 0 || index >= lenght_)
		throw Invalid_index;
	unsigned char mask = 128;
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte)
		mask >>= (index - lenght_ % BYTE) % BYTE;
	else
		mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + index;
	return vector_[byte] & mask;
}


int BoolVector::weight()const {
	int weight = 0;
	int i = 0;
	unsigned char mask;
	for (; i < lenght_ % BYTE; ++i) {
		mask = 128;
		mask >>= BYTE - lenght_ % BYTE + i;
		if (vector_[0] & mask)
			++weight;
	}
	if (i)
		i = 1;
	for (; i < memory_; ++i) {
		mask = 128;
		for (int j = 0; j < BYTE; ++j, mask >>= 1)
			if (vector_[i] & mask)
				++weight;
	}
	return weight;
}


void BoolVector::set0(int index) {
	if (index < 0 || index >= lenght_)
		throw Invalid_index;
	unsigned char mask = 128;
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte)
		mask >>= (index - lenght_ % BYTE) % BYTE;
	else
		mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + index;
	vector_[byte] &= ~mask;
}


void BoolVector::set1(int index) {
	if (index < 0 || index >= lenght_)
		throw Invalid_index;
	unsigned char mask = 128;
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte)
		mask >>= (index - lenght_ % BYTE) % BYTE;
	else
		mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + index;
	vector_[byte] |= mask;
}


void BoolVector::fullSet0() {
	for (int i = 0; i < memory_; ++i)
		vector_[i] = 0;
}


void BoolVector::fullSet1() {
	unsigned char mask = 128;
	mask >>= BYTE - abs(lenght_ - (memory_ - 1) * BYTE);
	int i = 0;
	for (; i < lenght_ % BYTE; ++i)
		vector_[0] |= mask >> i;
	if (i)
		i = 1;
	for (; i < memory_; ++i)
		vector_[i] = 255;
}


void BoolVector::set0(int index, int value) {
	if (index < 0 || index >= lenght_)
		throw Invalid_index;
	if (value < 0 || value > lenght_)
		throw Invalid_size;
	if (!value)
		return;
	if (lenght_ - index < value)
		value = lenght_ - index;
	if (value > 1) {
		unsigned char mask;
		for (int i = index; i < value + index; ++i) {
			mask = 128;
			int byte = memory_ - 1 - (lenght_ - i - 1) / BYTE;
			if (byte)
				mask >>= (i - lenght_ % BYTE) % BYTE;
			else
				mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + i;
			vector_[byte] &= ~mask;
		}
	}
	else
		set0(index);
}


void BoolVector::set1(int index, int value) {
	if (index < 0 || index >= lenght_)
		throw Invalid_index;
	if (value < 0 || value > lenght_)
		throw Invalid_size;
	if (value)
		return;
	if (lenght_ - index < value)
		value = lenght_ - index;
	if (value > 1) {
		unsigned char mask;
		for (int i = index; i < value + index; ++i) {
			mask = 128;
			int byte = memory_ - 1 - (lenght_ - i - 1) / BYTE;
			if (byte)
				mask >>= (i - lenght_ % BYTE) % BYTE;
			else
				mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + i;
			vector_[byte] |= mask;
		}
	}
	else
		set1(index);
}


void BoolVector::fullInversion() {
	unsigned char mask;
	int i = 0;
	for (; i < lenght_ % BYTE; ++i) {
		mask = 128;
		mask >>= BYTE - lenght_ % BYTE + i;
		vector_[0] ^= mask;
	}
	if (i)
		i = 1;
	for (; i < memory_; ++i) {
		mask = 128;
		for (int j = 0; j < BYTE; ++j, mask >>= 1)
			vector_[i] ^= mask;
	}
}


void BoolVector::componentInversion(int index) {
	if (index < 0 || index >= lenght_)
		throw Invalid_index;
	unsigned char mask = 128;
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte)
		mask >>= (index - lenght_ % BYTE) % BYTE;
	else
		mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + index;
	vector_[byte] ^= mask;
}


BoolVector BoolVector::operator~()const {
	BoolVector vector(*this);
	vector.fullInversion();
	return vector;
}


BoolVector& BoolVector::operator=(const BoolVector& vector) {
	if (this == &vector)
		return *this;
	delete[] vector_;
	lenght_ = vector.lenght_;
	vector_ = new unsigned char[memory_ = vector.memory_];
	memcpy(vector_, vector.vector_, memory_);
	return *this;
}


BoolVector& BoolVector::operator>>=(int value) {
	if (value <= 0)
		return *this;
	if (value >= lenght_)
		for (int i = 0; i < memory_; ++i)
			vector_[i] = 0;
	unsigned char mask = 128;
	for (int i = 0; i < value; ++i) {
		for (int j = memory_ - 1; j > 0; --j) {
			vector_[j] >>= 1;
			if (vector_[j - 1] & 1)
				vector_[j] |= mask;
		}
		vector_[0] >>= 1;
	}
	return *this;
}


BoolVector BoolVector::operator>>(int value)const {
	return BoolVector(*this) >>= value;
}


BoolVector& BoolVector::operator<<=(int value) {
	if (value <= 0)
		return *this;
	if (value >= lenght_)
		for (int i = 0; i < memory_; ++i)
			vector_[i] = 0;
	unsigned char mask = 128;
	for (int i = 0; i < value; ++i) {
		for (int j = 0; j < memory_ - 1; ++j) {
			vector_[j] <<= 1;
			if (vector_[j + 1] & mask)
				vector_[j] |= 1;
		}
		vector_[memory_ - 1] <<= 1;
	}
	if (lenght_ % BYTE)
		for (int i = 0; i < BYTE - lenght_ % BYTE; ++i)
			vector_[0] &= ~mask >> i;
	return *this;
}


BoolVector BoolVector::operator<<(int value)const {
	return BoolVector(*this) <<= value;
}


BoolVector& BoolVector::operator|=(const BoolVector& vector) {
	int min = memory_;
	if (memory_ > vector.memory_)
		min = vector.memory_;
	if (lenght_ < vector.lenght_)
		lenght_ = vector.lenght_;
	for (int i = 0; i < min; ++i)
		vector_[memory_ - i - 1] |= vector.vector_[vector.memory_ - i - 1];
	return *this;
}


BoolVector BoolVector::operator|(const BoolVector& vector)const {
	return BoolVector(*this) |= vector;
}


BoolVector& BoolVector::operator&=(const BoolVector& vector) {
	int min = memory_;
	if (memory_ > vector.memory_)
		min = vector.memory_;
	if (lenght_ < vector.lenght_)
		lenght_ = vector.lenght_;
	for (int i = 0; i < min; ++i)
		vector_[memory_ - i - 1] &= vector.vector_[vector.memory_ - i - 1];
	return *this;
}


BoolVector BoolVector::operator&(const BoolVector& vector)const {
	return BoolVector(*this) &= vector;
}


BoolVector& BoolVector::operator^=(const BoolVector& vector) {
	int min = memory_;
	if (memory_ > vector.memory_)
		min = vector.memory_;
	if (lenght_ < vector.lenght_)
		lenght_ = vector.lenght_;
	for (int i = 0; i < min; ++i)
		vector_[memory_ - i - 1] ^= vector.vector_[vector.memory_ - i - 1];
	return *this;
}


BoolVector BoolVector::operator^(const BoolVector& vector)const {
	return BoolVector(*this) ^= vector;
}


BoolVector::operator int() {
	int digit = 0;
	for (int i = 0; i < lenght_; ++i) {
		digit *= 2;
		if (bit(i))
			++digit;
	}
	return digit;
}


BoolVector::operator char* () {
	char* text = new char[memory_];
	for (int i = 0; i < memory_; ++i)
		text[i] = byte(i);
	return text;
}


std::istream& operator>>(std::istream& in, const BoolVector& vector) {
	unsigned char c;
	int i = 0;
	for (; i < vector.lenght_ % BYTE; ++i) {
		in >> c;
		if (c != '0')
			c = '1';
		vector.vector_[0] <<= 1;
		vector.vector_[0] |= c - '0';
	}
	if (i)
		i = 1;
	for (; i < vector.memory_; ++i)
		for (int j = 0; j < BYTE; ++j) {
			in >> c;
			if (c != '0')
				c = '1';
			vector.vector_[i] <<= 1;
			vector.vector_[i] |= c - '0';
		}
	return in;
}


std::ostream& operator<<(std::ostream& output, const BoolVector& vector) {
	unsigned char mask;
	for (int i = 0; i < vector.memory(); ++i) {
		mask = 128;
		for (int j = 0; j < BYTE; ++j, mask >>= 1)
			if (vector.byte(i) & mask)
				output << 1;
			else
				output << 0;
		output << ' ';
	}
	return output;
}