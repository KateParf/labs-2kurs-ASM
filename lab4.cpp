
#include<iostream>
using namespace std;

int main() {
	int n, b, d, resAsm, resCpp;
	
	cout << "Enter size of array:" << endl;
	cin >> n;
	while (n < 0) {
		cout << "Wrong size walue" << endl;
		cout << "Enter size of array:" << endl;
		cin >> n;
	}

	int* arr = new int[n];
	for (int i = 0; i < n; i++) {
		cout << "Enter " << i+1 << " elem of array:" << endl;
		cin >> arr[i];
	}

	cout << "Enter b:" << endl;
	cin >> b;
	cout << "Enter d:" << endl;
	cin >> d;


	// Найти количество положительных элементов массива A = { a[i] }, которые удовлетворяют условию : b <= a[i] <= d
	_asm {
		xor eax, eax; очищаем регистр
		xor esi, esi; очищаем регистр индекса в массиве
		xor edi, edi; очищаем регистр счётчика количества элементов

		mov ebx, arr; ebx указывает на начало массива
		mov ecx, n; счётчик цикла по всем элементам массива
		jecxz quit; завершить если длина массива 0 (длина лежит в ecx)

		begin_loop:
			mov eax, [ebx + esi * 4]; определяем текущий элемент

			cmp eax, 0; проверка что a[i] - положительное число
			jle end_loop; если <= , то завершаем цикл

			mov edx, d; <edx> = d для сравнения элемента с d
			cmp eax, edx; сравниваем a[i] и d
			jg end_loop; если > , то завершаем цикл

			mov edx, b; <edx> = b для сравнения элемента с b
			cmp eax, edx; сравниваем a[i] и b
			jl end_loop; если < , то завершаем цикл

			inc edi; элемент удовлетворяет условию, увеличиваем счётчик <edi> +=1

		end_loop:
			inc esi; <esi> +=1, переходим к следующему элементу
			loop begin_loop; повторяем цикл для всех элементов массива

		quit: mov resAsm, edi; <resAsm> = edi = количество удовлетворяющих элементов

	}

	// -- результат на ассемблере
	cout << "ASM: " << resAsm << endl;

	// -- проверяем результат на плюсах
	resCpp = 0;
	for (int i = 0; i < n; i++) {
		if (arr[i] >= 0 && arr[i] <= d && arr[i] >= b)
			resCpp++;
	}
	cout << "C++: " << resCpp << endl;

	return 0;
}
