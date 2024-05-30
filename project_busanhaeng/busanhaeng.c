#include<stdio.h>
#include<stdlib.h> // 랜덤 함수
#include<time.h> // time 함수
#include<stdbool.h>

// 기차 길이
#define LEN_MIN 15
#define LEN_MAX 50

// 마동석 체력
#define STM_MIN 0
#define STM_MAX 5

// 확률
#define PROB_MIN 10
#define PROB_MAX 90

// 어그로 범위
#define AGGRO_MIN 0
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
int madongseok_move_input;
char arr[3][50]; // 기차 상태 배열
int citizen_aggro = 1; // 시민의 어그로
int madongseok_aggro = 1; // 마동석의 어그로
bool citizen_move = false; // 사람이 움직이냐 안움직이냐
bool zombie_move = false; // 좀비가 움직이냐 안움직이냐
bool zombie_left = false; // 좀비의 이동방향이 왼쪽
bool zombie_right = false; // 좀비의 이동방향이 오른쪽
bool zombie_pull = false; // 마동석이 좀비 잡기 성공 or 실패

//함수 세팅
void intro();
void input_train_lenght();
void input_madongseok_stamina();
void input_percentile_probability();
void train_init(int);
void train_state(int);
void move_prob();
void move_citizen();
void move_zombie();
void input_madongseok_move();
void move_madongseok();
void citizen_behavior();
void zombie_behavior();
void madongseok_behavior();
void move();
void finish();
void fail_outtro();
void success_outtro();

