#include<iostream>
#include<vector>
#include<string>

constexpr int pointLimit = 10000000; // 扱えるポイント数の上限
constexpr int inf = (1 << 30) - 1; // 最小化したい変数の初期値

struct Live
{
    std::string name;
    int point;
};

// ライブ定義
const std::vector<Live> allLives = {
    // おすすめ楽曲
    {"recommend 2M 10.5x", 441},
    {"recommend 4M 10.5x", 620},
    {"recommend 6M 10.5x", 809},
    {"recommend MM 10.5x", 1071},
    {"recommend solo 2M 10.5x", 441},
    {"recommend solo 2M+ 10.5x", 777},

    // 通常楽曲
    {"normal 2M 10.5x", 368},
    {"normal 4M 10.5x", 515},
    {"normal 6M 10.5x", 672},
    {"normal MM 10.5x", 893},

    // イベント楽曲
    {"event 4x", 2148},
    {"event 2x", 1074},
    {"event 1x", 537}
};

int input_point(std::string targetOrCurrent, int upperLimit = pointLimit)
{
    int point = -1;
    while (true) {
        std::cout << " input " + targetOrCurrent + " point> ";
        std::cin >> point;
        if(0 <= point && point <= upperLimit) {
            break;
        } else {
            std::cout << " input value between 0 and " + std::to_string(upperLimit) << std::endl;
        }
    }
    return point;
}

// ライブ回数計算関数
// 目標ポイント数を取ることが不可能な場合、空のvectorを返す
std::vector<int> calculate(int target, int current)
{
    int remain = target - current; // 残りポイント

    std::vector<int> minLiveCount(remain + 1, inf); // minLiveCount[i] := iポイント取るために必要な最小ライブ回数
    minLiveCount[0] = 0;
    std::vector<int> lastLiveIndex(remain + 1, -1); // lastLiveIndex[i] := 最小ライブ回数でiポイント取ったときの最後に行ったライブ

    for (int i = 0; i <= remain; ++i) {
        for (int j = 0; j < (int)allLives.size(); ++j) {
            Live l = allLives[j];

            // 必要以上に稼ぎそうになるケースを弾く
            if (i + l.point > remain) {
                continue;
            }

            // 最小ライブ回数を更新できる場合、回数とともに最後に行ったライブも更新する
            if (minLiveCount[i] + 1 < minLiveCount[i + l.point]) {
                minLiveCount[i + l.point] = minLiveCount[i] + 1;
                lastLiveIndex[i + l.point] = j;
            }
        }
    }

    // 目標ポイントを取ることが不可能な場合
    if (minLiveCount[remain] >= inf) {
        return {};
    }

    std::vector<int> liveCount((int)allLives.size(), 0); // 各ライブを行う回数
    // remain point から遡って行うライブを復元する
    int point = remain;
    while (true) {
        if (point == 0) {
            break;
        }

        int lastLive = lastLiveIndex[point];
        ++liveCount[lastLive];
        point -= allLives[lastLive].point;
    }

    return liveCount;
}

int main()
{
    int target = 0; // 目標ポイント
    int current = 0; // 現在のポイント

    // 目標と現在のポイントを入力
    target = input_point("target");
    current = input_point("current", target);

    std::vector<int> liveCount = calculate(target, current);

    // 出力
    if (!liveCount.empty()) {
        std::cout << " live count is below." << std::endl;
        for (int i = 0; i < (int)allLives.size(); ++i) {
            std::cout << " " + allLives[i].name + " : " + std::to_string(liveCount[i]) << std::endl;
        }
    } else {
        std::cout << " you cannot get just " + std::to_string(target) + " points." << std::endl;
    }

    return 0;
}
