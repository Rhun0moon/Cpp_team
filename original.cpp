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

ScenePtr scene = nullptr;            // scene 선언, nullptr는 null 포인터 값을 의미
const auto check_radius = 25;   // 체크가 중앙에 오도록 하기 위해 뺀 값

class DifferencePoint {
	Rect left_rect, right_rect;
	ObjectPtr left_check, right_check;
	bool shown = false;

public:
	DifferencePoint(int lcx, int rcx, int cy, int r) : left_rect(lcx, cy, r), right_rect(rcx, cy, r) {
		left_check = Object::create("Images/check.png", scene, lcx - check_radius, cy - check_radius, false);
		right_check = Object::create("Images/check.png", scene, rcx - check_radius, cy - check_radius, false);
	}
	bool checkIn(int x, int y) const {        // 마우스가 올바른 곳을 클릭했는지 판단 - 예/아니요
		return left_rect.checkIn(x, y) || right_rect.checkIn(x, y);        // Rect의 checkIn으로 왼쪽과 오른쪽 중 어디든 상관없이 속하는지만 판단
	}
	void show() {                    // 오른쪽 왼쪽에 초록그림을 출력함
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

	auto problem = Object::create("Images/problem.png", scene, 0, 0);     // 오브젝트가 장면(scene)을 위에 올라가서 가림
	const auto num_of_differences = 7;

	DifferencePoint points[num_of_differences] = {     // 올바른 위치 X, Y 좌표
		{ 568, 1186, 594, 54 },
		{ 99, 717, 551, 17 },
		{ 383, 1001, 482, 16 },
		{ 401, 1019, 158, 27 },
		{ 61, 679, 255, 36 },
		{ 592, 1210, 421, 35 },
		{ 320, 938, 117, 13 },
	};

	auto count = 0;
	auto life = 3;                                             // 목숨 3개 
	problem->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {    // problem 오브젝트를 클릭을 한 위치
		auto wrong = true;    // 클릭했는지 안했는지 알도록
		for (auto i = 0; i < num_of_differences; ++i) {
			if (points[i].checkIn(x, y)) {          // 마우스가 올바르게 클릭했는가?
				if (!points[i].found()) {              // 이미 클릭된 올바른 위치가 아닌 경우
					points[i].show();                   // 초록 그림 보이는 함수
					count++;
				}
				wrong = false;                             // 잘 돌아가고 있는 상태
			}
		}

		if (wrong) {
			endGame();
		}

		if (count == num_of_differences) {
			showMessage("성공, 다 찾았다!!!");
		}

		return true;
	});

	showMessage("좌우에 틀린 곳을 찾아보세요.");
	startGame(scene);

	return 0;
}