// 인트로 출력 함수
void intro() {
	printf("준태를 살려라 ~~~\n\n");
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

void train_init(int train_len) {
	for (int i = 0; i < train_len; i++) {
		arr[0][i] = '#';
		arr[2][i] = '#';
	}
	for (int i = 0; i < train_len; i++) {
		if (i == 0 || i == train_len - 1) arr[1][i] = '#';
		// 시민
		else if (i == train_len - citizen) arr[1][i] = 'C';
		// 좀비
		else if (i == train_len - zombie) arr[1][i] = 'Z';
		// 마동석
		else if (i == train_len - madongseok) arr[1][i] = 'M';
		else arr[1][i] = ' ';
	}
}

// 열차의 상태를 출력하는 함수
void train_state(int train_len) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < train_len; j++) {
			printf("%c", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void move_prob() {
	// 사람의 이동 확률
	citizen_move = false;
	int rand_num_citizen = rand() % (PROB_MIN + PROB_MAX) + 1;
	if (percentile_probability < rand_num_citizen && rand_num_citizen <= PROB_MIN + PROB_MAX) {
		citizen_move = true;
		arr[1][train_lenght - citizen] = ' ';
		citizen++;
		arr[1][train_lenght - citizen] = 'C';
	}

	// 좀비는 두턴마다 한번 이동 기회 주어짐, 좀비의 이동 확률
	if (count % 2 != 0) {
		zombie_move = false;
		zombie_left = false;
		zombie_right = false;
		int rand_num_zombie = rand() % (PROB_MIN + PROB_MAX) + 1;
		if (rand_num_zombie <= percentile_probability) {
			if (citizen_aggro >= madongseok_aggro) {
				zombie_move = true;
				zombie_left = true;
			}
			else {
				zombie_move = true;
				zombie_right = true;
			}
		}
	}
}

// 시민이 움직였을 경우
void move_citizen() {
	int k = citizen_aggro;
	if (citizen_move == true) {
		if (citizen_aggro < AGGRO_MAX) {
			citizen_aggro++;
		}
		printf("citizen: %d -> %d (aggro: %d -> %d)\n", train_lenght - citizen + 1, train_lenght - citizen, k, citizen_aggro);
	}
	else {
		if (citizen_aggro > AGGRO_MIN) {
			citizen_aggro--;
		}
		printf("citizen: stay %d (cannot move) (aggro: %d -> %d)\n", train_lenght - citizen, k, citizen_aggro);
	}
}

void move_zombie() {
	if (count % 2 != 0 && zombie_pull == false) {
		if (zombie_move == true && zombie_left == true && zombie + 1 != citizen) {
			arr[1][train_lenght - zombie] = ' ';
			zombie++;
			arr[1][train_lenght - zombie] = 'Z';
			train_state(train_lenght);
			move_citizen();
			printf("zombie: %d -> %d (can move turn)\n", train_lenght - zombie + 1, train_lenght - zombie);
		}
		else if (zombie_move == true && zombie_right == true && zombie - 1 != madongseok) {
			arr[1][train_lenght - zombie] = ' ';
			zombie--;
			arr[1][train_lenght - zombie] = 'Z';
			train_state(train_lenght);
			move_citizen();
			printf("zombie: %d -> %d (can move turn)\n", train_lenght - zombie - 1, train_lenght - zombie);
		}
		else {
			train_state(train_lenght);
			move_citizen();
			printf("zombie: stay %d (cannot move) (can move turn)\n", train_lenght - zombie);
		}
	}
	else {
		if (zombie_pull == true) {
			train_state(train_lenght);
			move_citizen();
			printf("zombie: stay %d (cannot move) (madongseok pull success)\n", train_lenght - zombie);
		}
		else {
			train_state(train_lenght);
			move_citizen();
			printf("zombie: stay %d (cannot move) (can't move turn)\n", train_lenght - zombie);
		}
	}
	zombie_pull = false;
}

void input_madongseok_move() {
	while (1) {
		if (madongseok + 1 == zombie) {
			printf("madongseok move(0: stay)>> ");
			scanf_s("%d", &madongseok_move_input);
			if (madongseok_move_input == 0) {
				break;
			}
			else {
				printf("0을 입력해주세요.\n");
			}
		}
		else {
			printf("madongseok move(0: stay, 1: left)>> ");
			scanf_s("%d", &madongseok_move_input);
			if (madongseok_move_input == 0 || madongseok_move_input == 1) {
				break;
			}
			else {
				printf("0 또는 1을 입력해주세요.\n");
			}
		}
	}
}

void move_madongseok() {
	input_madongseok_move();
	int k = madongseok_aggro;
	if (madongseok_move_input == 0) {
		if (madongseok_aggro > AGGRO_MIN) {
			madongseok_aggro--;
		}
		train_state(train_lenght); // 바뀐 위치 맵 호출
		printf("madongseok: stay %d (aggro: %d -> %d, stemina: %d)\n", train_lenght - madongseok, k, madongseok_aggro, madongseok_stamina);
	}
	else if (madongseok_move_input == 1) {
		if (madongseok_aggro < AGGRO_MAX) {
			madongseok_aggro++;
		}
		arr[1][train_lenght - madongseok] = ' ';
		madongseok++;
		arr[1][train_lenght - madongseok] = 'M';
		train_state(train_lenght); // 바뀐 위치 맵 호출
		printf("madongseok: %d -> %d (aggro: %d -> %d, stemina: %d)\n\n", train_lenght - madongseok, train_lenght - madongseok - 1, k, madongseok_aggro, madongseok_stamina);
	}
}

void citizen_behavior() {
	printf("citizen does nothing.\n");
}

void zombie_behavior() {
	if ((zombie + 1) == (citizen) && (zombie - 1) == (madongseok)) {
		if (citizen_aggro >= madongseok_aggro) {
			printf("GAME OVER! citizen dead...");
			exit(0);
		}
		else {
			printf("Zombie attacked madongseok (aggro: %d vs %d, madongseok stemina: %d -> %d)\n", citizen_aggro, madongseok_aggro, madongseok_stamina, madongseok_stamina - 1);
			madongseok_stamina--;
			if (madongseok_stamina == STM_MIN) {
				printf("GAME OVER! madongseok dead...");
				exit(0);
			}
		}
	}
	else if ((zombie + 1) == (citizen)) {
		printf("GAME OVER! citizen dead...");
		exit(0);
	}
	else if ((zombie - 1) == (madongseok)) {
		printf("Zombie attacked madongseok (madongseok stemina: %d -> %d)\n", madongseok_stamina, madongseok_stamina - 1);
		madongseok_stamina--;
		if (madongseok_stamina == STM_MIN) {
			printf("GAME OVER! madongseok dead...");
			exit(0);
		}
	}
	else {
		printf("zombie does nothing\n");
	}
}

void madongseok_behavior() {
	int number;
	int k = madongseok_aggro;
	int ks = madongseok_stamina;
	if (zombie - 1 != madongseok) {
		while (1) {
			if (madongseok + 1 != zombie) {
				printf("madongseok action(0.rest, 1.provoke)>> ");
				scanf_s("%d", &number);
				if (number == ACTION_REST || number == ACTION_PROVOKE) {
					break;
				}
				else {
					printf("다시 입력해주세요.\n");
				}
			}
		}
		if (number == ACTION_REST) {
			if (madongseok_aggro > AGGRO_MIN) {
				madongseok_aggro--;
			}
			if (madongseok_stamina < STM_MAX) {
				madongseok_stamina++;
			}
			printf("madongseok rests...\n");
			printf("madongseok: (aggro: %d -> %d, stemina: %d -> %d)\n\n", k, madongseok_aggro, ks, madongseok_stamina);
		}
		else if (number == ACTION_PROVOKE) {
			printf("madongseok provoked zombie...\n");
			printf("madongseok: (aggro: %d -> %d, stemina: %d)\n\n", madongseok_aggro, AGGRO_MAX, madongseok_stamina);
			madongseok_aggro = AGGRO_MAX;
		}
	}
	else {
		while (1) {
			if (madongseok + 1 == zombie) {
				printf("madongseok action(0.rest, 1.provoke, 2.pull)>> ");
				scanf_s("%d", &number);
				if (number == ACTION_REST || number == ACTION_PROVOKE || number == ACTION_PULL) {
					break;
				}
				else {
					printf("다시 입력해주세요.\n");
				}
			}
		}
		if (number == ACTION_REST) {
			if (madongseok_aggro > AGGRO_MIN) {
				madongseok_aggro--;
			}
			if (madongseok_stamina < STM_MAX) {
				madongseok_stamina++;
			}
			printf("madongseok rests...\n");
			printf("madongseok: (aggro: %d -> %d, stemina: %d -> %d)\n\n", k, madongseok_aggro, ks, madongseok_stamina);
		}
		else if (number == ACTION_PROVOKE) {
			printf("madongseok provoked zombie...\n");
			printf("madongseok: (aggro: %d -> %d, stemina: %d)\n\n", madongseok_aggro, AGGRO_MAX, madongseok_stamina);
			madongseok_aggro = AGGRO_MAX;
		}
		else if (number == ACTION_PULL) {
			int pull_prob = rand() % (PROB_MIN + PROB_MAX) + 1;
			if (percentile_probability < pull_prob && pull_prob <= PROB_MIN + PROB_MAX) {
				printf("madongseok pulled zombie... next turn, it can't move\n");
				madongseok_aggro -= 2;
				if (madongseok_aggro < AGGRO_MIN) {
					madongseok_aggro = AGGRO_MIN;
				}
				madongseok_stamina--;
				printf("madongseok: (aggro: %d -> %d, stemina: %d -> %d)\n\n", k, madongseok_aggro, ks, madongseok_stamina);
				if (madongseok_stamina == STM_MIN) {
					printf("GAME OVER! madongseok dead...");
					exit(0);
				}
				zombie_pull = true;
			}
			else {
				printf("madongseok failed to pull zombie\n");
				madongseok_aggro -= 2;
				if (madongseok_aggro < AGGRO_MIN) {
					madongseok_aggro = AGGRO_MIN;
				}
				madongseok_stamina--;
				printf("madongseok: (aggro: %d -> %d, stemina: %d -> %d)\n\n", k, madongseok_aggro, ks, madongseok_stamina);
				if (madongseok_stamina == STM_MIN) {
					printf("GAME OVER! madongseok dead...");
					exit(0);
				}
				zombie_pull = false;
			}
		}
	}
}

// 4초마다 시민 이동, 좀비 이동, 열차 출력, 시민, 좀비 상태 출력 함수
void move() {
	move_prob(); // 이동 확률
	// train_state(train_lenght); // 바뀐 위치 맵 호출
	// move_citizen(); // 시민 이동
	move_zombie(); // 좀비 이동
	printf("\n"); // 줄바꿈
	move_madongseok();
	citizen_behavior();
	zombie_behavior();
	madongseok_behavior();
	finish(); // 끝나는 경우
	count++; // 좀비 이동 주기
}

// 성공 아웃트로 함수
void success_outtro() {
	printf("\nSUCCESS!\ncitizen(s) escaped to the next train");
}

// 실패 아웃트로 함수
void fail_outtro() {
	printf("\nfail! ㅠㅠ");
}

void finish() {
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

	if (madongseok_stamina == STM_MIN) {
		fail_outtro();
		exit(0);
	}
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

	//열차 초기화
	train_init(train_lenght);

	// 열차의 초기 상태
	train_state(train_lenght);
	printf("\n");

	// 움직임과 현재 상태
	while (1) {
		move();
	}

	return 0;
}
