#include <iostream> // 표준 입출력 함수를 사용하기 위해 <iostream> 헤더파일 추가
#include "Str.h" // 우리가 작성한 Str class를 사용하기 위해 Str.h 헤더파일 추가

using namespace std; // std 이름공간 지정

int main(void) {
	Str a("I'm a girl"); // "I'm a girl"로 초기화
	cout << a.contents(); // string 내용 리턴 
	a = "I'm a boy\n"; // "I'm a boy" 대입
	cout << a.contents(); // string 내용 리턴
	cout << a.compare("I'm a a") << endl; // a의 내용과 "I'm a a"의 내용 strcmp
	return 0;
}
