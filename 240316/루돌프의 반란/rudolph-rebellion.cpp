#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

int N, M, P, C, D; // 게임판의 크기, 게임턴 수, 산타 수, 루돌프의 힘, 산타의 힘
int Rr, Rc; // 루돌프 초기 위치 (Rr, Rc)
int santa[31][4] = {0, }; // P개 줄에 걸쳐 산타 번호(x)와 산타 초기위치 (y)(Sr)(Sc)(점수)(기절)
int board[50][50] = {0, }; // 게임판에 산타가 있다면 산타번호, 없으면 0
vector<int> isLive; // 현재 생존한 산타

int distance() {
    int min = pow(N-1, 2) * 2;
    vector<int> near;
    for(int i = 0; i < isLive.size(); i++) {
        int Sr = santa[isLive[i]][0];
        int Sc = santa[isLive[i]][1];
        int dist = pow(Rr - Sr, 2) + pow(Rc - Sc, 2);
        if(dist == min) {
            near.push_back(isLive[i]); // near에 가까운 산타 번호 넣기
        }
        else if(dist < min) {
            dist = min;
            near.clear();
            near.push_back(isLive[i]);
        }
    }
    // 가까운 산타가 2명 이상이면
    if(near.size() >= 2) {
        int maxR = 1;
        vector<int> more_near;
        for(int i = 0; i < near.size(); i++) {
            if(maxR == santa[near[i]][0]) {
                more_near.push_back(near[i]);
            }
            else if(maxR < santa[near[i]][0]) {
                maxR = santa[near[i]][0];
                more_near.clear();
                more_near.push_back(near[i]);
            }
        }
        // r좌표도 동일하면 c좌표가 큰 산타
        if(more_near.size() >= 2) {
            int maxC = 1;
            int mostNearSanta;
            for(int i = 0; i < more_near.size(); i++) {
                if(maxC < santa[more_near[i]][1]) {
                    maxC = santa[more_near[i]][1];
                    mostNearSanta = more_near[i];
                }
            }
            return mostNearSanta;
        }
        return more_near[0];
    }
    return near[0];
}

