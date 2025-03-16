//ONLY CHAT GPT는 챗지피티가 짜준 코드라는거임

//헤더
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

// 기호
#define PLAYER "■"
#define MOB "@"
#define PROJECTILE "#"
#define DANGER "!"
//
#define WINDOW_SIZE_X 152 // 창 X 크기
#define WINDOW_SIZE_Y 40 // 창 Y 크기

typedef struct { // 부동 소수점 좌표계
    float X;
    float Y;
} FLOAT_COORD;

typedef struct { // 몹
    COORD pos; // 현재 위치
    COORD next; // 예정 위치
} Mob;

typedef struct { // 투사체
    FLOAT_COORD Direction; // 방향
    FLOAT_COORD pos; // 현재 위치
} Projectile;


FLOAT_COORD GetDirection(COORD FROM, COORD TO) { // 방향 구하기기
    
    //좌표 구하기
    float X1 = TO.X-FROM.X; 
    float Y1 = TO.Y-FROM.Y;

    //정규화 하기
    float X = X1 / sqrt(pow(X1,2)+pow(Y1,2));
    float Y = Y1 / sqrt(pow(X1,2)+pow(Y1,2));
    
    FLOAT_COORD direction = {X,Y}; // 좌표 만들기

    return direction;
}

int isCoordinateInsideConsole(float X, float Y) { // 현재 좌표가 콘솔의 밖인지 안인지 보기 1이면 안, 0이면 밖 (ONLY CHAT GPT)
    int x = ceil(X);
    int y = ceil(Y);
    // 콘솔 윈도우의 핸들을 가져오기
    HWND hwnd = GetConsoleWindow();

    // 콘솔 화면 버퍼 정보 가져오기
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    // 콘솔 창의 크기 (열 수와 행 수)
    int consoleWidth = csbi.dwSize.X;  // 열 수
    int consoleHeight = csbi.dwSize.Y; // 행 수

    // 주어진 좌표가 콘솔 창 안에 있는지 확인
    if (x >= 0 && x < consoleWidth && y >= 0 && y < consoleHeight) {
        return 1;  // 창 안에 있음
    }
    return 0;  // 창 밖에 있음
}


void TP(float X, float Y) { // 커서 위치 변환 (ONLY CHAT GPT)
    int x = ceil(X);
    int y = ceil(Y);
    COORD coord;  // 좌표 구조체
    coord.X = x;  // X 좌표
    coord.Y = y;  // Y 좌표

    // 콘솔 핸들 가져오기
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCursorPosition(hConsole, coord);
}

COORD Player_Pre = {0,0}; // 이전 플레이어 위치

COORD Player_coord = {0,0}; // 현재 플레이어 위치

