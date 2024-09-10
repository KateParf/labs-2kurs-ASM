#include<iostream>

using namespace std;


int main() {
	double a, b, c;
	double err0 = 0; double resAsm = 0, resCpp = 0;

	cout << "Enter a:" << endl;
	cin >> a;
	cout << "Enter b:" << endl;
	cin >> b;
	cout << "Enter c:" << endl;
	cin >> c;
	
	const double t1 = 23; const double t2 = 4; const double t3 = 1;
	_asm {
		finit; команда инициализации сопроцессора

		; c + 23
		fld c; st(0) = c
		fadd t1; st(0) = c + 23

		; b * 4
		fld b; st(0) = b | st(1) = c + 23
		fmul t2; st(0) = b * 4 | st(1) = c + 23

		; c + 23 - (b * 4)
		fsubp st(1), st; st(0) = c + 23 - b * 4

		; a* c
		fld a; st(0) = a | st(1) = c + 23 - b * 4
		fmul c; st(0) = a * c | st(1) = c + 23 - b * 4

		; (a * c) / b
		fld b; st(0) = b | st(1) = a * c | st(2) = c + 23 - b * 4
		ftst
		fnstsw ax;  записываем значения SWR в AX
		sahf;  записываем содержимое AH в младший байт EFLAGS
		jz division_by_zero
		fdivp st(1), st; st(0) = a * c / b | st(1) = c + 23 - b * 4

		; (a * c / b) - 1
		fsub t3; st(0) = (a * c / b) - 1 | st(1) = c + 23 - b * 4

		; (c + 23 - (b * 4)) / ((a * c / b) - 1)
		ftst; сравнение st(0) с 0
		fnstsw ax
		sahf;  записываем содержимое AH в младший байт EFLAGS
		jz division_by_zero
		sahf
		fdivp st(1), st; st(0) = (c + 23 - b * 4) / (a * c / b - 1)

		fstp resAsm; resAsm = st(0)
		jmp quit

		division_by_zero :
			mov err0, 1
			jmp quit

		quit :
	}

	// -- результат на ассемблере
	if (err0) {
		cout << "Division by zero error" << endl;
		return 1;
	}
	cout << "ASM: " << resAsm << endl;

	// -- проверяем результат на плюсах
	// ((c + 23 - b * 4)) / ((a * c / b - 1))
	resCpp = (c + 23 - (b * 4)) / ((a * c / b) - 1);
	cout << "C++: " << resCpp << endl;

	return 0;
}