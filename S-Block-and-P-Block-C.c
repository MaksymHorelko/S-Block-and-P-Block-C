#include <stdio.h>
#include <stdlib.h>

// Розмір масиву за умовою
#define SIZE 2

/* Нижче функції для P-блоку */

int swapTwoBits(int x, int bits) {
	// Перестановка першогго біта та передостаннього
	size_t bitPosition1 = 1; // Позиція першого біта для обміну
	size_t bitPosition2 = bits - 1; // Позиція другого біта для обміну

	// Знаходимо значення бітів за позицією
	int bit1 = (x >> bitPosition1) & 1;
	int bit2 = (x >> bitPosition2) & 1;

	// Встановлюємо нові значення бітів
	x &= ~(1 << bitPosition1);
	x |= (bit2 << bitPosition1);

	x &= ~(1 << bitPosition2);
	x |= (bit1 << bitPosition2);

	return x;
}

int swapAllBits(int x, int bits) {
	for (size_t i = 0; i < bits / 2; i++) {
		size_t bitPosition1 = i; // Позиція першого біта для обміну
		size_t bitPosition2 = bits - i - 1; // Позиція другого біта для обміну

		// Знаходимо значення бітів за позицією
		int bit1 = (x >> bitPosition1) & 1;
		int bit2 = (x >> bitPosition2) & 1;

		// Встановлюємо нові значення бітів
		x &= ~(1 << bitPosition1);
		x |= (bit2 << bitPosition1);

		x &= ~(1 << bitPosition2);
		x |= (bit1 << bitPosition2);
	}
	return x;
}

int countBits(int x) {
	int bits = 4;
	if (x >= 0b00010000) {
		bits = 8;
	}
	return bits;
}

// Функція перестановки
int f(int x) {
	int bits = countBits(x);

	x = swapAllBits(x, bits);

	x = swapTwoBits(x, bits);

	return x;
}

// Функція розбивки 8бітного числа на дві тетради
int* split(int x) {
	int *splitedX = (int*) malloc(SIZE * sizeof(int));

	// Розбивка 8 бітного числа на два блоки
	splitedX[0] = x & 0b00001111;
	splitedX[1] = x & 0b11110000;

	return splitedX;
}

// P-блок заміни
int pBlock(int x) {
	// Маска
	x &= 0b11111111;

	int *arr = split(x);

	// Заміна за допомогою функції
	for (size_t i = 0; i < SIZE; i++) {
		arr[i] = f(arr[i]);
	}
	x = 0b00000000;
	x += arr[1];
	x += arr[0];
	return x;
}

// Функція відновлення для P-блоку
int invPBlock(int x) {
	x &= 0b11111111;

	int *arr = split(x);

	int bits = countBits(x);

	// Заміна за допомогою функції
	for (size_t i = 0; i < SIZE; i++) {
		arr[i] = swapTwoBits(arr[i], bits);
		arr[i] = swapAllBits(arr[i], bits);

	}
	x = 0b00000000;
	x += arr[1];
	x += arr[0];

	return x;
}

/* Нижче функції для S-блоку */

// S-блок перестановки
void sBlock() {

}

// Функція відновлення для S-блоку
void invSBlock() {

}

// Довільний алгоритм 'шифруванння'
int algorithm() {

	return 1;
}

void printBinary(int x) {
	if (x > 1) {
		printBinary(x / 2);
	}
	printf("%d", x % 2);
}

// Головна функція
int main(void) {
	int x = 0b11010001; // 209
	printf("Data: %d\nBin: ", x);
	printBinary(x);
	printf("\n\n");

	int codedX = pBlock(x);
	printf("Coded: %d\nBin: ", codedX);
	printBinary(codedX);
	printf("\n\n");

	int unCodedX = invPBlock(codedX);
	printf("Uncoded: %d\nBin: ", unCodedX);
	printBinary(unCodedX);
	printf("\n\n");

	return EXIT_SUCCESS;
}
