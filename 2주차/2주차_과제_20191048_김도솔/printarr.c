#include "ForStarWars.h"

// array 출력하는 함수
void printarr(int* arr) {
	for (int i = 0; i < 10; i++) { // index 0-9까지
		printf("%d ", arr[i]); // 각 숫자 개수 출력
	}
	printf("\n"); // 줄바꿈 문자 삽입
}
