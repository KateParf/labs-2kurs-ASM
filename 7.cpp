#include<iostream>
#include <cmath>

using namespace std;

bool err = 0;

// (y+3)^(y+cos2y)
// a^b = 2^(b*log2(a))
double func(double y)
{
	double res; int status;
	double t3 = 3; double t2 = 2; double t1 = 1;
	_asm {
		finit; команда инициализации сопроцессора
		; 2 * y
		fld qword ptr[y]; st(0) = y
		fmul t2; st(0) = 2 * y

		; cos2y
		fcos; st(0) = cos(2 * y)

		; y + cos2y
		fld qword ptr[y]; st(0) = y | st(1) = cos(2 * y)
		faddp st(1), st; st(0) = y + cos(2 * y)
		

		; y + 3
		fld qword ptr[y]; st(0) = y | st(1) = y + cos(2 * y)
		fadd t3; st(0) = y + 3 | st(1) = y + cos(2 * y)
		
		ftst; сравнение y + 3 с 0 - ограничение логарифма
		fnstsw ax;  записываем значения SWR в AX
		sahf;  записываем содержимое AH в младший байт EFLAGS
		ja if_great
		jz error_zero

		if_great :
			fyl2x; st(0) = (y + cos(2 * y)) * log2(y + 3)
			fld st; st(0) = st(1) = (y + cos(2 * y)) * log2(y + 3)
			frndint; в st(0) целая часть st(1)
			fsub st(1), st; в st(1) дробная часть
			fxch st(1); st(0) <=> st(1)
			f2xm1; st(0) = 2 ^ (дробная часть) - 1
			fld1; st(0) = 1 | st(1) = 2 ^ (дробная часть) - 1 | st(2) = целая часть
			faddp   st(1), st; st(0) = 2 ^ (дробная часть) | st(1) = целая часть
			fscale; st(0) = 2 ^ (дробная часть) * 2 ^ (целая часть)
			; = 2 ^ (дробная + целая) = 2 ^ (y + cos(2 * y)) * log2(y + 3)
			jmp endcalc

		error_zero :
			mov err, 1; формируем результат ошибки
			jmp endcalc

		endcalc :
			fst res;
	}
}

int main() {

	double y, res1;
	cout << "Enter y:" << endl;
	cin >> y;
	double res = func(y);

	// -- результат на ассемблере
	if (err) {
		cout << "log(b) b <= 0 error" << endl;
		return 1;
	}
	cout << "ASM: " << res << endl;

	// -- проверяем результат на плюсах
	res1 = pow(y + 3, (y + cos(2 * y)));
	cout << "C++: " << res1 << endl;

	return 0;
}