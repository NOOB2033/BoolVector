#include "BoolVector.h"


BoolVector::BoolVector(int lenght, bool value) {
	lenght_ = lenght;
	vector_ = new unsigned char[memory_ = (lenght_ + BYTE - 1) / BYTE];
	if (value) {
		for (int i = 0; i < memory_; ++i)
			vector_[i] = 255; // 11111111
		if (lenght_ % BYTE) {
			unsigned char mask = 128; // 10000000
			for (int i = 0; i < BYTE - lenght_ % BYTE; ++i) // обнуление первых бит в нулевом байте
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


int BoolVector::weight()const {
	int weight = 0;
	int i = 0;
	unsigned char mask;
	for (; i < lenght_ % BYTE; ++i) {
		mask = 128; // 10000000
		mask >>= BYTE - lenght_ % BYTE + i;
		if (vector_[0] & mask)
			++weight;
	}
	if (i)
		i = 1;
	for (; i < memory_; ++i) {
		mask = 128; // 10000000
		for (int j = 0; j < BYTE; ++j, mask >>= 1)
			if (vector_[i] & mask)
				++weight;
	}
	return weight;
}

bool BoolVector::bit(int index)const {
	unsigned char mask = 128; // 10000000
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte) // отдельно для нулевого байта, если он не полный
		mask >>= (index - lenght_ % BYTE) % BYTE;
	else
		mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + index;
	return vector_[byte] & mask;
}


std::istream& operator>>(std::istream& in, const BoolVector& vector) {
	unsigned char c;
	int i = 0;
	for (; i < vector.lenght_ % vector.BYTE; ++i) { // Для нулевого байта, если он не полный(используются не все 8 бит)
		in >> c;
		if (c != '0')
			c = '1';
		vector.vector_[0] <<= 1;
		vector.vector_[0] |= c - '0';
	}
	if (i) // Если изменился i, то мы обработали нулевой байт => идем к следующим
		i = 1;
	for (; i < vector.memory_; ++i)
		for (int j = 0; j < vector.BYTE; ++j) {
			in >> c;
			if (c != '0')
				c = '1';
			vector.vector_[i] <<= 1;
			vector.vector_[i] |= c - '0';
		}
	return in;
}


void BoolVector::set0(int index) {
	unsigned char mask = 128; // 10000000
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte) // отдельно для нулевого байта, если он не полный
		mask >>= (index - lenght_ % BYTE) % BYTE;
	else
		mask >>= BYTE - (lenght_ - (memory_ - 1) * BYTE) + index;
	vector_[byte] &= ~mask;
}


void BoolVector::set1(int index) {
	unsigned char mask = 128; // 10000000
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte) // отдельно для нулевого байта, если он не полный
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
	unsigned char mask = 128; // 10000000
	mask >>= BYTE - abs(lenght_ - (memory_ - 1) * BYTE);
	int i = 0;
	for (; i < lenght_ % BYTE; ++i)
		vector_[0] |= mask >> i;
	if (i)
		i = 1;
	for (; i < memory_; ++i)
		vector_[i] = 255; // 11111111
}


void BoolVector::set0(int index, int value) {
	if (value > 1) {
		unsigned char mask;
		for (int i = index; i < value + index; ++i) {
			mask = 128; // 10000000
			int byte = memory_ - 1 - (lenght_ - i - 1) / BYTE;
			if (byte) // отдельно для нулевого байта, если он не полный
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
	if (value > 1) {
		unsigned char mask;
		for (int i = index; i < value + index; ++i) {
			mask = 128; // 10000000
			int byte = memory_ - 1 - (lenght_ - i - 1) / BYTE;
			if (byte) // отдельно для нулевого байта, если он не полный
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
		mask = 128; // 10000000
		mask >>= BYTE - lenght_ % BYTE + i;
		vector_[0] ^= mask;
	}
	if (i)
		i = 1;
	for (; i < memory_; ++i) {
		mask = 128; // 10000000
		for (int j = 0; j < BYTE; ++j, mask >>= 1)
			vector_[i] ^= mask;
	}
}


void BoolVector::componentInversion(int index) {
	unsigned char mask = 128; // 10000000
	int byte = memory_ - 1 - (lenght_ - index - 1) / BYTE;
	if (byte) // отдельно для нулевого байта, если он не полный
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