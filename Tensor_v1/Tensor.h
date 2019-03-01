#ifndef __TENSOR_CLASS__
#define __TENSOR_CLASS__

#include "defs.h"

/*

// Class for Tensor

This class is for expressing general tensor in c/c++ program

This tensor has the following functions : 

1. expressing tensor with rank and dimension.
2. class contains the information of rank, dimension, number of element, and each element's type.
3. It also has the basic operations for >>, <<, !=, ==, >, <, >=, <= ( = is still possible but deprecated )
4. We can approach to each elements of tensor by bridge functions or [] operation
5. also provide some basic mathmatical operation for only number based tensor +(add),-(sub),*(mul) (scalar operation is automatically decided)
- add, adde
- sub, sube
- mul, mule
- divide ( only for elementwise )
=> boosted by cpu/gpu(later version)

6. provide portable functions : argmax, argmin, max, min
7. provide search and sorting algorithm
8. provide print(), info() function with any tensor and plot() function for the tensor under rank 3.
9. provide reshape() and transpose().
10. This tensor can only be defined by Data class ( managed by the Data class )
11. All the number is regarded as long long type( precision is preserved )

*/

/* Typedef for tensor basic class */
typedef long long int INTEGER;
typedef char CHAR;
typedef wchar_t WCHAR;
typedef long double REAL;
typedef wstring WSTRING;
typedef string STRING;

class COMPLEX {
public:
	long double real;
	long double imag;

	COMPLEX() : real(0.0), imag(0.0) {};

	COMPLEX(REAL real, REAL imag) : real(real), imag(imag) {};

	COMPLEX(REAL real) : real(real), imag(0.0) {};
	COMPLEX(INTEGER real) : real(real), imag(0.0) {};

	COMPLEX(const COMPLEX & in) {
		this->real = in.real;
		this->imag = in.imag;
	}

	friend ostream & operator << (ostream &out, const COMPLEX &c);
	friend istream & operator >> (istream &in, COMPLEX &c);

	/* operators */
	COMPLEX & operator=(const COMPLEX & in);
	COMPLEX & operator=(const REAL in);

	COMPLEX & operator+(const COMPLEX & in);
	COMPLEX & operator+(const REAL in);
	COMPLEX & operator-(const COMPLEX & in);
	COMPLEX & operator-(const REAL in);
	COMPLEX & operator*(const COMPLEX & in);
	COMPLEX & operator*(const REAL in);
	COMPLEX & operator/(const COMPLEX & in);
	COMPLEX & operator/(const REAL in);

	COMPLEX & operator+=(const COMPLEX & in);
	COMPLEX & operator+=(const REAL in);
	COMPLEX & operator-=(const COMPLEX & in);
	COMPLEX & operator-=(const REAL in);
	COMPLEX & operator*=(const COMPLEX & in);
	COMPLEX & operator*=(const REAL in);
	COMPLEX & operator/=(const COMPLEX & in);
	COMPLEX & operator/=(const REAL in);

	COMPLEX & operator^(unsigned int num);
	COMPLEX & exp(const COMPLEX & in);
	long double abs(); // magnitude
	long double phase(); // phase

	bool operator==(const COMPLEX & in);
	bool operator!=(const COMPLEX & in);
	bool operator==(const REAL in);
	bool operator!=(const REAL in);
};

class VECTOR {
public:
	vector<long double> value;

	VECTOR(){
	};

	VECTOR(const VECTOR & in) {
		this->value = in.value;
	}
	virtual ~VECTOR();

	void append(long double in) {
		value.push_back(in);
	}

	friend ostream & operator << (ostream &out, const VECTOR &c);
	friend istream & operator >> (istream &in, VECTOR &c);

	/* operators */
	VECTOR & operator<<(long double in);
	VECTOR & operator=(const VECTOR & in);
};

/*** Enum Classes ***/
enum class Type : unsigned int {
	CHAR,
	WCHAR,
	INTEGER, // Integer number, long long int
	REAL, // real number, long double
	COMPLEX, // Complex number, complex
	VECTOR, // mathmatical VECTOR
	STRING,
	WSTRING,
	UNKNOWN
};

class Tensor_Basic {
private:

	/****** Private Variable ******/


	/* Basic Information */
	unsigned int rank;
	unsigned int * dim;
	unsigned long int size;

	/* Storage */
	vector<wstring> * element;
	Type * type;
	mt19937 gen_1;
	int precision;

	/* Refered Information */
	unsigned long int ref_pos_start;
	unsigned long int ref_pos_end;
	unsigned int ref_rank;

