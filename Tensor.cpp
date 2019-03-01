#include "Tensor.h"

/****** Private Functions ******/


void Tensor_Basic::Alloc_Tensor(const vector<unsigned int> & tensor_dim)
{
	unsigned int tensor_rank = tensor_dim.size();

	if (tensor_rank == 0)
		Exception_Report(__func__, __LINE__);
	else {

		Dealloc_Tensor();

		this->dim = new unsigned int[tensor_rank];
		this->rank = tensor_rank;
		this->size = 1;

		for (unsigned int i = 0; i < tensor_rank; i++) {
			this->dim[i] = tensor_dim[i];
			this->size *= tensor_dim[i];
		}

		this->element = new vector<wstring>[this->size];
		this->type = new Type[this->size];
	}

	init_param();
	Collect_log(__func__);
}

void Tensor_Basic::Dealloc_Tensor()
{

	if (this->element != nullptr) {
		
		for (unsigned long int i = 0; i < this->size; i++) {
			element[i].clear();
		}
		delete[] element;
		delete[] type;
		delete[] dim;
		
		this->element = nullptr;
		this->type = nullptr;
		this->dim = nullptr;
	}

	this->rank = 0;
	this->size = 0;
	init_param();

	Collect_log(__func__);
}

void Tensor_Basic::init_param()
{
	this->ref_rank = 0;
	this->ref_pos_start = 0;
	this->ref_pos_end = this->size - 1;

	Collect_log(__func__);
}

wstring Tensor_Basic::Type2str(Type in)
{
	wstring type_name;

	switch (in) {
	case Type::CHAR:
		type_name = L"CHAR";
		break;
	case Type::WCHAR:
		type_name = L"WCHAR";
		break;
	case Type::INTEGER:
		type_name = L"INTEGER";
		break;
	case Type::REAL:
		type_name = L"REAL";
		break;
	case Type::COMPLEX:
		type_name = L"COMPLEX";
		break;
	case Type::VECTOR:
		type_name = L"VECTOR";
		break;
	case Type::STRING:
		type_name = L"STRING CLASS";
		break;
	case Type::WSTRING:
		type_name = L"WSTRING CLASS";
		break;
	default:
		type_name = L"UNKNOWN TYPE";
	}
	return type_name;
}


void Tensor_Basic::copy_element(vector<wstring> * element, unsigned long int start_pos) const
{
	if (element != nullptr && this->element != nullptr) {

		for (unsigned long int i = ref_pos_start; i <= ref_pos_end; i++) {

			unsigned int element_size = this->element[i].size();

			element[i - ref_pos_start + start_pos].clear();

			for (unsigned int j = 0; j < element_size; j++) {
				element[i - ref_pos_start + start_pos].push_back(this->element[i][j]);
			}
		}
	}
	else {
		Exception_Report(__func__, __LINE__); // the elements are not allocated
	}
}

vector<unsigned int> Tensor_Basic::copy_info() const
{
	vector<unsigned int> dims;

	if(this->rank != 0){
		if (this->rank == this->ref_rank)
			dims.push_back(1);
		else {
			for (unsigned int i = this->ref_rank; i < this->rank; i++) {
				dims.push_back(this->dim[i]);
			}
		}
	}
	else {
		Exception_Report(__func__, __LINE__); // rank cannot be 0
	}

	return dims;
}

void Tensor_Basic::copy_type(Type * type, unsigned long int start_pos) const
{
	if (type != nullptr) {
		for (unsigned long int i = ref_pos_start; i <= ref_pos_end; i++) {
			type[i - ref_pos_start + start_pos] = this->type[i];
		}
	}
	else {
		Exception_Report(__func__, __LINE__); // type is not allocated
	}
}

void Tensor_Basic::insert(const Tensor_Basic & Object)
{
	/* Check the condition */
	if(Object.rank == 0)
		Exception_Report(__func__, __LINE__); // the object is not allocated
	if (this->rank != 0) { // This is not first initialization
		if (this->ref_pos_end - this->ref_pos_start != Object.ref_pos_end - Object.ref_pos_start)
			Exception_Report(__func__, __LINE__); // copy length does not match
	}
	else {
		/* Re Allocate this class data */
		vector<unsigned int> tensor_info = Object.copy_info();
		Alloc_Tensor(tensor_info);
		tensor_info.clear();
	}

	/* Copy the data */
	Object.copy_element(this->element, this->ref_pos_start);
	Object.copy_type(this->type, this->ref_pos_start);
}

void Tensor_Basic::Exception_Report(string fname, int line, bool error) const
{
	wstring statement = this->name + L":" + To_wstr(fname + to_string(line));

	if (error) {
		unsigned int log_num = logs.size();

		statement += L"\n";

		for (unsigned int i = 0; i < log_num; i++) {
			statement += To_wstr(logs[i]);
		}
	}

	Print_Error(L"TB", statement, error);
}

void Tensor_Basic::Collect_log(string fname)
{
	if (this->logs.size() == 25) {
		this->logs.erase(this->logs.begin());
	}

	if (this->logs.size() == 0)
		this->logs.push_back(fname);
	else
		this->logs.push_back("," + fname);
}

void Tensor_Basic::Convert_Value(INTEGER & in, unsigned long int idx)
{
	if(this->element[idx].size() != 1)
		Exception_Report(__func__, __LINE__);
	in = stoll(this->element[idx][0]);

	Collect_log(__func__);
}

void Tensor_Basic::Convert_Value(CHAR & in, unsigned long int idx)
{
	if (this->element[idx].size() != 1)
		Exception_Report(__func__, __LINE__);
	in = (char)stoi(this->element[idx][0]);

	Collect_log(__func__);
}

void Tensor_Basic::Convert_Value(WCHAR & in, unsigned long int idx)
{
	if (this->element[idx].size() != 1)
		Exception_Report(__func__, __LINE__);
	in = (wchar_t)stoi(this->element[idx][0]);

	Collect_log(__func__);
}

