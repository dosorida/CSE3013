#include "ForStarWars.h"

// 0으로 초기화된 array와 page 수가 주어지면 페이지를 이루는 숫자들(0-9)의 개수를 세서array에 담아주는 함수
void count(int* arr, int page) {
	int tmp = 0; // 1-N장까지 각 페이지 수를 담을 변수 tmp 선언
	for (int i = 1; i <= page; i++) {
		tmp = i; // 1장부터 시작
		while (tmp != 0) { // tmp가 0이 되기 전까지 반복
			int n = tmp % 10; // tmp의 가장 오른쪽 자리 수 구해서
			arr[n] += 1; // 그 숫자에 해당하는 index의 값 +1
			tmp /= 10; // 10으로 나눠서 다음 자리 숫자 구할 준비
		}
	}
}	
