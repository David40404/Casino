#pragma once

template <typename T>
class Array {
	T* data;
	unsigned long long len;
public:
	Array(unsigned long long len = 0) : len(len), data(new T[len]) {
	}
	~Array() {
		//delete[] data;
	}

	void push_back(const T& value) {
		T* new_data = new T[len + 1];

		for (unsigned long long i = 0; i < len; i++)
			new_data[i] = data[i];

		new_data[len] = value;

		delete[] data;

		data = new_data;
		++len;
	}
	void push_begin(const T& value) {
		T* new_data = new T[len + 1];

		for (unsigned long long i = 0; i < len; i++)
			new_data[i + 1] = data[i];

		new_data[0] = value;

		delete[] data;

		data = new_data;
		++len;
	}

	void pop_back() {
		T* new_data = new T[len - 1];

		for (unsigned long long i = 0; i < len - 1; i++)
			new_data[i] = data[i];

		delete[] data;

		data = new_data;
		--len;
	}
	void pop_begin() {
		T* new_data = new T[len - 1];

		for (unsigned long long i = 0; i < len - 1; i++)
			new_data[i] = data[i + 1];

		delete[] data;

		data = new_data;
		--len;
	}

	T& add_back() {
		T* new_data = new T[len + 1];

		for (unsigned long long i = 0; i < len; i++)
			new_data[i] = data[i];

		delete[] data;

		data = new_data;
		++len;

		return data[len - 1];
	}
	T& add_begin() {
		T* new_data = new T[len + 1];

		for (unsigned long long i = 0; i < len; i++)
			new_data[i + 1] = data[i];

		delete[] data;

		data = new_data;
		++len;

		return data[0];
	}

	T& back() {
		return data[len - 1];
	}
	T& begin() {
		return data[0];
	}

	void swap(unsigned long long index1, unsigned long long index2) {
		T tmp = data[index1];
		data[index1] = data[index2];
		data[index2] = tmp;
	}

	void clear() {
		len = 0;
		delete[] data;
		data = new T[len];
	}

	unsigned long long size() {
		return len;
	}

	vector <T> get_vector() {
		vector <T> vec(len);

		for (unsigned long long i = 0; i < len; ++i)
			vec[i] = data[i];

		return vec;
	}

	T* get_data() {
		return data;
	}



	T& operator[](const unsigned long long index) {
		return data[(len + (index % len)) % len];
	}

	friend ostream& operator<<(ostream& out, const Array& arr) {
		for (int i = 0; i < arr.len; i++)
			out << arr.data[i] << " ";
		return out;
	}

};