void Tensor_Basic::Convert_Value(REAL & in, unsigned long int idx)
{
	if (this->element[idx].size() != 1)
		Exception_Report(__func__, __LINE__);
	in = stold(this->element[idx][0]);

	Collect_log(__func__);
}

void Tensor_Basic::Convert_Value(STRING & in, unsigned long int idx)
{
	if (this->element[idx].size() != 1)
		Exception_Report(__func__, __LINE__);
	in = To_str(this->element[idx][0]);

	Collect_log(__func__);
}

void Tensor_Basic::Convert_Value(WSTRING & in, unsigned long int idx)
{
	if (this->element[idx].size() != 1)
		Exception_Report(__func__, __LINE__);
	in = this->element[idx][0];

	Collect_log(__func__);
}

void Tensor_Basic::Convert_Value(COMPLEX & in, unsigned long int idx)
{
	if (this->element[idx].size() != 2)
		Exception_Report(__func__, __LINE__);
	in.real = stold(this->element[idx][0]);
	in.imag = stold(this->element[idx][1]);

	Collect_log(__func__);
}

void Tensor_Basic::Convert_Value(VECTOR & in, unsigned long int idx)
{
	if (this->element[idx].size() == 0)
		Exception_Report(__func__, __LINE__);

	unsigned int element_size = this->element[idx].size();

	for (unsigned int i = 0; i < element_size; i++) {
		in.append(stold(this->element[idx][i]));
	}

	Collect_log(__func__);
}

Tensor_Basic::Tensor_Basic(wstring name) : rank(0), size(0), precision(15), 
element(nullptr), type(nullptr), dim(nullptr), gen_1(time(NULL))
{
	this->name = name;
	srand(time(NULL));
	init_param();

	Collect_log(__func__);
}

Tensor_Basic::Tensor_Basic(const Tensor_Basic & new_tensor) : rank(0), size(0), precision(15), 
element(nullptr), type(nullptr), dim(nullptr), gen_1(time(NULL))
{
	srand(time(NULL));

	/* copy the elementary information */
	this->name = new_tensor.name;
	this->precision = new_tensor.precision;
	
	/* Check the condition */
	if (new_tensor.rank == 0 || new_tensor.ref_rank != 0)
		Exception_Report(__func__, __LINE__); // input tensor must be allocated
	else
		/* copy value */
		insert(new_tensor);

	init_param();

	Collect_log(__func__);
}

Tensor_Basic::~Tensor_Basic()
{
	Dealloc_Tensor();

	logs.clear();
}

const unsigned int Tensor_Basic::get_rank()
{
	unsigned int ranks = this->rank - this->ref_rank;

	init_param();

	Collect_log(__func__);

	return ranks;
}

const unsigned int Tensor_Basic::get_size()
{

	if(this->ref_rank != 0)
		Exception_Report(__func__, __LINE__);

	init_param();

	Collect_log(__func__);

	return this->size;
}

const Type Tensor_Basic::get_type()
{
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	init_param();

	Collect_log(__func__);

	return this->type[ref_pos_start];
}

wstring Tensor_Basic::get_name()
{
	init_param();

	Collect_log(__func__);

	return this->name;
}

vector<unsigned int> Tensor_Basic::shape()
{
	vector<unsigned int> shapes;

	if (!rank)
		Exception_Report(__func__, __LINE__);

	if (this->rank - this->ref_rank <= 1)
		shapes.push_back(1);
	if (this->rank == this->ref_rank)
		shapes.push_back(1);

	for (unsigned int i = this->ref_rank; i < this->rank; i++) {
		shapes.push_back(this->dim[i]);
	}

	init_param();

	Collect_log(__func__);

	return shapes;
}

void Tensor_Basic::set_precision(const int precision)
{
	this->precision = precision;

	init_param();

	Collect_log(__func__);
}

void Tensor_Basic::change_name(wstring new_name)
{
	this->name = new_name;

	init_param();

	Collect_log(__func__);
}

void Tensor_Basic::info()
{
	/* Print Object's Name */
	wcout << L"Info of " << this->name;

	unsigned long int start_pt = 0;
	vector<unsigned int> idxs;
	vector<unsigned int> tmp_shape;
	vector<Type> tmp_type;

	for (unsigned int i = 0; i < this->ref_rank; i++) {
		unsigned int pos = 1;
		for (unsigned int j = 0; j < this->rank - (i + 1); j++) {
			pos *= this->dim[this->rank - j - 1];
		}

		for (unsigned int j = this->dim[i] - 1; j >= 0; j--) {
			if (start_pt + pos * j <= this->ref_pos_start) {
				idxs.push_back(j);
				break;
			}
		}
		start_pt += pos * idxs.back();
	}

	unsigned int idx_size = idxs.size();
	for (unsigned int i = 0; i < idx_size; i++) {
		wcout << L"[" << idxs[i] << L"]";
	}
	idxs.clear();
	wcout << L":" << endl;

	
	if (this->element != nullptr) {

		/* get shapes */
		if (this->rank - this->ref_rank <= 1)
			tmp_shape.push_back(1);
		if (this->rank == this->ref_rank)
			tmp_shape.push_back(1);

		for (unsigned int i = this->ref_rank; i < this->rank; i++) {
			tmp_shape.push_back(this->dim[i]);
		}

		/* Shape print */
		wcout << L"\t\tShape = [ ";
		unsigned int tmp_shape_size = tmp_shape.size();
		for (int i = 0; i < tmp_shape_size; i++) {
			wcout << tmp_shape[i];
			if (i != tmp_shape_size - 1)
				wcout << L", ";
		}
		wcout << L" ]" << endl;
		tmp_shape.clear();

		/* Type print */
		unsigned int end = (this->ref_rank > 0) ? this->ref_pos_end : this->size - 1;
		wcout << L"\t\tType = { ";
		for (int i = ref_pos_start; i <= end; i++) {
			tmp_type.push_back(this->type[i]);
		}
		/* remove the same */
		sort(tmp_type.begin(), tmp_type.end());
		tmp_type.erase(unique(tmp_type.begin(), tmp_type.end()), tmp_type.end());

		unsigned int type_size = tmp_type.size();

		for (int i = 0; i < type_size; i++) {
			wcout << Type2str(tmp_type[i]);
			if (i != type_size - 1)
				wcout << ", ";
		}
		wcout << L" }";
		if (type_size > 1)
			wcout << L"[ Mix of " << type_size << L" types ]";

		/* print precision */
		wcout << endl << L"\t\tPrecision = 10 ^ -" << this->precision << endl;

		/* print value */
		if (this->rank == this->ref_rank) {
			if (this->type[this->ref_pos_start] == Type::COMPLEX) {
				COMPLEX num = get_element_v2<COMPLEX>(this->ref_pos_start);
				cout << "\t\tValue = " << num;
			}
			else if (this->type[this->ref_pos_start] == Type::VECTOR) {
				VECTOR num = get_element_v2<VECTOR>(this->ref_pos_start);
				cout << "\t\tValue = " << num;
			}
			else {
				WSTRING num = get_raw(this->ref_pos_start)[0];
				wcout << L"\t\tValue = " << num;
			}
		}
	}
	else {
		cout << endl << "     Tensor is not allocated!";
	}

	cout << endl;

	init_param();

	Collect_log(__func__);
}

