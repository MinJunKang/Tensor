#include "Tensor.h"

#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

#include <crtdbg.h>

int main() {

	/* Memory Check code */
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(361);
#endif

	Setting();

	Tensor_Basic a(L"a");

	a.randu(3,15,3,3);

	Tensor_Basic b(L"b");
	COMPLEX c1(3.0, 4.0);
	COMPLEX c2(2.0, 5.0);
	VECTOR ex2;

	ex2 << 2.0 << 3.0 << 4.0;

	c1 = c1 * c2 + c2;
	c1 /= c2;

	cout << c1 << endl << ex2 << endl;
	cout << c1.abs() << endl;
	VECTOR ex3;

	b << a[0];
	b[0][0] << c1;

	b[0][0] >> c2;

	a[1][0][0] = 12334545656722855;
	b[1][0] = a[1][0][0];

	b[1][0].info();

	b[1][0].print();

	wstring str = b.Tensor2Str();

	b.Str2Tensor(str);

	b.print();

	Ending();

	return 0;
}