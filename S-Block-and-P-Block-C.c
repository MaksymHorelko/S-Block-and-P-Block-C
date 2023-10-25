#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Розмір масиву за умовою
#define SIZE 2

// Кількість можливих ключів для 4ьох бітного числа
#define numKeys 16

/* Допоміжні функції */

// Функція друку числа в бінарному виді
void printBinary(unsigned char x) {
	for (int i = 0; i < 8; i++) {
		printf("%c", (x & 0x80) ? '1' : '0');
		x <<= 1;
	}
	printf("\n");
}

// Функція розбивки 8бітного числа на дві тетради
uint8_t* split(uint8_t x) {
	uint8_t *splitedX = (uint8_t*) malloc(SIZE * sizeof(uint8_t));

	// Розбивка 8 бітного числа на два блоки
	splitedX[1] = x & 0b00001111;
	splitedX[0] = x & 0b11110000;

	splitedX[0] >>= 4;

	return splitedX;
}

// Функція сполучення двох блоків
uint8_t concatenateSplitedByte(uint8_t *array) {
	return (array[0] << 4) | array[1];
}

/* Нижче функції для P-блоку */

// Функція перестановки двох бітів (першого та передостаннього)
uint8_t swapTwoBits(uint8_t x) {
	// Перестановка першогго біта та передостаннього
	size_t bitPosition1 = 1; // Позиція першого біта для обміну
	size_t bitPosition2 = 3; // Позиція другого біта для обміну

	// Знаходимо значення бітів за позицією
	uint8_t bit1 = (x >> bitPosition1) & 1;
	uint8_t bit2 = (x >> bitPosition2) & 1;

	// Встановлюємо нові значення бітів
	x &= ~(1 << bitPosition1);
	x |= (bit2 << bitPosition1);

	x &= ~(1 << bitPosition2);
	x |= (bit1 << bitPosition2);

	return x;
}

// Функція перестановки всіх бітів
uint8_t swapAllBits(uint8_t x) {
	for (size_t i = 0; i < 4 / 2; i++) {
		size_t bitPosition1 = i; // Позиція першого біта для обміну
		size_t bitPosition2 = 4 - i - 1; // Позиція другого біта для обміну

		// Знаходимо значення бітів за позицією
		uint8_t bit1 = (x >> bitPosition1) & 1;
		uint8_t bit2 = (x >> bitPosition2) & 1;

		// Встановлюємо нові значення бітів
		x &= ~(1 << bitPosition1);
		x |= (bit2 << bitPosition1);

		x &= ~(1 << bitPosition2);
		x |= (bit1 << bitPosition2);
	}
	return x;
}

// Функція перестановки
uint8_t f(uint8_t x) {

	x = swapAllBits(x);

	x = swapTwoBits(x);

	return x;
}

// P-блок заміни
uint8_t pBlock(uint8_t x) {

	// Заміна за допомогою функції
	x = f(x);

	return x;
}

// Функція відновлення для P-блоку
uint8_t invPBlock(uint8_t x) {

	// Заміна за допомогою функції
	x = swapTwoBits(x);
	x = swapAllBits(x);

	return x;
}

/* Нижче функції для S-блоку */

// Функція ініціалізації таблиці
uint8_t* initStaticTable() {
	// Створена табличка
	static uint8_t table[] = {
			0b1010, 0b1011, 0b0110, 0b1000,
			0b0111, 0b1101, 0b0001, 0b0101,
			0b1111, 0b0010, 0b0000, 0b1001,
			0b0011, 0b0100, 0b1100, 0b1110
	};

	uint8_t *pointer = table;
	return pointer;
}

// Функція створення ключів для таблиці
uint8_t* createKeysForTable() {
	uint8_t *key = (uint8_t*) malloc(numKeys * sizeof(uint8_t));
	uint8_t k = 0b0000;
	for (size_t i = 0; i < numKeys; i++) {
		key[i] = k;
		k += 0b0001;
	}
	return key;
}

// Функція знаходження значення в таблиці за допомогою відповідного ключа
uint8_t findBitsByKeyInTable(uint8_t x, uint8_t *table, uint8_t *key) {
	for (size_t i = 0; i < numKeys; i++) {
		if (key[i] == x) {
			return table[i];
		}
	}
	return 0;
}

// S-блок перестановки
uint8_t sBlock(uint8_t x) {

	uint8_t *splitedByte = split(x);

	uint8_t *table = initStaticTable();

	uint8_t *keys = createKeysForTable();

	// Заміна значень за таблицею
	for (size_t i = 0; i < SIZE; i++) {
		splitedByte[i] = findBitsByKeyInTable(splitedByte[i], table, keys);
	}

	uint8_t result = concatenateSplitedByte(splitedByte);

	free(splitedByte);
	free(keys);

	return result;
}

// Функція відновлення для S-блоку
uint8_t invSBlock(uint8_t x) {

	uint8_t *table = initStaticTable();

	uint8_t *keys = createKeysForTable();

	uint8_t *splitedByte = split(x);

	for (size_t j = 0; j < SIZE; j++) {
		for (size_t i = 0; i < numKeys; i++) {
			if (splitedByte[j] == table[i]) {
				splitedByte[j] = keys[i];
				break;
			}
		}
	}

	uint8_t result = concatenateSplitedByte(splitedByte);

	free(splitedByte);
	free(keys);

	return result;
}

// Головна функція
int main(void) {

	uint8_t x = 0b11010001; // 209 // Вхідні дані

	/* P-BLOCK */

	printf("P-block: \n");

	printf("Data: %d\nBin: ", x);
	printBinary(x);
	printf("\n");

	uint8_t codedX = pBlock(x);
	printf("Coded by P block: %d\nBin: ", codedX);
	printBinary(codedX);
	printf("\n");

	uint8_t unCodedX = invPBlock(codedX);
	printf("Uncoded: %d\nBin: ", unCodedX);
	printBinary(unCodedX);
	printf("\n");

	/* S-BLOCK */

	printf("S-block: \n");

	printf("Data: %d\nBin: ", x);
	printBinary(x);
	printf("\n");

	codedX = sBlock(x);
	printf("Coded by S block: %d\nBin: ", codedX);
	printBinary(codedX);
	printf("\n");

	unCodedX = invSBlock(codedX);
	printf("Uncoded: %d\nBin: ", unCodedX);
	printBinary(unCodedX);
	printf("\n");

	getchar();

	return EXIT_SUCCESS;
}
