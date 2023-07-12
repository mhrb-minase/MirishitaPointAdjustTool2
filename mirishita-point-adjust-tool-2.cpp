#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

constexpr int pointLimit = 10000000; // 扱えるポイント数の上限
constexpr int eventItemLimit = 1000000000; // 扱えるイベントアイテム数の上限
constexpr int inf = (1 << 30) - 1; // 最小化したい変数の初期値
constexpr int necessaryEventItemPerLive = 180; // イベント曲(等倍)1回に必要なアイテム数
constexpr int pointPerOneEventLive = 537; // イベント曲(等倍)1回で得られるポイント数
std::vector<int> eventLiveMagnificationList = {4, 2, 1}; // イベント楽曲倍率リスト(大きい順)

struct Live
{
    std::string name;
    int point;
};

// イベント曲以外のライブ定義
const std::vector<Live> normalLives = {
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
    {"normal MM 10.5x", 893}
};

int input_integer(std::string name, int upperLimit)
{
    int point = -1;
    while (true) {
        std::cout << " input " + name + "> ";
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
// 各ライブの必要回数が入ったvectorが返る
// 最後の要素にイベント曲(等倍)の必要回数が入る
// 目標ポイント数を取ることが不可能な場合、空のvectorを返す
std::vector<int> calculate(int targetPoint, int currentPoint, int currentEventItem)
{
    int remainPoint = targetPoint - currentPoint; // 残りポイント

    std::vector<int> minNormalLiveCount(remainPoint + 1, inf); // minNormalLiveCount[i] := iポイント取るために必要な最小ライブ回数
    minNormalLiveCount[0] = 0;
    std::vector<int> lastLiveIndex(remainPoint + 1, -1); // lastLiveIndex[i] := 最小ライブ回数でiポイント取ったときの最後に行ったライブ

    for (int i = 0; i <= remainPoint; ++i) {
        for (int j = 0; j < (int)normalLives.size(); ++j) {
            Live l = normalLives[j];

            // 必要以上に稼ぎそうになるケースを弾く
            if (i + l.point > remainPoint) {
                continue;
            }

            // 最小ライブ回数を更新できる場合、回数とともに最後に行ったライブも更新する
            if (minNormalLiveCount[i] + 1 < minNormalLiveCount[i + l.point]) {
                minNormalLiveCount[i + l.point] = minNormalLiveCount[i] + 1;
                lastLiveIndex[i + l.point] = j;
            }
        }
    }

    int minLiveCount = minNormalLiveCount[remainPoint]; // イベント曲含めた最小ライブ回数を入れる
    int normalLivePoint = remainPoint;
    for (int i = 0; i <= remainPoint; ++i) {
        // iポイントまで通常楽曲で貯めるとする

        int remainEventItem = currentEventItem + i; // 持っているはずのイベントアイテム数
        int eventLivePoint = remainPoint - i; // イベントライブで必要なポイント数

        // 組み合わせとしてありえない場合を弾く
        if (
            minNormalLiveCount[i] >= inf ||
            eventLivePoint % pointPerOneEventLive != 0 ||
            (long long)eventLivePoint / pointPerOneEventLive * necessaryEventItemPerLive > remainEventItem
        ) {
            continue;
        }

        int eventLiveCount = 0; // イベントライブ回数
        int tmp = eventLivePoint;
        for (int magnification : eventLiveMagnificationList) {
            eventLiveCount += tmp / (pointPerOneEventLive * magnification);
            tmp %= (pointPerOneEventLive * magnification);
        }

        // あり得ない組み合わせを弾いた上、1倍消費はあるのだが、念のためポイントを取りきれてない場合を弾いておく
        if (tmp > 0) {
            continue;
        }

        if (minNormalLiveCount[i] + eventLiveCount < minLiveCount) {
            minLiveCount = minNormalLiveCount[i] + eventLiveCount;
            normalLivePoint = i;
        }
    }

    // 目標ポイントを取ることが不可能な場合
    if (minLiveCount >= inf) {
        return {};
    }

    std::vector<int> liveCount((int)normalLives.size(), 0); // 各ライブを行う回数
    // remain point から遡って行うライブを復元する
    int point = normalLivePoint;
    while (true) {
        if (point == 0) {
            break;
        }

        int lastLive = lastLiveIndex[point];
        ++liveCount[lastLive];
        point -= normalLives[lastLive].point;
    }

    // イベントライブ回数(等倍)を加える
    liveCount.emplace_back((remainPoint - normalLivePoint) / pointPerOneEventLive);

    return liveCount;
}

int main()
{
    int targetPoint = 0; // 目標ポイント
    int currentPoint = 0; // 現在のポイント
    int currentEventItem = 0; // 現在のイベントアイテム所持数

    // 念のためイベント楽曲倍率リストを大きい順に並べ替えておく
    std::sort(eventLiveMagnificationList.rbegin(), eventLiveMagnificationList.rend());

    // 入力
    targetPoint = input_integer("target point", pointLimit);
    currentPoint = input_integer("current point", targetPoint);
    currentEventItem = input_integer("number of event items", eventItemLimit);

    std::vector<int> liveCount = calculate(targetPoint, currentPoint, currentEventItem);

    // 出力
    if (!liveCount.empty()) {
        std::cout << " live count is below." << std::endl;
        for (int i = 0; i < (int)normalLives.size(); ++i) {
            std::cout << " " + normalLives[i].name + " : " + std::to_string(liveCount[i]) << std::endl;
        }
        int tmp = liveCount.back(); // イベントライブ回数
        for (int magnification : eventLiveMagnificationList) {
            std::cout << " event " + std::to_string(magnification) + "x : " + std::to_string(tmp / magnification) << std::endl;
            tmp %= magnification;
        }
    } else {
        std::cout << " you cannot get just " + std::to_string(targetPoint) + " points." << std::endl;
    }

    return 0;
}
