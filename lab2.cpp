
#include<iostream>
using namespace std;

int main()
{
	int a, b, err, res1, res2;
	cout << "Enter a:" << endl;
	cin >> a;
	cout << "Enter b:" << endl;
	cin >> b;


	int t1, t2, t3;
	t1 = -2, t2 = 2, t3 = 3;

	_asm
	{
		xor eax, eax; очищаем регистр
		mov ecx, a; < eсx >= a
		mov ebx, b; < ebx >= b
		cmp ecx, ebx; сравнение a и b

		jg l_bigger; переход если a > b
		jl l_smaller; переход если a < b
		je l_equal; переход если a = b

		l_equal :
		mov eax, b; < eax >= b
		imul t1; <eax> = > <edx:eax> = -2 * b
		jo error_overflow
		jmp quit; переход на конец программы

		l_bigger :
		or ebx, ebx; сравнение b и 0
			jz error_zero; ошибка деление на ноль

			mov eax, ecx; < eax >= a
			imul t2; <eax> = > <edx:eax> = 2 * a
			jo error_overflow
			sub eax, 10; <eax> = 2 * a - 10
			jo error_overflow

			cdq; подготовка деления <edx:eax> = 2 * a - 10
			idiv ebx; <eax> = 2 * a - 10 / b
			jmp quit; переход на конец программы

			l_smaller :
		or ecx, ecx; сравнение a и 0
			jz error_zero; ошибка деление на ноль

			mov eax, ecx; < eax >= a
			imul t3; <edx:eax> = 3a
			jo error_overflow
			sub eax, ebx; <eax> = 3 * a  - b
			jo error_overflow
			push eax; в стеке 3 * a  - b
			mov eax, ecx; < eax >= a
			imul t2; <edx:eax> = 2a
			jo error_overflow
			mov ecx, eax; < ecx >= 2a
			pop eax; < eax > = 3 * a  - b
			cdq; подготовка деления <edx:eax> = 3 * a  - b
			idiv ecx; <eax> = 3 * a  - b / 2a
			mov ecx, eax; <ecx> = (3 * a  - b) / 2a
			mov eax, t2; <eax> = 2
			sub eax, ecx; <eax> = 2 - (3 * a  - b) / 2a
			jo error_overflow
			jmp quit

			error_zero :
		mov err, 1
			jmp quit
			error_overflow :
		mov err, 2
			jmp quit
			quit :
		mov res2, eax
	}



	if (err == 1)
		cout << "Zero div error" << endl;
	else if (err == 2)
		cout << "Overflow error" << endl;
	else {
		cout << "ASM: " << res2 << endl;

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
	}


	return 0;
}
