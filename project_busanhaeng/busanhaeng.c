#include<stdio.h>
#include<stdlib.h> // 랜덤 함수
#include<time.h> // time 함수
#include<stdbool.h>

#define LEN_MIN 15 // 기차 길이
#define LEN_MAX 50

#define STM_MIN 0 // 마동석 체력
#define STM_MAX 5

#define PROB_MIN 10 // 확률
#define PROB_MAX 90

#define AGGRO_MIN 0 // 어그로 범위
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int train_lenght; // 열차 길이
int madongseok_stamina; // 마동석의 체력
int percentile_probability; // 좀비와 시민이 움직일 확률 변수
int count = 1; // 좀비가 두턴 주기로 움직이게 할 변수
int citizen = 6; // 시민의 위치
int zombie = 3; // 좀비의 위치
int madongseok = 2; // 마동석의 위치
int arr[3][50];

// 인트로 출력 함수
void intro() {
	printf("준태를 살려라 ~~~\n\n");
}

// 성공 아웃트로 함수
void success_outtro() {
	printf("\nSUCCESS!\ncitizen(s) escaped to the next train");
}

// 실패 아웃트로 함수
void fail_outtro() {
	printf("\nfail! ㅠㅠ");
}

// train lenght 받는 조건
void input_train_lenght() {
	while (1) {
		printf("train lenght(15~50)>> ");
		scanf_s("%d", &train_lenght);

		if (train_lenght >= LEN_MIN && train_lenght <= LEN_MAX) {
			return;
		}
		else {
			printf("범위에 맞게 다시 입력하시오.\n");
		}
	}
}

// madongseok stamina를 받는 조건
void input_madongseok_stamina() {
	while (1) {
		printf("madongseok stamina(0~5)>> ");
		scanf_s("%d", &madongseok_stamina);

		if (madongseok_stamina >= STM_MIN && madongseok_stamina <= STM_MAX) {
			return;
		}
		else {
			printf("범위에 맞게 다시 입력하시오.\n");
		}
	}
}

// percentile probability 받는 조건 -> 확률
void input_percentile_probability() {
	while (1) {
		printf("percentile probability(10~90)>> ");
		scanf_s("%d", &percentile_probability);

		if (percentile_probability >= PROB_MIN && percentile_probability <= PROB_MAX) {
			return;
		}
		else {
			printf("범위에 맞게 다시 입력하시오.\n");
		}
	}
}

// 열차의 상태를 출력하는 함수
void train_state(int train_len) {
	// 첫번째 줄
	for (int i = 0; i < train_len; i++) {
		printf("#");
	}
	printf("\n");

	// 두번째줄 + 시민, 좀비, 마동석 위치
	for (int i = 0; i < train_len; i++) {
		if (i == 0 || i == train_len - 1) {
			printf("#");
		}
		// 시민
		else if (i == train_len - citizen) {
			printf("C");
		}
		// 좀비
		else if (i == train_len - zombie) {
			printf("Z");
		}
		// 마동석
		else if (i == train_len - madongseok) {
			printf("M");
		}
		else {
			printf(" ");
		}
	}
	printf("\n");

	// 세번째 줄
	for (int i = 0; i < train_len; i++) {
		printf("#");
	}
}

// 4초마다 시민 이동, 좀비 이동, 열차 출력, 시민, 좀비 상태 출력 함수
void move() {
	bool citizen_move = false; // 사람이 움직이냐 안움직이냐
	bool zombie_move = false; // 좀비가 움직이냐 안움직이냐

	// 사람의 이동 확률
	int rand_num_citizen = rand() % (PROB_MIN + PROB_MAX) + 1;
	if (percentile_probability < rand_num_citizen && rand_num_citizen <= PROB_MIN + PROB_MAX) {
		citizen_move = true;
		citizen++;
	}

	// 좀비는 두턴마다 한번 이동 기회 주어짐, 좀비의 이동 확률
	if (count % 2 != 0) {
		int rand_num_zombie = rand() % (PROB_MIN + PROB_MAX) + 1;
		if (rand_num_zombie <= percentile_probability) {
			zombie_move = true;
			zombie++;
		}
	}

	// 바뀐 위치 맵 호출
	train_state(train_lenght);

	// 줄바꿈
	printf("\n\n");

	// 시민이 움직였을 경우
	if (citizen_move == true) {
		printf("citizen: %d -> %d\n", train_lenght - citizen + 1, train_lenght - citizen);
	}
	else {
		printf("citizen: stay %d (cannot move)\n", train_lenght - citizen);
	}

	// 좀비가 움직였을 경우
	if (zombie_move == true) {
		printf("zombie: %d -> %d\n", train_lenght - zombie + 1, train_lenght - zombie);
	}
	else {
		printf("zombie: stay %d (cannot move)\n", train_lenght - zombie);
	}

	// 시민이 열차 1번 칸에 도착하여 끝날 경우 - > 승리
	if (train_lenght - citizen == 1) {
		success_outtro();
		exit(0);
	}

	// 좀비가 시민 옆에 붙어서 끝날 경우 - > 패배
	if (train_lenght - citizen + 1 == train_lenght - zombie) {
		fail_outtro();
		exit(0);
	}

	// 줄바꿈
	printf("\n\n\n");

	count++; // 좀비 이동 주기
}

// main
int main() {
	srand((unsigned int)time(NULL)); // 랜덤 값을 매번 다르게 바꿔주는 함수 없으면 값 고정

	// train lenght 입력
	input_train_lenght();

	// madongseok stamina 입력
	input_madongseok_stamina();

	// percentile probability 입력
	input_percentile_probability();

	//인트로 함수 출력
	intro();

	// 열차의 초기 상태
	train_state(train_lenght);
	printf("\n\n\n");

	// 움직임과 현재 상태
	while (1) {
		move();
	}

	return 0;
}
