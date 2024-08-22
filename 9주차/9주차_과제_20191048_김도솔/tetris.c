﻿#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	createRankList();

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		case MENU_RANK: rank(); break;
		default: break;
		}
	}

	endwin();
	system("clear");
	freeRank();
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	for (int i = 0; i < BLOCK_NUM; i++){
		nextBlock[i]=rand()%7;
	}

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();

	DrawBlockWithFeatures(blockY,blockX, nextBlock[0], blockRotate);
	
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	// 1주차 과제 수정
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	// move(9,WIDTH+10);
	move(15,WIDTH+10);
	printw("SCORE");
	// DrawBox(10,WIDTH+10,1,8);
	DrawBox(16,WIDTH+10,1,8);

	move(HEIGHT,WIDTH+10);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
	move(HEIGHT,WIDTH+10);
}


void PrintScore(int score){
	//move(11,WIDTH+11);
	move((6 * BLOCK_NUM) - 1, WIDTH + 11);
	printw("%8d",score);
	move(HEIGHT,WIDTH+10);
}

void DrawNextBlock(int *nextBlock){
	for (int k = 1; k < BLOCK_NUM; k++) {
		int i, j;
		for( i = 0; i < 4; i++ ){
			move(-2 + i + (k * 6),WIDTH+13);
			for( j = 0; j < 4; j++ ){
				if( block[nextBlock[k]][0][i][j] == 1 ){
					attron(A_REVERSE);
					printw(" ");
					attroff(A_REVERSE);
				}
				else printw(" ");
			}
		}
	}
	move(HEIGHT,WIDTH+10);
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
	move(HEIGHT,WIDTH+10);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				int x = blockX + j;
				int y = blockY + i;
				if (y >= HEIGHT) return 0;
				if (x < 0) return 0;
				if (x >= WIDTH) return 0;
				if (field[y][x] == 1) return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int preBlock = currentBlock;
	int preBlockRotate = blockRotate;
	int preBlockY = blockY;
	int preBlockX = blockX;
	int preShadowY = blockY;

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	switch (command) {
	case KEY_UP:
		preBlockRotate = (blockRotate + 3) % 4;
		break;
	case KEY_DOWN:
		preBlockY--;
		break;
	case KEY_RIGHT:
		preBlockX--;
		break;
	case KEY_LEFT:
		preBlockX++;
		break;
	default:
		break;
	}

	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[preBlock][preBlockRotate][i][j] == 1 && i + preBlockY >= 0) {
				move(i + preBlockY + 1, j + preBlockX + 1);
				printw(".");
			}
		}
	}

	for(; preShadowY < HEIGHT; preShadowY++) {
        if(CheckToMove(field, preBlock, preBlockRotate, preShadowY + 1, preBlockX) == 0) break;
    }

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[preBlock][preBlockRotate][i][j] == 1 && i + preShadowY >= 0) {
				move(i + preShadowY + 1, j + preBlockX + 1);
				printw(".");
			}
		}
	}

	move(HEIGHT, WIDTH + 10);

	//3. 새로운 블록 정보를 그린다.
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig){
	// user code
	//강의자료 p26-27의 플로우차트를 참고한다.
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
        score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		if (blockY == -1) {
			gameOver = 1;
		}
		score += DeleteLine(field);
		
		for (int i = 0; i < BLOCK_NUM - 1; i++) {
			nextBlock[i] = nextBlock[i + 1];
		}
		nextBlock[BLOCK_NUM - 1] = rand() % 7;
		DrawNextBlock(nextBlock);
		
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH / 2 - 2;
		PrintScore(score);
		DrawField();
	}
	timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	//Block이 추가된 영역의 필드값을 바꾼다.
	int touched = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				int y = blockY + i;
				int x = blockX + j;

				if (y >= HEIGHT) continue;;
				if (x < 0) continue;
				if (x >= WIDTH) continue;

				field[y][x] = 1;

				if(f[y + 1][x] || y + 1 == HEIGHT) touched++;
			}
		}
	}
	int score = touched * 10;
	return score;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	int line_num = 0;
	int score = 0;
	for (int i = 0; i < HEIGHT; i++) {
		int flag = 1;
		for (int j = 0; j < WIDTH; j++) {
			if (f[i][j] == 0) flag = 0;
		}
		//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
		if (flag == 1) {
			line_num++;
			for (int y = i; y > 0; y--) {
				for (int x = 0; x < WIDTH; x++) {
					f[y][x] = f[y - 1][x];
				}
			}
			for (int x = 0; x < WIDTH; x++) {
				f[0][x] = 0;
			}
			i--;
		}
	}
	score = 100 * (line_num) * (line_num);
	return score;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
    for(; y < HEIGHT; y++) {
        if(CheckToMove(field, nextBlock[0], blockRotate, y + 1, x) == 0) break;
    }
    DrawBlock(y, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
}

