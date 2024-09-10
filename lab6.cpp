#include<iostream>

using namespace std;

bool err = 0;

// ---7 (y+3)**(y+cos2y)
double func2(double a, double b)
{
	double res; int status; 
	double t1 = -2; double t2 = 2; double t3 = -10; double t4 = 3;
	_asm {
		finit; команда инициализации сопроцессора
		fld qword ptr[b]; st(0) = b
		fld qword ptr[a]; st(0) = a | st(1) = b
		fcompp; сравниваем a и b
		fstsw status; сохраняем регистр флагов сопроцессора
		mov ah, byte ptr[status + 1]
		sahf; записываем в регистр флагов процессора
		ja a_bigger; переход если a больше
		jb b_bigger; переход если b больше

		; -2 * b
		fld qword ptr[b]; st(0) = b
		fmul t1; st(0) = -2 * b
		jmp endcalc

		a_bigger : ; (2 * a - 10) / b
		fld qword ptr[a]; st(0) = a
		fmul t2; st(0) = 2 * a
		fadd t3; st(0) = 2 * a - 10

		fld qword ptr[b]; st(0) = b | st(1) = 2 * a - 10
		ftst; сравнение b с 0
		fnstsw ax;  записываем значения SWR в AX
		sahf;  записываем содержимое AH в младший байт EFLAGS
		je division_by_zero
		fdivp st(1), st; st(0) = 2 * a - 10 / b
		jmp endcalc

		b_bigger : ; 2 - (3 * a - b) / (2 * a)

		fld qword ptr[t2]; st(0) = 2
		fld qword ptr[a]; st(0) = a | st(1) = 2
		fmul t4; st(0) = 3 * a | st(1) = 2
		fld qword ptr[b]; st(0) = b | st(1) = 3 * a | st(2) = 2
		fsubp st(1), st; st(0) = 3 * a - b | st(1) = 2

		fld qword ptr[a]; st(0) = a | st(1) = 3 * a - b | st(2) = 2
		ftst; сравнение a с 0
		fnstsw ax;  записываем значения SWR в AX
		sahf;  записываем содержимое AH в младший байт EFLAGS
		je division_by_zero
		fmul t2; st(0) = 2 * a

		fdivp st(1), st; st(0) = (3 * a - b) / (2 * a) | st(1) = 2

		fsubp st(1), st; st(0) = 2 - (3 * a - b)
		jmp endcalc

		division_by_zero : ;
		mov err, 1; формируем результат ошибки
			jmp endcalc

			endcalc :
		fst res;
	}
}

// --- 6
double func(double a, double b)
{
	double res; int status; double t1 = -2; double t2 = 2; double t3 = -10; double t4 = 3;
	_asm {		
		finit; команда инициализации сопроцессора
		fld qword ptr[b]; st(0) = b
		fld qword ptr[a]; st(0) = a | st(1) = b
		fcompp; сравниваем a и b
		fstsw status; сохраняем регистр флагов сопроцессора
		mov ah, byte ptr[status + 1]
		sahf; записываем в регистр флагов процессора
		ja a_bigger; переход если a больше
		jb b_bigger; переход если b больше

		; -2 * b
		fld qword ptr[b]; st(0) = b
		fmul t1; st(0) = -2 * b
		jmp endcalc

		a_bigger : ; (2 * a - 10) / b
		fld qword ptr[a] ; st(0) = a
		fmul t2; st(0) = 2 * a
		fadd t3; st(0) = 2 * a - 10

		fld qword ptr[b]; st(0) = b | st(1) = 2 * a - 10
		ftst; сравнение b с 0
		fnstsw ax;  записываем значения SWR в AX
		sahf;  записываем содержимое AH в младший байт EFLAGS
		je division_by_zero
		fdivp st(1), st; st(0) = 2 * a - 10 / b
		jmp endcalc

		b_bigger: ; 2 - (3 * a - b) / (2 * a)
		
		fld qword ptr[t2]; st(0) = 2 
		fld qword ptr[a]; st(0) = a | st(1) = 2 
		fmul t4; st(0) = 3 * a | st(1) = 2
		fld qword ptr[b]; st(0) = b | st(1) = 3 * a | st(2) = 2
		fsubp st(1), st; st(0) = 3 * a - b | st(1) = 2
				
		fld qword ptr[a]; st(0) = a | st(1) = 3 * a - b | st(2) = 2
		ftst; сравнение a с 0
		fnstsw ax;  записываем значения SWR в AX
		sahf;  записываем содержимое AH в младший байт EFLAGS
		je division_by_zero
		fmul t2; st(0) = 2 * a

		fdivp st(1), st; st(0) = (3 * a - b) / (2 * a) | st(1) = 2

		fsubp st(1), st; st(0) = 2 - (3 * a - b)
		jmp endcalc

		division_by_zero: ;
		mov err, 1; формируем результат ошибки
		jmp endcalc

		endcalc: 
		fst res;
	}
}

int main() {
	
	double a, b, res1;
	cout << "Enter a:" << endl;
	cin >> a;
	cout << "Enter b:" << endl;
	cin >> b;
	double res = func(a, b);

	// -- результат на ассемблере
	if (err) {
		cout << "Division by zero error" << endl;
		return 1;
	}
	cout << "ASM: " << res << endl;

	// -- проверяем результат на плюсах
	if (a < b) {
		res1 = 2 - (3 * a - b) / (2 * a);
	}
	else if (a == b) {
		res1 = -2 * b;
	}
	else {
		res1 = (2 * a - 10) / b;
	}
	cout << "C++: " << res1 << endl;

	return 0;
}