void Tensor_Basic::print(short enter_posx, short max_row, short max_col, short word_space)
{
	if (word_space > max_col)
		word_space = max_col;

	/* Print Object's Name */
	wcout << this->name;

	unsigned long int start_pt = 0;
	vector<unsigned int> idxs;

	for (unsigned int i = 0; i < this->ref_rank; i++) {
		unsigned int pos = 1;
		for (unsigned int j = 0; j < this->rank - (i + 1); j++) {
			pos *= this->dim[this->rank - j - 1];
		}

		for (unsigned int j = this->dim[i] - 1; j >= 0; j--) {
			if (start_pt + pos * j <= this->ref_pos_start) {
				idxs.push_back(j);
				break;
			}
		}
		start_pt += pos * idxs.back();
	}

	unsigned int idx_size = idxs.size();
	for (unsigned int i = 0; i < idx_size; i++) {
		wcout << L"[" << idxs[i] << L"]";
	}
	idxs.clear();
	wcout << L":";

	/* Print the value */
	unsigned long int row_count = 0;
	short pos = 0;

	unsigned long int start = this->ref_pos_start;
	unsigned long int end = this->ref_pos_end;
	unsigned int stop_pt = 1;

	if (this->element != nullptr) {

		if (this->ref_rank != this->rank) {
			for (unsigned int i = 0; i < this->rank - (this->ref_rank + 1); i++) {
				stop_pt *= dim[this->rank - i - 1];
			}
		}

		if (stop_pt == 1) // vector
			stop_pt = end - start + 1;

		for (unsigned int i = start; i <= end; i++) {
			if (i % stop_pt == 0) {
				row_count++;
				if (row_count == max_row - 1 && end / stop_pt + 1 > max_row) {
					gotoxy(enter_posx / 2, wherey() + 2);
					wcout << L"..." << endl;
					i = end - stop_pt;
					row_count = (unsigned int)i / stop_pt + 1;
					continue;
				}
				else {
					cout << endl << "     " << row_count << "th row :";
				}
			}
			pos = wherex();
			wcout << L"  ";

			if (i % stop_pt == max_col - 1 && stop_pt > max_col) {
				wcout << L"   ...     ";
				i = row_count * stop_pt - 2;
				if (pos + word_space > enter_posx)
					gotoxy(15, wherey() + 2);
				else
					gotoxy(pos + word_space, wherey() + 1);
				continue;
			}

			unsigned int element_size = element[i].size();

			for (unsigned int j = 0; j < element_size; j++) {
				wcout << setprecision(this->precision) << element[i][j];

				if (i != end && j == 0 && element_size == 2) {
					gotoxy(pos + word_space / 2, wherey() + 1);
					wcout << L", ";
				}
				else if(i != end && j == 0 && element_size > 2){
					if (element_size < 10) {
						gotoxy(pos + 8, wherey() + 1);
						wcout << L"..." << L"(" << to_wstring(element_size) << L")";
					}
					else {
						gotoxy(pos + word_space / 2 - 2, wherey() + 1);
						wcout << L"..." << L"(>=10)";
					}
				}
				else if (i == end && j != element_size - 1 && element_size > 2) {
					gotoxy(pos + 8 * (j + 1), wherey() + 1);
					wcout << L", ";
				}
				else if (i == end && j == element_size - 1 && element_size > 2) {
					gotoxy(pos + 8 * element_size, wherey() + 1);
					wcout << L"(" << to_wstring(element_size) << L")        ";
				}
				else if (i == end && j == 0 && element_size == 2) {
					gotoxy(pos + 8, wherey() + 1);
					wcout << L", ";
				}
			}
			short space = pos + word_space - (wherex() + 1);
			if (space <= 0) {
				gotoxy(pos + word_space, wherey() + 1);
			}
			for (short k = 0; k < abs(space); k++) {
				cout << " ";
			}

			if (pos + word_space > enter_posx)
				gotoxy(15, wherey() + 2);
			else
				gotoxy(pos + word_space, wherey() + 1);
		}
	}
	else {
		cout << endl << "     Tensor is not allocated!";
	}
	cout << endl;

	init_param();

	Collect_log(__func__);
}