	/* Log data for error report */
	vector<string> logs; // max to 25 history

	/****** Private Functions ******/

	void Alloc_Tensor(const vector<unsigned int> & tensor_dim); // Register Tensor

	void Dealloc_Tensor(); // Remove Tensor

	void init_param(); // Initialize the Refered Information

	template <typename T>
	Type type_encoder(const T & value); // Declare the type of inputs
	wstring Type2str(Type in);

	template <typename T>
	wstring to_string_with_precision(const T value); // Convert to string with certain precision

	template <typename T>
	T gen_randn(T min, T max); // get the random number for rand initialize
	double gaussianRandom(double average, double stdev);

	/* Copy the elementary information */
	vector<unsigned int> copy_info() const;
	/* Copy the data */
	void copy_element(vector<wstring> * tensor, unsigned long int start_pos = 0) const;
	void copy_type(Type * type, unsigned long int start_pos = 0) const;

	void insert(const Tensor_Basic & Object); // Insert the Object's data to this class

	/* Error Manage */
	void Exception_Report(string fname, int line, bool error = true) const;
	void Collect_log(string fname);

	/* Converter */
	void Convert_Value(INTEGER & in, unsigned long int idx);
	void Convert_Value(CHAR & in, unsigned long int idx);
	void Convert_Value(WCHAR & in, unsigned long int idx);
	void Convert_Value(REAL & in, unsigned long int idx);
	void Convert_Value(STRING & in, unsigned long int idx);
	void Convert_Value(WSTRING & in, unsigned long int idx);
	void Convert_Value(COMPLEX & in, unsigned long int idx);
	void Convert_Value(VECTOR & in, unsigned long int idx);

protected:
	wstring name;
public:
	/* Constructor and Destructor */
	Tensor_Basic(wstring name);
	Tensor_Basic(const Tensor_Basic & new_tensor);
	virtual ~Tensor_Basic();

	/* I/O Functions of the basic information */
	const unsigned int get_rank();
	const unsigned int get_size();
	const Type get_type();
	wstring get_name();
	vector<unsigned int> shape();
	void set_precision(const int precision);
	void change_name(wstring new_name);

	/* Value check function */
	void info();
	void print(short enter_posx = 90, short max_row = 8, short max_col = 13, short word_space = 14); // show max 40 rows and 16 elements each
	friend ostream & operator << (ostream &out, Tensor_Basic & tensor);
	void plot(); //

	/* Approach to element directly */
	wstring Tensor2Str();
	bool Str2Tensor(wstring str_tensor);

	/* Initializer */
	void zeros(unsigned int args, ...);
	void ones(unsigned int args, ...);
	void randu(unsigned int args, ...); // uniform distribution
	void randn(unsigned int args, ...); // normal distribution
	void initu(vector<unsigned int> dims, double min = 0.0, double max = 1.0); // uniform distribution
	void initg(vector<unsigned int> dims, double average = 0.0, double stdev = 1.0); // gaussian distribution

	/* idx access */
	Tensor_Basic& operator[](unsigned int idx);

	/* Operators and Functions */
	template <typename T>
	Tensor_Basic& operator>> (T & out); // include all the cases
	Tensor_Basic& operator>> (Tensor_Basic & out);
	template <typename T>
	void get_element_v1(T & out, unsigned long int idx);
	template <typename T>
	T get_element_v2(unsigned long int idx);
	vector<wstring> get_raw(unsigned long int idx);
	vector<wstring> get_raw();

	template <typename T>
	Tensor_Basic& operator<< (const T & in);
	Tensor_Basic& operator<< (const string & in);
	Tensor_Basic& operator<< (const wstring & in);
	Tensor_Basic& operator<< (const char * in);
	Tensor_Basic& operator<< (const wchar_t * in);
	Tensor_Basic& operator<< (Tensor_Basic & in);
	Tensor_Basic& operator<< (const COMPLEX & in);
	Tensor_Basic& operator<< (const VECTOR & in);

	template <typename T>
	Tensor_Basic& operator=(const T & in);
	Tensor_Basic& operator=(const string & in);
	Tensor_Basic& operator=(const wstring & in);
	Tensor_Basic& operator=(const char * in);
	Tensor_Basic& operator=(const wchar_t * in);
	Tensor_Basic& operator=(Tensor_Basic & in);
	Tensor_Basic& operator=(const COMPLEX & in);
	Tensor_Basic& operator=(const VECTOR & in);