int main() {
    system("chcp 65001"); // 인코딩
    system("cls"); // 콘솔창 비우기

    int score; // 점수
    int mob_count = 1; // 몹 수
    int projectile_count = 0; // 투사체 수
    Mob *mobs = (Mob *) malloc(sizeof(Mob)*mob_count); // 몬스터 배열
    if (mobs == NULL) {
        printf("mobs MALLOC이 작동하지 못함!");
        exit(1);
    }
    mobs[0].pos = (COORD) {71,20}; // 첫 몹의 위치

    
    Projectile *projectiles = (Projectile *) malloc(sizeof(Mob)*3); // 투사체 배열
    if (projectiles == NULL) {
        printf("projectiles MALLOC이 작동하지 못함!");
        exit(1);
    }

    int newMobTerm=0; // 새 몹 쿨타임
    int MobMoveTerm=0; // 몹 위치변환 쿨타임
    int MobAttackTerm=0; // 몹 공격 쿨타임
    int isgameover=0; // 게임오버 여부
    srand(time(NULL)); // 랜덤 시드를 시간으로 맞춤

    while (1) {
        system("cls"); // 콘솔창 비우기
        if (GetAsyncKeyState('W') & 0x8000) { // W 클릭 시
            Player_coord.Y-=1;
        }
        
        if (GetAsyncKeyState('A') & 0x8000) { // A 클릭 시
            Player_coord.X-=2; // 줄바꿈 특성상 X 한칸이 Y 한칸보다 짧기 때문에 2배
        }
        
        if (GetAsyncKeyState('S') & 0x8000) { // S 클릭 시
            Player_coord.Y+=1; 
        }
        
        if (GetAsyncKeyState('D') & 0x8000) { // D 클릭 시
            Player_coord.X+=2; // 줄바꿈 특성상 X 한칸이 Y 한칸보다 짧기 때문에 2배
        }
        if (isCoordinateInsideConsole(Player_coord.X,Player_coord.Y)) { // 콘솔 밖이 아니면 플레이어 위치로 커서 옮긴 후 출력
            TP(Player_coord.X,Player_coord.Y);
            printf(PLAYER);
        } else {
            Player_coord=Player_Pre; // 변환된 현재 위치를 이전 위치로 변경
        }

        Player_Pre=Player_coord; //이전 위치 갱신

        if (newMobTerm >= 200) { // 1당 0.05초
            newMobTerm=0;
            mob_count++; // 몹 수 증가
            mobs = (Mob *) realloc(mobs,sizeof(Mob)*mob_count); // 메모리 크기 변환

            // 모듈러 연산자와 랜덤 함수를 이용하여 위치를 랜덤하게 변경
            mobs[mob_count-1].pos.X = rand() % WINDOW_SIZE_X+1;
            mobs[mob_count-1].pos.Y = rand() % WINDOW_SIZE_Y+1;
        }

        for (int i = 0; i < mob_count; i++) {
            TP(mobs[i].pos.X,mobs[i].pos.Y); // 배열에 있는 모든 몹의 위치로 커서 옮긴 후 출력
            printf(MOB);
        }

        if (MobMoveTerm == 60) {
            for (int i = 0; i < mob_count; i++) { // 경고를 위한 위치 설정
                mobs[i].next.X = rand() % WINDOW_SIZE_X+1;
                mobs[i].next.Y = rand() % WINDOW_SIZE_Y+1;
            }
        }

        if (MobMoveTerm > 60 && MobMoveTerm < 80) { // 경고 위치에 커서를 옮긴 후 출력
            for (int i = 0; i < mob_count; i++) {
                TP(mobs[i].next.X,mobs[i].next.Y);
                printf(DANGER);
            }
        }

        if (MobMoveTerm >= 80) { // 위치 옮기기
            MobMoveTerm=0;
            for (int i = 0; i < mob_count; i++) {
                mobs[i].pos.X = mobs[i].next.X;
                mobs[i].pos.Y = mobs[i].next.Y;
            }
        }

        if (MobAttackTerm >= 20) {
            MobAttackTerm=0;
            for (int i = 0; i < mob_count; i++) {
                projectile_count++;
                projectiles = (Projectile *) realloc(projectiles,sizeof(Projectile)*projectile_count); // 투사체 배열 크기 재설정
                //위치변경
                projectiles[projectile_count-1].pos.X = mobs[i].pos.X;
                projectiles[projectile_count-1].pos.Y = mobs[i].pos.Y;

                projectiles[projectile_count-1].Direction = GetDirection(mobs[i].pos,Player_coord); // 플레이어쪽으로 방향 설정
            }
        }

        for (int i = 0; i < projectile_count; i++) {
            projectiles[i].pos.X += projectiles[i].Direction.X*2; // 줄바꿈 특성상 X 한칸이 Y 한칸보다 짧기 때문에 2배
            projectiles[i].pos.Y += projectiles[i].Direction.Y;

            if (isCoordinateInsideConsole(projectiles[i].pos.X,projectiles[i].pos.Y)) { // 투사체가 창안에 있으면
                TP(projectiles[i].pos.X,projectiles[i].pos.Y); // 커서 위치 옮기기
                printf(PROJECTILE); // 출력
                
                if ((abs(projectiles[i].pos.X-Player_coord.X) < 1) && (abs(projectiles[i].pos.Y-Player_coord.Y) < 1)) { // 히트박스 : 투사체가 중심인 3x3 공간 (의외로 작음)
                    isgameover=1;
                }
            } else { // 만약 투사체가 창 밖에 있다면 투사체 제거
                for (int j = i; j < projectile_count - 1; j++) { // 현재 투사체의 인덱스를 기준으로 앞에 있는 모든 요소를 앞으로 당김
                    projectiles[j] = projectiles[j + 1];
                }
                projectile_count--; // 투사체 개수 감소
                i--;
            }
        }
        
        // 쿨타임
        newMobTerm++;
        MobAttackTerm++;
        MobMoveTerm++;
        score++;
        TP(0,0); // 커서가 거슬리니깐 위치를 0,0으로 옮김

        if (isgameover) { // 게임오버
            break;
        }

        Sleep(50); // 딜레이 0.05초
    }

    
    TP(0,0);
    free(mobs);
    free(projectiles);
    printf("사망! 점수 : %d",score); // 사망
    Sleep(10000);

    return 0;
}