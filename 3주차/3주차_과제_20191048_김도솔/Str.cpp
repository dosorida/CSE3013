#include <cstring> // string.h의 멤버함수를 이용하기 위해 헤더파일 추가
#include "Str.h" // 우리가 만든 헤더파일 추가

// string형 변수 Str의 멤버함수 구현
Str::Str(int leng) { // 생성자, leng은 string의 길이
	str = new char[leng + 1]; // 문자열 길이만큼 메모리 할당
	len = leng; // 문자열 길이 저장
	str[0] = '\0'; // NULL 문자 삽입
}

Str::Str(char *neyong) { // 생성자, neyong은 초기화할 내용이 들어감
	len = strlen(neyong); // 문자열 길이 저장
	str = new char[len + 1]; // 문자열 길이만큼 메모리 할당
	strcpy(str, neyong); // 전달받은 내용으로 문자열 초기화
	str[len] = '\0'; // 문자열 끝에 NULL 문자 삽입
}

Str::~Str() { // 소멸자
	delete[] str; // 메모리 할당 해제
}

int Str::length(void) { // string의 길이를 리턴하는 함수
	return len;
}

char* Str::contents(void) { // string의 내용을 리턴하는 함수
	return str;
}

int Str::compare(class Str& a) { // a의 내용과 strcmp
	return strcmp(str, a.contents());
}

int Str::compare(char *a) { // a의 내용과 strcmp
	return strcmp(str, a);
}

void Str::operator=(char *a) { // string의 값을 대입
	delete[] this->str; // 메모리 할당 해제
	
	if (a == 0) { // a가 NULL 포인터일 경우
		str = new char[1]; // 메모리 할당
		str[0] = '\0'; // NULL 문자만 삽입
		len = 0; // 길이 0으로 설정
	}
	else { // a가 NULL 포인터가 아닐 경우
		len = strlen(a); // a의 문자열 길이 저장
		str = new char[len + 1]; // 메모리 할당
		strcpy(str, a); // a의 내용 대입
		str[len] = '\0'; // 문자열 끝에 NULL 문자 삽입
	}
}

void Str::operator=(class Str& a) { // Str의 내용을 대입
	delete[] this->str; // 메모리 할당 해제
	len = a.length(); // a의 문자열 길이 저장
	str = new char[len + 1]; // 메모리 할당
	strcpy(str, a.contents()); // a의 내용 대입
	str[len] = '\0'; // 문자열 끝에 NULL 문자 삽입
}