	template <typename T>
	void put_value(const T & in, unsigned long int idx);
	void put_value(const string & in, unsigned long int idx);
	void put_value(const wstring & in, unsigned long int idx);
	void put_value(const char * in, unsigned long int idx);
	void put_value(const wchar_t * in, unsigned long int idx);
	void put_value(const COMPLEX & in, unsigned long int idx);
	void put_value(const VECTOR & in, unsigned long int idx);


	/* Compare Value */
	template <typename T>
	bool operator==(const T & in);
	bool operator==(const string & in);
	bool operator==(const wstring & in);
	bool operator==(const char * in);
	bool operator==(const wchar_t * in);
	bool operator==(Tensor_Basic & in);
	bool operator==(const COMPLEX & in);
	bool operator==(const VECTOR & in);

	template <typename T>
	bool operator!=(const T & in);
	bool operator!=(const string & in);
	bool operator!=(const wstring & in);
	bool operator!=(const char * in);
	bool operator!=(const wchar_t * in);
	bool operator!=(Tensor_Basic & in);
	bool operator!=(const COMPLEX & in);
	bool operator!=(const VECTOR & in);

	template <typename T>
	bool operator>= (const T & in);
	bool operator>= (Tensor_Basic & in);

	template <typename T>
	bool operator<= (const T & in);
	bool operator<= (Tensor_Basic & in);

	template <typename T>
	bool operator> (const T & in);
	bool operator> (Tensor_Basic & in);

	template <typename T>
	bool operator< (const T & in);
	bool operator< (Tensor_Basic & in);

	/* Special function */
	bool is_real_tensor();
	bool is_complex_tensor();

	void reshape(unsigned int args, ...); // reshape // reshape function is also defined in outside

	void TP(); // transpose // transpose function is also defined in outside

	unsigned long int argmax(); //
	unsigned long int argmin(); //
	REAL max_(); //
	REAL min_(); //
	void sort_(bool descending_order = true); //
};



/****** Private Functions ******/

template<typename T>
inline Type Tensor_Basic::type_encoder(const T & value)
{
	Type type;
	if (typeid(value) == typeid(CHAR))
		type = Type::CHAR;
	else if (typeid(value) == typeid(WCHAR))
		type = Type::WCHAR;
	else if (typeid(value) == typeid(INTEGER))
		type = Type::INTEGER;
	else if (typeid(value) == typeid(REAL))
		type = Type::REAL;
	else if (typeid(value) == typeid(COMPLEX))
		type = Type::COMPLEX;
	else if (typeid(value) == typeid(VECTOR))
		type = Type::VECTOR;
	else if (typeid(value) == typeid(const char *))
		type = Type::STRING;
	else if (typeid(value) == typeid(string))
		type = Type::STRING;
	else if (typeid(value) == typeid(const wchar_t *))
		type = Type::WSTRING;
	else if (typeid(value) == typeid(wstring))
		type = Type::WSTRING;
	else
		type = Type::UNKNOWN;

	Collect_log(__func__);

	return type;
}

template<typename T>
inline wstring Tensor_Basic::to_string_with_precision(const T value)
{
	std::wostringstream streamObj;

	// set Fixed -Point Notation
	streamObj << std::fixed;

	// set precision to 2 digits
	streamObj << std::setprecision(precision);

	//Add double to stream
	streamObj << value;

	Collect_log(__func__);

	return streamObj.str();
}

template<typename T>
inline T Tensor_Basic::gen_randn(T min, T max)
{
	uniform_real_distribution<T> dist(min, max);

	Collect_log(__func__);

	return dist(gen_1);
}

template<typename T>
inline void Tensor_Basic::get_element_v1(T & out, unsigned long int pos)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);
	if (this->type[this->ref_pos_start] != type_encoder(out))
		Exception_Report(__func__, __LINE__);

	Convert_Value(out, this->ref_pos_start);

	init_param();
	Collect_log(__func__);
}

template<typename T>
inline T Tensor_Basic::get_element_v2(unsigned long int idx)
{
	T out;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);
	if (this->type[this->ref_pos_start] != type_encoder(out))
		Exception_Report(__func__, __LINE__);

	Convert_Value(out, this->ref_pos_start);

	init_param();
	Collect_log(__func__);

	return out;
}