void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	FILE *fp;
	int i, j;
	char str[NAMELEN + 1];
	int num;
	int read;
	int index = 0;
	score_number = 0;

	// 1. "rank.txt"열기
	fp = fopen("rank.txt", "r");

	// 2. 파일에서 랭킹 정보 읽어오기
	// 3. LinkedList로 저장

	fscanf(fp, "%d", &head->length); // 랭킹 수 읽어오기

	fscanf(fp, "%s %d", str, &num); // 첫 번째 랭킹 정보 읽어오기
	Node* first;
	first = malloc(sizeof(Node));
	strcpy(first->name, str);
	first->score = num;
	first->link = NULL;
	head->first = first;
	score_number++;

	for (int i = 0; i < head->length - 1; i++) {
		// 순서대로 랭킹 정보 읽어오기
		read = fscanf(fp, "%s %d", str, &num);

		// 새로운 노드 생성
		Node* rank;
		rank = malloc(sizeof(Node));
		strcpy(rank->name, str);
		rank->score = num;
		rank->link = NULL;

		// 새로운 노드 리스트에 연결
		Node* curr = head->first;
		for (int j = 0; j < i; j++) {
			curr = curr->link;
		}
		curr->link = rank;
		score_number++;
	}

	score_number = head->length;

	// 4. 파일 닫기
	fclose(fp);
}

void rank() {
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X = 1, Y=score_number, ch, i, j;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
		int scan = 0;

		// X 입력받기
		printw("X: ");
		echo();
		scan = scanw("%d", &X);
		if (scan == -1) X = 1;
		noecho();

		// Y 입력받기
		printw("Y: ");
		echo();
		scan = scanw("%d", &Y);
		if (scan == -1) Y = score_number;
		noecho();

		// 랭크 목록 시작
		printw("       name       |   score   \n");
		printw("------------------------------\n");

		// 적절한 input인지 확인
		if (X > Y) {
			printf("search failure: no rank in the list\n");
		}
		
		// X와 Y사이의 rank 출력
		else {
			int index = 0;
			Node* curr;
			curr = head->first;
			int space;
			while(curr) {
				if ((index + 1 >= X)&&(index + 1 <= Y)) {
					printw(" %s", curr->name);
					space = 17 - strlen(curr->name);
					for (int i = 0; i < space; i++) {
						printw(" ");
					}
					printw("|");
					printw(" %d\n", curr->score);
				}
				index++;
				curr = curr->link;
			}
		}
		
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;

		// 사용자의 이름(문자열) 입력받기
		printw("input the name: ");
		echo();
		scanw("%s", str);
		noecho();

		// 랭크 목록 시작
		printw("       name       |   score   \n");
		printw("------------------------------\n");

		// 저장된 이름과 비교하기
		Node* curr;
		curr = head->first;
		int space;
		while(curr) {
			if (strcmp(str, curr->name) == 0) {
				printw(" %s", curr->name);
				space = 17 - strlen(curr->name);
				for (int i = 0; i < space; i++) {
					printw(" ");
				}
				printw("|");
				printw(" %d\n", curr->score);
				check = 1;
			}
			curr = curr->link;
		}

		// 일치하는 사용자가 없으면 에러 출력
		if (check == 0) {
			printw("\nsearch failure: no name in the list\n");
		}
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int X;

		// rank 번호 입력받기
		printw("input the rank: ");
		echo();
		scanw("%d", &X);
		noecho();

		// 입력받은 랭킹이 존재하는지 확인
		if ((X > score_number) || (X < 0)) {
			printw("\nsearch failure: the rank not in the list\n");
		}

		// 해당 랭킹이 범위를 벗어나지 않으면 노드를 찾아 삭제
		else {
			int count = 0;
			Node* curr = head->first;
			Node* pre = curr;

			// 첫 번째 랭킹을 삭제하는 경우
			if (X == 1) {
				head->first = curr->link;
				free(curr);
				printw("\nresult: the rank deleted\n");
				score_number--;
			}
			// 두 번째 ~ 마지막 랭킹을 삭제하는 경우
			else {
				while (curr) {
					count++;
					if(count == X) {
						pre->link = curr->link;
						free(curr);
						printw("\nresult: the rank deleted\n");
						score_number--;
						break;
					}
					pre = curr;
					curr = curr->link;
				}
			}
			head->length = score_number;
		}
		writeRankFile();
	}
	getch();
}


void writeRankFile() {
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	Node* curr;

	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp, "%d\n", score_number);

	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	curr = head->first;
	while(curr) {
		fprintf(fp, "%s %d\n", curr->name, curr->score);
		curr = curr->link;
	}

	// 파일 닫기
	fclose(fp);
}

void freeRank() {
	// 메모리 할당 해제
	Node* curr = head->first;
	Node* delete;
	while (curr) {
		delete = curr;
		free(delete);
		curr = curr->link;
	}
}

void newRank(int score) {
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	clear();

	//1. 사용자 이름을 입력받음
	printw("your name: ");
	echo();
	scanw("%s", str);
	noecho();

	//2. 새로운 노드를 생성해 이름과 점수를 저장
	Node* new;
	new = malloc(sizeof(Node));
	strcpy(new->name, str);
	new->score = score;
	new->link = NULL;

	//3. 리스트의 적절한 위치에 저장
	Node* curr = head->first;
	Node* pre = head->first;

	if (score_number == 0) {
		head->first = new;
		score_number++;
	}
	else {
		int index = 0;
		// 점수 비교해서 index 얻기
		while (curr) {
			if ((new->score) < (curr->score)) {
				curr = curr->link;
				index++;
			}
			else {
				break;
			}
		}
		// 적절한 위치에 삽입
		for (int j = 0; j < index - 1; j++) {
			pre = pre->link;
		}
		new->link = pre->link;
		pre->link = new;
		score_number++;
	}
	head->length = score_number;

	writeRankFile();
}


void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
