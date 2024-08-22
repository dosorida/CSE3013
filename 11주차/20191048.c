#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n, m; // 미로의 너비와 높이
int width, height; // 미로 array의 가로, 세로
char **maze; // 미로 array
int setnum = 1;

void createMaze();
void printMaze();
void freeMaze(char**);

int main(void) {
    // 미로의 너비와 높이 입력받기
    scanf("%d", &n);
    rewind(stdout);
    scanf("%d", &m);

    // 미로 array의 가로, 세로 구하기
    width = 2 * n + 1;
	height = 2 * m + 1;
    
    srand((unsigned int)time(NULL));

    createMaze();
	printMaze();
    freeMaze(maze);

    return 0;
}

void createMaze() {
    // maze 메모리 할당
    maze = (char**)malloc(sizeof(char*) * height);
    for (int i = 0; i < height; i++) {
        maze[i] = (char*)malloc(sizeof(char) * width);
    }

    // 미로 전체 setting
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((i % 2 == 0) && (j % 2 == 0)) {
                maze[i][j] = '+';
            }
            else if (i % 2 == 0) {
                maze[i][j] = '-';
            }
            else if (j % 2 == 0) {
                maze[i][j] = '|';
            }
            else {
                maze[i][j] = ' ';
            }
        }
    }

    // 1. 미로의 첫 번째 줄 초기화
    for (int j = 1; j < width; j += 2) {
		maze[1][j] = setnum;
        setnum++;
	}

    // 5. 마지막 줄에 도달할 때까지 2 ~ 4 반복
    for (int i = 1; i < height - 2; i += 2) {

        // 2. 현재 행에서 벽 제거
        int rnum = 0;
        for (int j = 1; j < width - 2; j += 2) { // 첫 번째 방 ~ 마지막 방까지 집합 비교
            // 인접한 두 방이 서로 다른 집합에 속해 있다면
            if (maze[i][j] != maze[i][j + 2]) {
                // 두 방 사이의 벽을 남겨둘지 제거할지 임의로 선택
                rnum = rand() % 2;
                if (rnum == 0) continue;
                // 벽을 제거하면 같은 집합으로 합치기
                else {
                    maze[i][j + 1] = ' ';
                    char update = maze[i][j + 2];
                    maze[i][j + 2] = maze[i][j];
                    for (int k = 1; k < i + 1; k += 2) {
                        for (int l = 1; l < width; l += 2) {
                            if(maze[k][l] == update) {
                                maze[k][l] = maze[i][j];
                            }
                        }
                    }
                }
            }
        }
        
        // 3. 현재 행과 다음 행 사이의 벽 제거
        int rflag = 0;
        for (int j = 1; j < width; j += 2) {

            // 마지막 방인 경우
            if (j == width - 2) {
                // 왼쪽 방과 같은 집합에 속하는 경우
                if(maze[i][j] == maze[i][j - 2]) {
                    // rflag가 0이면 벽 제거
                    if (rflag == 0) {
                        maze[i + 1][j] = ' ';
                        maze[i + 2][j] = maze[i][j];
                    }
                    // rflag가 1이면 임의로 벽 제거
                    else {
                        rnum = rand() % 2;
                        if (rnum == 1) {
                             maze[i + 1][j] = ' ';
                            maze[i + 2][j] = maze[i][j];
                        }
                    }
                }
                // 왼쪽 방과 다른 집합에 속하는 경우 벽 제거
                else {
                    maze[i + 1][j] = ' ';
                    maze[i + 2][j] = maze[i][j];
                }
                break;
            }

            // 오른쪽 방과 같은 집합에 속하는 경우
            if(maze[i][j] == maze[i][j + 2]) {
                // 하단의 벽을 남겨둘지 제거할지 임의로 선택
                rnum = rand() % 2;
                if (rnum == 1) {
                    maze[i + 1][j] = ' ';
                    maze[i + 2][j] = maze[i][j];
                    rflag = 1;
                }

            }

            // 오른쪽 방과 같은 집합에 속하지 않는 경우
            else {
                // 첫 번째 방일 경우 벽 제거
                if (j == 1) {
                    maze[i + 1][j] = ' ';
                    maze[i + 2][j] = maze[i][j];
                }
                // 첫 번째 방이 아니고 왼쪽 방과 같은 집합에 속할 경우
                else if(maze[i][j] == maze[i][j - 2]) {
                    // rflag가 0이면 벽 제거
                    if (rflag == 0) {
                        maze[i + 1][j] = ' ';
                        maze[i + 2][j] = maze[i][j];
                    }
                    // rflag가 1이면 임의로 벽 제거
                    else {
                        rnum = rand() % 2;
                        if (rnum == 1) {
                            maze[i + 1][j] = ' ';
                            maze[i + 2][j] = maze[i][j];
                            rflag = 1;
                        }
                    }
                    rflag = 0;
                }
                // 첫 번째 방이 아니면서 왼쪽 방과 같은 집합에 속하지 않는 경우
                else {
                    maze[i + 1][j] = ' ';
                    maze[i + 2][j] = maze[i][j];
                }
            }
        }
        
        // 4. 3에서 완성하지 않은 다음 줄의 나머지 방 구체화
        for (int j = 1; j < width; j += 2) {
			if (maze[i + 2][j] == ' ') {
				maze[i + 2][j] = setnum;
                setnum++;
            }
		}
    }
    
    // 6. 마지막 줄 벽 제거
    int i = height - 2;
    for (int j = 1; j < width - 2; j += 2) {
        if (maze[i][j] != maze[i][j + 2]) {
            maze[i][j + 1] = ' ';
            char update = maze[i][j + 2];
            maze[i][j + 2] = maze[i][j];
            for (int k = 1; k < height; k += 2) {
                for (int l = 1; l < width; l += 2) {
                    if(maze[k][l] == update) {
                        maze[k][l] = maze[i][j];
                    }
                }
            }
        }
    }
}

void printMaze() {
	FILE *fp = fopen("maze.maz", "w");
	for (int i = 0; i < height; i++) {
		for (int j = 0 ;j < width; j++) {
			if ((i % 2 == 1) && (j % 2 == 1))
				fprintf(fp, " ");
			else
				fprintf(fp, "%c", maze[i][j]);
		}
		fprintf(fp, "\n");
	}
    fclose(fp);
}

void freeMaze(char** maze) {
    // maze 메모리 할당 해제
    for (int i = 0; i < height; i++) {
        free(maze[i]);
    }
    free(maze);
}