void Tensor_Basic::plot()
{
	/* Check condition */

	unsigned long int start_pt = 0;
	vector<unsigned int> idxs;

	for (unsigned int i = 0; i < this->ref_rank; i++) {
		unsigned int pos = 1;
		for (unsigned int j = 0; j < this->rank - (i + 1); j++) {
			pos *= this->dim[this->rank - j - 1];
		}

		for (unsigned int j = this->dim[i] - 1; j >= 0; j--) {
			if (start_pt + pos * j <= this->ref_pos_start) {
				idxs.push_back(j);
				break;
			}
		}
		start_pt += pos * idxs.back();
	}


}

wstring Tensor_Basic::Tensor2Str()
{
	wstring str;
	wstring ch = L"\t";
	wstring tmp;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);

	for (unsigned long int i = this->ref_pos_start; i <= this->ref_pos_end; i++) {

		unsigned int element_size = this->element[i].size();
		tmp.clear();

		for (unsigned int j = 0; j < element_size; j++) {
			tmp += this->element[i][j];
			if (j != element_size - 1)
				tmp += L",";
		}

		unsigned long int tmp_len = tmp.length();

		for (unsigned long int i = 0; i < tmp_len; i++) {
			if (tmp[i] == L'\t')
				tmp[i] = L' ';
		}
		str = str + tmp;
		str.append(ch);
	}

	init_param();

	Collect_log(__func__);

	return str;
}

bool Tensor_Basic::Str2Tensor(wstring str_tensor)
{
	/* temporary data storage */
	vector<vector<wstring>> tmp_element;
	vector<wstring> tmp_value;
	vector<Type> tmp_type;
	wstring tmp_str;
	unsigned int type_track = 0; // 0 : unknown, 1 : integer, 2 : real number, 3 : complex, 4 : str

	/* convert to tensor type */
	unsigned long int tensor_size = str_tensor.length();

	if (tensor_size == 0) {
		Exception_Report(__func__, __LINE__, false); // warning : string is not valid tensor
		return false;
	}
	else {
		if (str_tensor[tensor_size - 1] != L'\t') {
			Exception_Report(__func__, __LINE__, false); // warning : string is not valid tensor
			return false;
		}

		for (unsigned long int i = 0; i < tensor_size; i++) {

			wchar_t c = str_tensor[i];

			if (c == L'\t') {
				switch (type_track) {
				case 1:
					tmp_type.push_back(Type::INTEGER);
					break;
				case 2:
					tmp_type.push_back(Type::REAL);
					break;
				case 3:
					tmp_type.push_back(Type::COMPLEX);
					break;
				case 4:
					if (tmp_value.size() == 1 && tmp_value[0].length() == 1)
						tmp_type.push_back(Type::WCHAR);
					else
						tmp_type.push_back(Type::WSTRING);
					break;
				case 5:
					tmp_type.push_back(Type::VECTOR);
					break;
				default:
					Exception_Report(__func__, __LINE__, false); // warning : string is not valid tensor
					return false;
				}

				tmp_value.push_back(tmp_str);
				tmp_element.push_back(tmp_value);

				/* initialize */
				type_track = 0;
				tmp_value.clear();
				tmp_str.clear();
			}
			else if(c == L','){
				/* if type is supposed to be number */
				if (type_track != 0 && type_track != 4) {
					if (type_track == 3 || type_track == 5) { 
						type_track = 5; // The type should be set
						tmp_value.push_back(tmp_str);
						tmp_str.clear();
					}
					else {
						type_track = 3; // the type should be long complex
						tmp_value.push_back(tmp_str);
						tmp_str.clear();
					}
				}
				else {
					type_track = 4; // type should be string
					tmp_str.push_back(c);
				}
			}
			else {
				tmp_str.push_back(c);

				if (isdigit(c)) {
					if (type_track == 0)
						type_track = 1;
					else if(type_track == 4 && tmp_str.length() == 2 && tmp_str[0] == L'-'){
						type_track = 1;
					}
				}
				else if (c == L'.') {
					if (type_track == 1)
						type_track = 2;
					else if (type_track == 0)
						type_track = 4;
				}
				else {
					type_track = 4;
				}
			}
		}
	}

	/* Alloc to tensor */
	vector<unsigned int> tensor_info;
	tensor_info.push_back(tmp_element.size());

	Alloc_Tensor(tensor_info);

	for (unsigned long int i = 0; i < this->size; i++) {
		unsigned int element_size = tmp_element[i].size();
		for (unsigned int j = 0; j < element_size; j++) {
			this->element[i].push_back(tmp_element[i][j]);
		}
		this->type[i] = tmp_type[i];
	}

	init_param();

	Collect_log(__func__);

	return true;
}

void Tensor_Basic::zeros(unsigned int args, ...)
{
	vector<unsigned int> array_info;
	va_list ap;
	va_start(ap, args);
	unsigned int rank_ = args;

	if (rank_ == 0)
		Exception_Report(__func__, __LINE__);

	for (unsigned int i = 0; i < rank_; i++) {
		array_info.push_back(va_arg(ap, unsigned int));
	}

	/* Allocator */
	Alloc_Tensor(array_info);

	/* Initialize */
	for (unsigned int i = 0; i < this->size; i++) {

		element[i].push_back(to_wstring(0));

		type[i] = Type::INTEGER;
	}

	init_param();
	array_info.clear();
	va_end(ap);

	Collect_log(__func__);
}

void Tensor_Basic::ones(unsigned int args, ...)
{
	vector<unsigned int> array_info;
	va_list ap;
	va_start(ap, args);
	unsigned int rank_ = args;

	if (rank_ == 0)
		Exception_Report(__func__, __LINE__);

	for (unsigned int i = 0; i < rank_; i++) {
		array_info.push_back(va_arg(ap, unsigned int));
	}

	/* Allocator */
	Alloc_Tensor(array_info);

	/* Initialize */
	for (unsigned int i = 0; i < this->size; i++) {

		element[i].push_back(to_wstring(1));

		type[i] = Type::INTEGER;
	}

	init_param();
	array_info.clear();
	va_end(ap);

	Collect_log(__func__);
}

