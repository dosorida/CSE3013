#include "ForStarWars.h"

// 메인 함수
void main(void) {
	int t_num = 0; // 테스트 케이스의 개수를 받을 변수
	int page = 0; // 페이지 수를 받을 변수
	int arr[10]; // 각 페이지를 이루는 숫자(0-9)의 개수를 세서 저장할 array
	scanf("%d", &t_num); // 테스트 케이스 개수 먼저 받기

	for (int i = 0; i < t_num; i++) { // 테스트 케이스 수만큼 반복
		scanf("%d", &page); // 페이지 수 받기
		initarr(arr); // array 0으로 초기화 하고
		count(arr, page); // array에 각 숫자들의 개수 세서
		printarr(arr); // 한 줄에 출력
	}
}
