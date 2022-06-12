#include <bangtal>
using namespace bangtal;

class Rect {     // 틀을 위해 만들어둔 거구만 -> 객체? 구조체?로 쓰임
	int centerX, centerY;
	int radius;

public:
	Rect(int cx, int cy, int r) : centerX(cx), centerY(cy), radius(r) {};        // 올바른 위치 범위??
	bool checkIn(int x, int y) const {                                                               // 올바른 위치를 선택했는지 판단
		return (x > centerX - radius && x < centerX + radius && y > centerY - radius && y < centerY + radius);
	}
};

ScenePtr scene = nullptr;            // 바탕을 위한 그림, nullptr는 null 포인터 값을 의미
SceneID sceneID;
ObjectID heart1, heart2, heart3;           // 목숨을 위한 그림
const auto check_radius = 25;   // 체크가 중앙에 오도록 하기 위해 뺀 값

class DifferencePoint {
	Rect left_rect, right_rect;
	ObjectPtr left_check, right_check;
	bool shown = false;

public:
	DifferencePoint(int lcx, int rcx, int cy, int r) : left_rect(lcx, cy, r), right_rect(rcx, cy, r) {
		left_check = Object::create("Images/ex_right_64.png", scene, lcx - check_radius, cy - check_radius, false);
		right_check = Object::create("Images/ex_right_64.png", scene, rcx - check_radius, cy - check_radius, false);
	}
	bool checkIn(int x, int y) const {        // 마우스가 올바른 곳을 클릭했는지 판단 - 예/아니요
		return left_rect.checkIn(x, y) || right_rect.checkIn(x, y);        // Rect의 checkIn으로 왼쪽과 오른쪽 중 어디든 상관없이 속하는지만 판단
	}
	void show() {                    // 오른쪽 왼쪽에 초록 그림을 출력함
		left_check->show();
		right_check->show();

		shown = true;
	}

	void wrong_show() {                    // 오른쪽 왼쪽에 빨강 그림을 출력함
		setObjectImage(left_check->ID(), "Images/ex_wrong_64.png");
		setObjectImage(right_check->ID(), "Images/ex_wrong_64.png");
		left_check->show();
		right_check->show();

		shown = true;
	}

	bool found() const { return shown; }
};

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);           // 필요없는 부분 지움
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);    // 필요없는 부분 지움

	scene = Scene::create("틀린그림찾기", "Images/problem.png");               // 장면 생성

	auto timer = Timer::create(10.f);              // 타이머 10초
	showTimer(timer);                                       // 타이머 보이도록 
	auto game_exit = Timer::create(3.f);              // 게임 끝을 위한 타이머 3초

	auto problem = Object::create("Images/ex_1.png", scene, 0, 0);     // 오브젝트가 장면(scene)을 위에 올라가서 가림
	//problem->setImage("Images/problem.png");
	heart1 = createObject("Images/ex_heart_64.png");
	heart2 = createObject("Images/ex_heart_64.png");
	heart3 = createObject("Images/ex_heart_64.png");
	locateObject(heart1, problem->ID(), 1170, 4);
	locateObject(heart2, problem->ID(), 1100, 4);
	locateObject(heart3, problem->ID(), 1030, 4);
	showObject(heart1);
	showObject(heart2);
	showObject(heart3);



	const auto num_of_differences = 7;

	DifferencePoint points[num_of_differences] = {     // 올바른 위치 X, Y 좌표, 각각의 네모 크기가 다름
		{ 520, 1140, 553, 20 },     // 달
		{ 107, 727, 540, 15 },        // 별
		{ 380, 1000, 473, 10 },     // 원숭이 귀
		{ 395, 1015, 164, 27 },     // 색다른 풀
		{ 58, 678, 268, 36 },          // 색다른 텐트
		{ 576, 1196, 421, 35 },     // 깃털
		{ 318, 938, 130, 13 },       // 불의 나무
	};
	DifferencePoint worng_points[3] = {   // 왼x, 오x,  y, 네모 반지름 r / 오x-왼x = 620 / 중앙선 = 640
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	};

	auto count = 0;
	auto life = 3;                                             // 목숨 3개 
	problem->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {    // problem 오브젝트를 클릭을 한 위치
		auto wrong = true;
		for (auto i = 0; i < num_of_differences; ++i) {
			if (points[i].checkIn(x, y)) {          // 마우스가 올바르게 클릭했는가?
				if (!points[i].found()) {              // 이미 클릭된 올바른 위치가 아닌 경우
					points[i].show();                   // 초록 그림 보이는 함수
					count++;
				}
				wrong = false;                             // num_of_differences번 올바른 위치이랑 비교했을 때 하나라도 같으면 false임
			}
		}
		if (wrong) {      //여기서 true 라는 건 올바른 위치랑 비교했을 때 하나도 없었다는 것
			if (x > 640) {  // 오른쪽을 클릭
				worng_points[life - 1] = { x-618, x, y, 0 };
			}
			else {
				worng_points[life - 1] = { x, x+618, y, 0 };
			}
			worng_points[life - 1].wrong_show();
			if(life == 3) hideObject(heart3);
			if (life == 2) hideObject(heart2);
			if (life == 1) hideObject(heart1);
			life--;
		}

		timer->setOnTimerCallback([&](TimerPtr)->bool {
			showMessage("타임 오버 실패 - 3초후 자동 꺼짐");
			game_exit->start();
			return true;
			});
		
		if (life == 0) {
			timer->stop();
			showMessage("목숨 없음 실패 - 3초후 자동 꺼짐");
			game_exit->start();
		}

		if (count == num_of_differences) {
			timer->stop();
			showMessage("성공 - 3초후 자동 꺼짐");
			game_exit->start();
		}

		game_exit->setOnTimerCallback([&](TimerPtr)->bool {
			endGame();
			return true;
			});

		return true;
	});

	//showMessage("좌우에 틀린 곳을 찾아보세요.");
	timer->start();
	startGame(scene);
	return 0;
}