void Tensor_Basic::randu(unsigned int args, ...)
{
	vector<unsigned int> array_info;
	va_list ap;
	va_start(ap, args);
	unsigned int rank_ = args;

	if (rank_ == 0)
		Exception_Report(__func__, __LINE__);

	for (unsigned int i = 0; i < rank_; i++) {
		array_info.push_back(va_arg(ap, unsigned int));
	}

	/* Allocator */
	Alloc_Tensor(array_info);

	/* Initialize */
	for (unsigned int i = 0; i < this->size; i++) {

		element[i].push_back(to_string_with_precision(gen_randn(0.0, 1.0)));

		type[i] = Type::REAL;
	}

	init_param();
	array_info.clear();
	va_end(ap);

	Collect_log(__func__);
}

double Tensor_Basic::gaussianRandom(double average, double stdev) {
	double v1, v2, s, temp;

	do {
		v1 = gen_randn(-1.0, 1.0);      // -1.0 ~ 1.0 까지의 값
		v2 = gen_randn(-1.0, 1.0);      // -1.0 ~ 1.0 까지의 값
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);

	s = sqrt((-2 * log(s)) / s);

	temp = v1 * s;
	temp = (stdev * temp) + average;

	Collect_log(__func__);

	return temp;
}

void Tensor_Basic::randn(unsigned int args, ...)
{
	vector<unsigned int> array_info;
	va_list ap;
	va_start(ap, args);
	unsigned int rank_ = args;

	if (rank_ == 0)
		Exception_Report(__func__, __LINE__);

	for (unsigned int i = 0; i < rank_; i++) {
		array_info.push_back(va_arg(ap, unsigned int));
	}

	/* Allocator */
	Alloc_Tensor(array_info);

	/* Initialize */
	for (unsigned int i = 0; i < this->size; i++) {

		element[i].push_back(to_string_with_precision(gaussianRandom(0.0,1.0)));

		type[i] = Type::REAL;
	}

	init_param();
	array_info.clear();
	va_end(ap);

	Collect_log(__func__);
}

void Tensor_Basic::initu(vector<unsigned int> dims, double min, double max)
{
	unsigned int dim_size = dims.size();

	if (dim_size == 0)
		Exception_Report(__func__, __LINE__);

	/* Allocator */
	Alloc_Tensor(dims);

	/* Initialize */
	for (unsigned int i = 0; i < this->size; i++) {

		element[i].push_back(to_string_with_precision(gen_randn(min, max)));

		type[i] = Type::REAL;
	}
	init_param();
	Collect_log(__func__);
}

void Tensor_Basic::initg(vector<unsigned int> dims, double average, double stdev)
{
	unsigned int dim_size = dims.size();

	if (dim_size == 0)
		Exception_Report(__func__, __LINE__);

	/* Allocator */
	Alloc_Tensor(dims);

	/* Initialize */
	for (unsigned int i = 0; i < this->size; i++) {

		element[i].push_back(to_string_with_precision(gaussianRandom(average, stdev)));

		type[i] = Type::REAL;
	}
	init_param();
	Collect_log(__func__);
}