template<typename T>
inline Tensor_Basic & Tensor_Basic::operator>>(T & out)
{
	if(this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if(this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);
	if(this->type[this->ref_pos_start] != type_encoder(out))
		Exception_Report(__func__, __LINE__);

	Convert_Value(out, this->ref_pos_start);

	init_param();
	Collect_log(__func__);

	return (*this);
}

template<typename T>
inline Tensor_Basic & Tensor_Basic::operator<<(const T & in)
{
	if(this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start] = to_string_with_precision(in);

	init_param();
	Collect_log(__func__);

	return (*this);
}

template<typename T>
inline Tensor_Basic & Tensor_Basic::operator=(const T & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(to_string_with_precision(in));

	init_param();
	Collect_log(__func__);

	return (*this);
}

template<typename T>
inline void Tensor_Basic::put_value(const T & in, unsigned long int idx)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if(idx >= this->size)
		Exception_Report(__func__, __LINE__);

	this->type[idx] = type_encoder(in);
	this->element[idx].clear();
	this->element[idx].push_back(to_string_with_precision(in));

	init_param();
	Collect_log(__func__);

	return (*this);
}

template<typename T>
inline bool Tensor_Basic::operator==(const T & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == to_string_with_precision(in)) {
		init_param();
		Collect_log(__func__);
		return true;
	}
	else {
		if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (type_encoder(in) == Type::INTEGER || type_encoder(in) == Type::REAL)) {
			REAL num_1 = stold(this->element[this->ref_pos_start][0]);
			REAL num_2 = stold(to_string_with_precision(in));

			init_param();
			Collect_log(__func__);

			if (fabsl(num_1 - num_2) < powl(10, precision * (-1)))
				return true;
			else
				return false;
		}

		init_param();
		Collect_log(__func__);
		return false;
	}
}

template<typename T>
inline bool Tensor_Basic::operator!=(const T & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == to_string_with_precision(in)) {
		init_param();
		Collect_log(__func__);
		return false;
	}
	else {
		if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (type_encoder(in) == Type::INTEGER || type_encoder(in) == Type::REAL)) {
			REAL num_1 = stold(this->element[this->ref_pos_start][0]);
			REAL num_2 = stold(to_string_with_precision(in));

			init_param();
			Collect_log(__func__);

			if (fabsl(num_1 - num_2) < powl(10, precision * (-1)))
				return false;
			else
				return true;
		}

		init_param();
		Collect_log(__func__);
		return true;
	}
}

template<typename T>
inline bool Tensor_Basic::operator>=(const T & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	// NUMBER
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (type_encoder(in) == Type::INTEGER || type_encoder(in) == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(to_string_with_precision(in));

		init_param();
		Collect_log(__func__);

		if (num_1 >= num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (type_encoder(in) != Type::COMPLEX && type_encoder(in) != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = to_string_with_precision(in);

		init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) >= 0)
			return true;
		else
			return false;
	}
	else {
		Exception_Report(__func__, __LINE__, false);
		init_param();
		Collect_log(__func__);
		return false;
	}
}

template<typename T>
inline bool Tensor_Basic::operator<=(const T & in)
{
	// NUMBER
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (type_encoder(in) == Type::INTEGER || type_encoder(in) == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(to_string_with_precision(in));

		init_param();
		Collect_log(__func__);

		if (num_1 <= num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (type_encoder(in) != Type::COMPLEX && type_encoder(in) != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = to_string_with_precision(in);

		init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) <= 0)
			return true;
		else
			return false;
	}
	else {
		Exception_Report(__func__, __LINE__, false);
		init_param();
		Collect_log(__func__);
		return false;
	}
}

template<typename T>
inline bool Tensor_Basic::operator>(const T & in)
{
	// NUMBER
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (type_encoder(in) == Type::INTEGER || type_encoder(in) == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(to_string_with_precision(in));

		init_param();
		Collect_log(__func__);

		if (num_1 > num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (type_encoder(in) != Type::COMPLEX && type_encoder(in) != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = to_string_with_precision(in);

		init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) > 0)
			return true;
		else
			return false;
	}
	else {
		Exception_Report(__func__, __LINE__, false);
		init_param();
		Collect_log(__func__);
		return false;
	}
}

template<typename T>
inline bool Tensor_Basic::operator<(const T & in)
{
	// NUMBER
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (type_encoder(in) == Type::INTEGER || type_encoder(in) == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(to_string_with_precision(in));

		init_param();
		Collect_log(__func__);

		if (num_1 < num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (type_encoder(in) != Type::COMPLEX && type_encoder(in) != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = to_string_with_precision(in);

		init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) < 0)
			return true;
		else
			return false;
	}
	else {
		Exception_Report(__func__, __LINE__, false);
		init_param();
		Collect_log(__func__);
		return false;
	}
}

#endif