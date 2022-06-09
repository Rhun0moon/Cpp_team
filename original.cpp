#include <bangtal>
using namespace bangtal;

class Rect {     // Ʋ�� ���� ������ �ű��� -> ��ü? ����ü?�� ����
	int centerX, centerY;
	int radius;

public:
	Rect(int cx, int cy, int r) : centerX(cx), centerY(cy), radius(r) {};        // �ùٸ� ��ġ ����??
	bool checkIn(int x, int y) const {                                                               // �ùٸ� ��ġ�� �����ߴ��� �Ǵ�
		return (x > centerX - radius && x < centerX + radius && y > centerY - radius && y < centerY + radius);
	}
};

ScenePtr scene = nullptr;            // scene ����, nullptr�� null ������ ���� �ǹ�
const auto check_radius = 25;   // üũ�� �߾ӿ� ������ �ϱ� ���� �� ��

class DifferencePoint {
	Rect left_rect, right_rect;
	ObjectPtr left_check, right_check;
	bool shown = false;

public:
	DifferencePoint(int lcx, int rcx, int cy, int r) : left_rect(lcx, cy, r), right_rect(rcx, cy, r) {
		left_check = Object::create("Images/check.png", scene, lcx - check_radius, cy - check_radius, false);
		right_check = Object::create("Images/check.png", scene, rcx - check_radius, cy - check_radius, false);
	}
	bool checkIn(int x, int y) const {        // ���콺�� �ùٸ� ���� Ŭ���ߴ��� �Ǵ� - ��/�ƴϿ�
		return left_rect.checkIn(x, y) || right_rect.checkIn(x, y);        // Rect�� checkIn���� ���ʰ� ������ �� ���� ������� ���ϴ����� �Ǵ�
	}
	void show() {                    // ������ ���ʿ� �ʷϱ׸��� �����
		left_check->show();
		right_check->show();

		shown = true;
	}
	bool found() const { return shown; }
};

int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);           // �ʿ���� �κ� ����
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);    // �ʿ���� �κ� ����

	scene = Scene::create("Ʋ���׸�ã��", "Images/problem.png");               // ��� ����

	auto problem = Object::create("Images/problem.png", scene, 0, 0);     // ������Ʈ�� ���(scene)�� ���� �ö󰡼� ����
	const auto num_of_differences = 7;

	DifferencePoint points[num_of_differences] = {     // �ùٸ� ��ġ X, Y ��ǥ
		{ 568, 1186, 594, 54 },
		{ 99, 717, 551, 17 },
		{ 383, 1001, 482, 16 },
		{ 401, 1019, 158, 27 },
		{ 61, 679, 255, 36 },
		{ 592, 1210, 421, 35 },
		{ 320, 938, 117, 13 },
	};

	auto count = 0;
	auto life = 3;                                             // ��� 3�� 
	problem->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {    // problem ������Ʈ�� Ŭ���� �� ��ġ
		auto wrong = true;    // Ŭ���ߴ��� ���ߴ��� �˵���
		for (auto i = 0; i < num_of_differences; ++i) {
			if (points[i].checkIn(x, y)) {          // ���콺�� �ùٸ��� Ŭ���ߴ°�?
				if (!points[i].found()) {              // �̹� Ŭ���� �ùٸ� ��ġ�� �ƴ� ���
					points[i].show();                   // �ʷ� �׸� ���̴� �Լ�
					count++;
				}
				wrong = false;                             // �� ���ư��� �ִ� ����
			}
		}

		if (wrong) {
			endGame();
		}

		if (count == num_of_differences) {
			showMessage("����, �� ã�Ҵ�!!!");
		}

		return true;
	});

	showMessage("�¿쿡 Ʋ�� ���� ã�ƺ�����.");
	startGame(scene);

	return 0;
}