Tensor_Basic & Tensor_Basic::operator[](unsigned int idx)
{
	/* Condition Check */
	if(this->element == nullptr)
		Exception_Report(__func__, __LINE__);
	else if(this->ref_rank == this->rank)
		Exception_Report(__func__, __LINE__);
	else if(idx >= this->dim[this->ref_rank])
		Exception_Report(__func__, __LINE__);

	/* Calculate position */
	unsigned long int pos = 1;

	for (int i = 0; i < this->rank - (this->ref_rank + 1); i++) {
		pos *= this->dim[this->rank - i - 1];
	}

	this->ref_pos_end = ref_pos_start;
	this->ref_pos_start += pos * idx;
	this->ref_pos_end += pos * (idx + 1) - 1;
	this->ref_rank++;

	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator>>(Tensor_Basic & out)
{
	
	Collect_log(__func__);
	return (*this);
}

vector<wstring> Tensor_Basic::get_raw(unsigned long int pos)
{
	if(this->ref_rank == 0)
		Exception_Report(__func__, __LINE__);
	if(pos >= this->size)
		Exception_Report(__func__, __LINE__);

	return this->element[pos];
}

vector<wstring> Tensor_Basic::get_raw()
{
	vector<wstring> raw_result;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	unsigned int element_size = this->element[this->ref_pos_start].size();

	for (unsigned int i = 0; i < element_size; i++) {
		raw_result.push_back(this->element[this->ref_pos_start][i]);
	}

	init_param();
	Collect_log(__func__);

	return raw_result;
}

Tensor_Basic & Tensor_Basic::operator<<(const string & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(To_wstr(in));

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator<<(const wstring & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(in);

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator<<(const char * in)
{
	string new_element = in;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(To_wstr(new_element));

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator<<(const wchar_t * in)
{
	wstring new_element = in;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(new_element);

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator<<(Tensor_Basic & in)
{
	/* copy value */
	insert(in);

	set_precision(in.precision);

	in.init_param();
	init_param();

	Collect_log(__func__);
	return (*this);
}

Tensor_Basic & Tensor_Basic::operator<<(const COMPLEX & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(to_string_with_precision(in.real));
	this->element[this->ref_pos_start].push_back(to_string_with_precision(in.imag));

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator<<(const VECTOR & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();

	unsigned int set_size = in.value.size();
	for (unsigned int i = 0; i < set_size; i++) {
		this->element[this->ref_pos_start].push_back(to_string_with_precision(in.value[i]));
	}

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator=(const string & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(To_wstr(in));

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator=(const wstring & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(in);

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator=(const char * in)
{
	string new_element = in;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(To_wstr(new_element));

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator=(const wchar_t * in)
{
	wstring new_element = in;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(new_element);

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator=(Tensor_Basic & in)
{
	/* copy value */
	insert(in);

	set_precision(in.precision);

	in.init_param();
	init_param();

	Collect_log(__func__);
	return (*this);
}

Tensor_Basic & Tensor_Basic::operator=(const COMPLEX & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();
	this->element[this->ref_pos_start].push_back(to_string_with_precision(in.real));
	this->element[this->ref_pos_start].push_back(to_string_with_precision(in.imag));

	init_param();
	Collect_log(__func__);

	return (*this);
}

Tensor_Basic & Tensor_Basic::operator=(const VECTOR & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_rank != this->rank)
		Exception_Report(__func__, __LINE__);

	this->type[this->ref_pos_start] = type_encoder(in);
	this->element[this->ref_pos_start].clear();

	unsigned int set_size = in.value.size();
	for (unsigned int i = 0; i < set_size; i++) {
		this->element[this->ref_pos_start].push_back(to_string_with_precision(in.value[i]));
	}

	init_param();
	Collect_log(__func__);

	return (*this);
}

void Tensor_Basic::put_value(const string & in, unsigned long int idx)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (idx >= this->size)
		Exception_Report(__func__, __LINE__);

	this->type[idx] = type_encoder(in);
	this->element[idx].clear();
	this->element[idx].push_back(To_wstr(in));

	init_param();
	Collect_log(__func__);
}

void Tensor_Basic::put_value(const wstring & in, unsigned long int idx)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (idx >= this->size)
		Exception_Report(__func__, __LINE__);

	this->type[idx] = type_encoder(in);
	this->element[idx].clear();
	this->element[idx].push_back(in);

	init_param();
	Collect_log(__func__);
}

void Tensor_Basic::put_value(const char * in, unsigned long int idx)
{
	wstring str = To_wstr(in);

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (idx >= this->size)
		Exception_Report(__func__, __LINE__);

	this->type[idx] = type_encoder(in);
	this->element[idx].clear();
	this->element[idx].push_back(str);

	init_param();
	Collect_log(__func__);
}

void Tensor_Basic::put_value(const wchar_t * in, unsigned long int idx)
{
	wstring str = in;

	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (idx >= this->size)
		Exception_Report(__func__, __LINE__);

	this->type[idx] = type_encoder(in);
	this->element[idx].clear();
	this->element[idx].push_back(str);

	init_param();
	Collect_log(__func__);
}

void Tensor_Basic::put_value(const COMPLEX & in, unsigned long int idx)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (idx >= this->size)
		Exception_Report(__func__, __LINE__);

	this->type[idx] = type_encoder(in);
	this->element[idx].clear();
	this->element[idx].push_back(to_string_with_precision(in.real));
	this->element[idx].push_back(to_string_with_precision(in.imag));

	init_param();
	Collect_log(__func__);
}

void Tensor_Basic::put_value(const VECTOR & in, unsigned long int idx)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (idx >= this->size)
		Exception_Report(__func__, __LINE__);

	this->type[idx] = type_encoder(in);
	this->element[idx].clear();

	unsigned int set_size = in.value.size();
	for (unsigned int i = 0; i < set_size; i++) {
		this->element[idx].push_back(to_string_with_precision(in.value[i]));
	}

	init_param();
	Collect_log(__func__);
}

bool Tensor_Basic::operator==(const string & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == To_wstr(in)) {
		init_param();
		Collect_log(__func__);
		return true;
	}
	else {
		init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator==(const wstring & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == in) {
		init_param();
		Collect_log(__func__);
		return true;
	}
	else {
		init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator==(const char * in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == To_wstr(in)) {
		init_param();
		Collect_log(__func__);
		return true;
	}
	else {
		init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator==(const wchar_t * in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	wstring str = in;

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == str) {
		init_param();
		Collect_log(__func__);
		return true;
	}
	else {
		init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator==(Tensor_Basic & in)
{
	if (this->rank == 0 || in.rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_pos_end - this->ref_pos_start != in.ref_pos_end - in.ref_pos_start)
		return false;

	for (unsigned long int i = in.ref_pos_start; i <= in.ref_pos_end; i++) {
		if ((this->element[i - in.ref_pos_start + this->ref_pos_start] != in.element[i]) || (this->type[i - in.ref_pos_start + this->ref_pos_start] != in.type[i])) {
			in.init_param();
			init_param();
			Collect_log(__func__);
			return false;
		}
	}

	in.init_param();
	init_param();
	Collect_log(__func__);

	return true;
}

bool Tensor_Basic::operator==(const COMPLEX & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	vector<wstring> complex_value;
	complex_value.push_back(to_string_with_precision(in.real));
	complex_value.push_back(to_string_with_precision(in.imag));

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start] == complex_value) {
		init_param();
		Collect_log(__func__);
		return true;
	}
	else {
		init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator==(const VECTOR & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	vector<wstring> value_;
	unsigned int vector_size = in.value.size();
	for (unsigned int i = 0; i < vector_size; i++) {
		value_.push_back(to_string_with_precision(in.value[i]));
	}

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start] == value_) {
		init_param();
		Collect_log(__func__);
		return true;
	}
	else {
		init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator!=(const string & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == To_wstr(in)) {
		init_param();
		Collect_log(__func__);
		return false;
	}
	else {
		init_param();
		Collect_log(__func__);
		return true;
	}
}

bool Tensor_Basic::operator!=(const wstring & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == in) {
		init_param();
		Collect_log(__func__);
		return false;
	}
	else {
		init_param();
		Collect_log(__func__);
		return true;
	}
}

bool Tensor_Basic::operator!=(const char * in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == To_wstr(in)) {
		init_param();
		Collect_log(__func__);
		return false;
	}
	else {
		init_param();
		Collect_log(__func__);
		return true;
	}
}

bool Tensor_Basic::operator!=(const wchar_t * in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	wstring str = in;

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start][0] == str) {
		init_param();
		Collect_log(__func__);
		return false;
	}
	else {
		init_param();
		Collect_log(__func__);
		return true;
	}
}

bool Tensor_Basic::operator!=(Tensor_Basic & in)
{
	if (this->rank == 0 || in.rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->ref_pos_end - this->ref_pos_start != in.ref_pos_end - in.ref_pos_start)
		return false;

	for (unsigned long int i = in.ref_pos_start; i <= in.ref_pos_end; i++) {
		if ((this->element[i - in.ref_pos_start + this->ref_pos_start] != in.element[i]) || (this->type[i - in.ref_pos_start + this->ref_pos_start] != in.type[i])) {
			in.init_param();
			init_param();
			Collect_log(__func__);
			return true;
		}
	}

	in.init_param();
	init_param();
	Collect_log(__func__);

	return false;
}

bool Tensor_Basic::operator!=(const COMPLEX & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	vector<wstring> complex_value;
	complex_value.push_back(to_string_with_precision(in.real));
	complex_value.push_back(to_string_with_precision(in.imag));

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start] == complex_value) {
		init_param();
		Collect_log(__func__);
		return false;
	}
	else {
		init_param();
		Collect_log(__func__);
		return true;
	}
}

bool Tensor_Basic::operator!=(const VECTOR & in)
{
	if (this->rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank)
		Exception_Report(__func__, __LINE__);

	vector<wstring> value_;
	unsigned int vector_size = in.value.size();
	for (unsigned int i = 0; i < vector_size; i++) {
		value_.push_back(to_string_with_precision(in.value[i]));
	}

	if (this->type[this->ref_pos_start] == type_encoder(in) && this->element[this->ref_pos_start] == value_) {
		init_param();
		Collect_log(__func__);
		return false;
	}
	else {
		init_param();
		Collect_log(__func__);
		return true;
	}
}

bool Tensor_Basic::operator>=(Tensor_Basic & in)
{
	if (this->rank == 0 || in.rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank || in.rank != in.ref_rank)
		Exception_Report(__func__, __LINE__);

	// Number
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (in.type[in.ref_pos_start] == Type::INTEGER || in.type[in.ref_pos_start] == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(in.element[in.ref_pos_start][0]);

		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1 >= num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (in.type[in.ref_pos_start] != Type::COMPLEX && in.type[in.ref_pos_start] != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = in.element[in.ref_pos_start][0];

		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) >= 0)
			return true;
		else
			return false;
	}
	else {
		Exception_Report(__func__, __LINE__, false);
		init_param();
		in.init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator<=(Tensor_Basic & in)
{
	if (this->rank == 0 || in.rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank || in.rank != in.ref_rank)
		Exception_Report(__func__, __LINE__);

	// Number
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (in.type[in.ref_pos_start] == Type::INTEGER || in.type[in.ref_pos_start] == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(in.element[in.ref_pos_start][0]);

		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1 <= num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (in.type[in.ref_pos_start] != Type::COMPLEX && in.type[in.ref_pos_start] != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = in.element[in.ref_pos_start][0];

		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) <= 0)
			return true;
		else
			return false;
	}
	else {
		Exception_Report(__func__, __LINE__, false);
		init_param();
		in.init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator>(Tensor_Basic & in)
{
	if (this->rank == 0 || in.rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank || in.rank != in.ref_rank)
		Exception_Report(__func__, __LINE__);

	// Number
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (in.type[in.ref_pos_start] == Type::INTEGER || in.type[in.ref_pos_start] == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(in.element[in.ref_pos_start][0]);

		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1 > num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (in.type[in.ref_pos_start] != Type::COMPLEX && in.type[in.ref_pos_start] != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = in.element[in.ref_pos_start][0];

		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) > 0)
			return true;
		else
			return false;
	}
	else {
		Exception_Report(__func__, __LINE__, false);
		init_param();
		in.init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::operator<(Tensor_Basic & in)
{
	if (this->rank == 0 || in.rank == 0)
		Exception_Report(__func__, __LINE__);
	if (this->rank != this->ref_rank || in.rank != in.ref_rank)
		Exception_Report(__func__, __LINE__);

	// Number
	if ((this->type[this->ref_pos_start] == Type::INTEGER || this->type[this->ref_pos_start] == Type::REAL) && (in.type[in.ref_pos_start] == Type::INTEGER || in.type[in.ref_pos_start] == Type::REAL)) {
		REAL num_1 = stold(this->element[this->ref_pos_start][0]);
		REAL num_2 = stold(in.element[in.ref_pos_start][0]);

		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1 < num_2)
			return true;
		else
			return false;
	}
	// STRING, CHAR
	else if ((this->type[this->ref_pos_start] != Type::COMPLEX && this->type[this->ref_pos_start] != Type::VECTOR) && (in.type[in.ref_pos_start] != Type::COMPLEX && in.type[in.ref_pos_start] != Type::VECTOR)) {
		wstring num_1 = this->element[this->ref_pos_start][0];
		wstring num_2 = in.element[in.ref_pos_start][0];
		
		init_param();
		in.init_param();
		Collect_log(__func__);

		if (num_1.compare(num_2) < 0)
			return true;
		else
			return false;
	}
	else {
		
		Exception_Report(__func__, __LINE__, false);
		init_param();
		in.init_param();
		Collect_log(__func__);
		return false;
	}
}

bool Tensor_Basic::is_real_tensor()
{
	if (this->rank == 0)
		return false;
	
	for (unsigned long int i = this->ref_pos_start; i <= this->ref_pos_end; i++) {
		if (this->type[i] != Type::INTEGER || this->type[i] != Type::REAL)
			return false;
	}

	init_param();
	Collect_log(__func__);

	return true;
}

bool Tensor_Basic::is_complex_tensor()
{
	bool is_complex = false;

	if (this->rank == 0)
		return false;

	for (unsigned long int i = this->ref_pos_start; i <= this->ref_pos_end; i++) {
		if (this->type[i] != Type::INTEGER || this->type[i] != Type::REAL || this->type[i] != Type::COMPLEX)
			return false;
		if (is_complex == false) {
			if(this->type[i] == Type::COMPLEX)
				is_complex = true;
		}
	}

	init_param();
	Collect_log(__func__);

	return is_complex;
}

void Tensor_Basic::reshape(unsigned int args, ...)
{

}

void Tensor_Basic::TP()
{

}

unsigned long int Tensor_Basic::argmax()
{
	return 0;
}

unsigned long int Tensor_Basic::argmin()
{
	return 0;
}

REAL Tensor_Basic::max_()
{
	return REAL();
}

REAL Tensor_Basic::min_()
{
	return REAL();
}

void Tensor_Basic::sort_(bool descending_order)
{

}

ostream & operator<<(ostream & out, const COMPLEX & c)
{
	out << setprecision(15) << "(" << c.real << ") + (" << c.imag << ")i";

	return out;
}

istream & operator>>(istream & in, COMPLEX & c)
{
	cout << "Enter Real Part >> ";
	in >> c.real;
	cout << "Enter Imagenory Part >> ";
	in >> c.imag;

	return in;
}

ostream & operator<<(ostream & out, const VECTOR & c)
{
	unsigned int size = c.value.size();

	out << "( ";

	for (unsigned int i = 0; i < size; i++) {
		out << setprecision(15) << c.value[i];
		if (i != size - 1)
			out << ", ";
	}

	out << " )";

	return out;
}

istream & operator>>(istream & in, VECTOR & c)
{
	unsigned int size = 0;
	long double element;

	c.value.clear();

	cout << "Enter set's size >> ";
	in >> size;

	for (unsigned int i = 0; i < size; i++) {
		cout << "Enter " << i + 1 << "st element >> ";
		in >> element;
		c.append(element);
	}

	return in;
}

ostream & operator<<(ostream & out, Tensor_Basic & tensor)
{
	tensor.print();

	return out;
}

VECTOR::~VECTOR()
{
	this->value.clear();
}

VECTOR & VECTOR::operator<<(long double in)
{
	append(in);

	return (*this);
}

VECTOR & VECTOR::operator=(const VECTOR & in)
{
	cout << in.value.size();
	this->value = in.value;

	return (*this);
}

COMPLEX & COMPLEX::operator=(const COMPLEX & in)
{
	this->real = in.real;
	this->imag = in.imag;

	return (*this);
}

COMPLEX & COMPLEX::operator=(const REAL in)
{
	this->real = in;
	this->imag = 0.0;

	return (*this);
}

COMPLEX & COMPLEX::operator+(const COMPLEX & in)
{
	this->real += in.real;
	this->imag += in.imag;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator+(const REAL in)
{
	this->real += in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator-(const COMPLEX & in)
{
	this->real -= in.real;
	this->imag -= in.imag;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator-(const REAL in)
{
	this->real -= in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator*(const COMPLEX & in)
{
	this->real = this->real * in.real - this->imag * in.imag;
	this->imag = this->real * in.imag + this->imag * in.real;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator*(const REAL in)
{
	this->real *= in;
	this->imag *= in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator/(const COMPLEX & in)
{
	long double num = in.real * in.real + in.imag * in.imag;

	this->real = (this->real * in.real + this->imag * in.imag) / num;
	this->imag = (this->imag * in.real - this->real * in.imag) / num;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator/(const REAL in)
{
	this->real /= in;
	this->imag /= in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator+=(const COMPLEX & in)
{
	this->real += in.real;
	this->imag += in.imag;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator+=(const REAL in)
{
	this->real += in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator-=(const COMPLEX & in)
{
	this->real -= in.real;
	this->imag -= in.imag;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator*=(const COMPLEX & in)
{
	this->real = this->real * in.real - this->imag * in.imag;
	this->imag = this->real * in.imag + this->imag * in.real;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator*=(const REAL in)
{
	this->real *= in;
	this->imag *= in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator/=(const COMPLEX & in)
{
	long double num = in.real * in.real + in.imag * in.imag;

	this->real = (this->real * in.real + this->imag * in.imag) / num;
	this->imag = (this->imag * in.real - this->real * in.imag) / num;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator/=(const REAL in)
{
	this->real /= in;
	this->imag /= in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::operator^(unsigned int num)
{
	for (unsigned int i = 0; i < num; i++) {
		(*this) *= (*this);
	}

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

COMPLEX & COMPLEX::exp(const COMPLEX & in)
{
	this->real = expl(in.real) * cosl(in.imag);
	this->imag = expl(in.real) * sinl(in.imag);

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}

long double COMPLEX::abs()
{
	return sqrtl(this->real * this->real + this->imag * this->imag);
}

long double COMPLEX::phase()
{
	return atanl(this->imag / this->real);
}

bool COMPLEX::operator==(const COMPLEX & in)
{
	if (fabsl(in.real - this->real) < 1e-100 && fabsl(in.imag - this->imag) < 1e-100)
		return true;
	else
		return false;
}

bool COMPLEX::operator!=(const COMPLEX & in)
{
	if (fabsl(in.real - this->real) < 1e-100 && fabsl(in.imag - this->imag) < 1e-100)
		return false;
	else
		return true;
}

bool COMPLEX::operator==(const REAL in)
{
	if (fabsl(in - this->real) < 1e-100)
		return true;
	else
		return false;
}

bool COMPLEX::operator!=(const REAL in)
{
	if (fabsl(in - this->real) < 1e-100)
		return false;
	else
		return true;
}

COMPLEX & COMPLEX::operator-=(const REAL in)
{
	this->real -= in;

	if (this->real < 1e-308)
		this->real = 0;
	if (this->imag < 1e-308)
		this->imag = 0;

	return (*this);
}