void game() {
    // 가장 가까운 산타 번호 구하기
    int nearSanta = distance();
    int dirX, dirY;
    if(santa[nearSanta][0] > Rr) dirX = 1;
    else if(santa[nearSanta][0] == Rr) dirX = 0;
    else dirX = -1;
    if(santa[nearSanta][1] > Rc) dirY = 1;
    else if(santa[nearSanta][1] == Rc) dirY = 0;
    else dirY = -1;
    // 루돌프가 거리 가장 가까운 산타쪽으로 이동
    Rr += dirX, Rc += dirY;
    // 충돌했으면 == 게임판에 산타가 있으면
    if(board[Rr][Rc] != 0) {
        int santaIndex = board[Rr][Rc];
        board[Rr][Rc] = 0;
        // 산타 C 점수 획득
        santa[santaIndex][2] += C;
        // 산타 날아가고
        santa[santaIndex][0] += dirX * C, santa[santaIndex][1] += dirY * C;

        // 범위 밖이면 탈락 isLive에서 지우기
        if(santa[santaIndex][0] < 1 || santa[santaIndex][0] > N
        || santa[santaIndex][1] < 1 || santa[santaIndex][1] > N) {
            auto it = find(isLive.begin(), isLive.end(), santaIndex);
            isLive.erase(it);
        }
        // 범위 안이면 기절 + 2
        else {
            santa[santaIndex][3] += 2;
            // 그 위치의 산타랑 충돌했으면 연쇄작용
            int r = santa[santaIndex][0], c = santa[santaIndex][1];
            while(true) {
                if(board[r][c] == 0) {
                    board[r][c] = santaIndex;
                    break;
                }
                else {
                    int crashedSanta = board[r][c];
                    board[r][c] = santaIndex;
                    santa[crashedSanta][0] += dirX, santa[crashedSanta][0] += dirY;
                    if(santa[crashedSanta][0] < 1 || santa[crashedSanta][0] > N
                    || santa[crashedSanta][1] < 1 || santa[crashedSanta][1] > N) {
                        auto it = find(isLive.begin(), isLive.end(), crashedSanta);
                        isLive.erase(it);
                        break;
                    }
                    else {
                        r = santa[crashedSanta][0], c = santa[crashedSanta][1];
                        santaIndex = crashedSanta;
                    }
                }
            }
        }
    }

    sort(isLive.begin(), isLive.end());
    
    // 생존한 산타 1~P번까지 이동
    for(int i = 0; i < isLive.size(); i++) {
        int santaIndex = isLive[i];
        // 기절한 상태면 패스, 기절 - 1
        if(santa[santaIndex][3] > 0) {
            santa[santaIndex][3]--;
            continue;
        }

        int dX[4] = {-1, 0, 1, 0};
        int dY[4] = {0, 1, 0, -1};
        int dirX = 0, dirY = 0;
        if(santa[santaIndex][0] < Rr) {
            dirX = dX[0], dirY = dY[0];
        }
        else if(santa[santaIndex][1] < Rc) {
            dirX = dX[1], dirY = dY[1];
        }
        else if(santa[santaIndex][0] == Rr && santa[santaIndex][1] > Rc) {
            dirX = dX[3], dirY = dY[3];
        }
        else {
            dirX = dX[2], dirY = dY[2];
        }
        int nextR = santa[santaIndex][0] + dirX, nextC = santa[santaIndex][1] + dirY;
        
        
        // 만약 그 위치에 산타가 있으면 이동 안함
        if(board[nextR][nextC] != 0) continue;
        // 아니면 한칸 이동
        else {
            board[santa[santaIndex][0]][santa[santaIndex][1]] = 0;
            board[nextR][nextC] = i;
            santa[santaIndex][0] = nextR;
            santa[santaIndex][1] = nextC;
            // 충돌했으면
            if(Rr == nextR && Rc == nextC) {
                // 산타 D 점수 획득
                santa[santaIndex][2] += D;
                // 산타 날아가고
                board[nextR][nextC] = 0;
                santa[santaIndex][0] -= dirX * D, santa[santaIndex][1] -= dirY * D;
                    // 범위 밖이면 탈락 isLive에서 지우기
                    // 범위 안이면 기절 + 2
                    // 그 위치의 산타랑 충돌했으면 연쇄작용
                    // 범위 밖이면 탈락 isLive에서 지우기
                if(santa[santaIndex][0] < 1 || santa[santaIndex][0] > N
                || santa[santaIndex][1] < 1 || santa[santaIndex][1] > N) {
                    auto it = find(isLive.begin(), isLive.end(), santaIndex);
                    isLive.erase(it);
                    i--;
                }
                // 범위 안이면 기절 + 2
                else {
                    santa[santaIndex][3] += 2;
                    // 그 위치의 산타랑 충돌했으면 연쇄작용
                    int r = santa[santaIndex][0], c = santa[santaIndex][1];
                    while(true) {
                        if(board[r][c] == 0) {
                            board[r][c] = santaIndex;
                            break;
                        }
                        else {
                            int crashedSanta = board[r][c];
                            board[r][c] = santaIndex;
                            santa[crashedSanta][0] += dirX, santa[crashedSanta][1] += dirY;
                            if(santa[crashedSanta][0] < 1 || santa[crashedSanta][0] > N
                            || santa[crashedSanta][1] < 1 || santa[crashedSanta][1] > N) {
                                auto it = find(isLive.begin(), isLive.end(), crashedSanta);
                                isLive.erase(it);
                                break;
                            }
                            else {
                                r = santa[crashedSanta][0], c = santa[crashedSanta][1];
                                santaIndex = crashedSanta;
                            }
                        }
                    }
                }
            }
        }
    }
    // 생존한 산타 전부 점수 1점씩 획득
    for(auto santaIndex: isLive) {
        santa[santaIndex][2]++;
    }
}

int main() {
    cin >> N >> M >> P >> C >> D;
    cin >> Rr >> Rc;
    for(int i = 0; i < P; i++) {
        int num, sr, sc;
        cin >> num >> sr >> sc;
        board[sr][sc] = num;
        santa[num][0] = sr, santa[num][1] = sc;
        isLive.push_back(i+1);
    }

    for(int i = 0; i < M; i++) {
        game();
    }
    for(int i = 1; i <= P; i++) {
        if(i == P) cout << santa[i][2];
        else cout << santa[i][2] << " ";
    }
    return 0;
}