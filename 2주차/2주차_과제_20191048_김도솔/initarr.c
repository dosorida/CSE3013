#include "ForStarWars.h"

// 크기 10의 array가 주어지면 모든 값을 0으로 초기화 하는 함수
void initarr(int* arr) {
	for (int i = 0; i < 10; i++) { // index 0-9까지
		arr[i] = 0; // 0으로 초기화
	}
}
