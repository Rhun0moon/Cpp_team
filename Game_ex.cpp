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

ScenePtr scene = nullptr;            // ������ ���� �׸�, nullptr�� null ������ ���� �ǹ�
SceneID sceneID;
ObjectID heart1, heart2, heart3;           // ����� ���� �׸�
const auto check_radius = 25;   // üũ�� �߾ӿ� ������ �ϱ� ���� �� ��

class DifferencePoint {
	Rect left_rect, right_rect;
	ObjectPtr left_check, right_check;
	bool shown = false;

public:
	DifferencePoint(int lcx, int rcx, int cy, int r) : left_rect(lcx, cy, r), right_rect(rcx, cy, r) {
		left_check = Object::create("Images/ex_right_64.png", scene, lcx - check_radius, cy - check_radius, false);
		right_check = Object::create("Images/ex_right_64.png", scene, rcx - check_radius, cy - check_radius, false);
	}
	bool checkIn(int x, int y) const {        // ���콺�� �ùٸ� ���� Ŭ���ߴ��� �Ǵ� - ��/�ƴϿ�
		return left_rect.checkIn(x, y) || right_rect.checkIn(x, y);        // Rect�� checkIn���� ���ʰ� ������ �� ���� ������� ���ϴ����� �Ǵ�
	}
	void show() {                    // ������ ���ʿ� �ʷ� �׸��� �����
		left_check->show();
		right_check->show();

		shown = true;
	}

	void wrong_show() {                    // ������ ���ʿ� ���� �׸��� �����
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
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);           // �ʿ���� �κ� ����
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);    // �ʿ���� �κ� ����

	scene = Scene::create("Ʋ���׸�ã��", "Images/problem.png");               // ��� ����

	auto timer = Timer::create(10.f);              // Ÿ�̸� 10��
	showTimer(timer);                                       // Ÿ�̸� ���̵��� 
	auto game_exit = Timer::create(3.f);              // ���� ���� ���� Ÿ�̸� 3��

	auto problem = Object::create("Images/problem_ex.png", scene, 0, 0);     // ������Ʈ�� ���(scene)�� ���� �ö󰡼� ����
	heart1 = createObject("Images/ex_heart_64.png");
	heart2 = createObject("Images/ex_heart_64.png");
	heart3 = createObject("Images/ex_heart_64.png");
	locateObject(heart1, problem->ID(), 1180, 24);
	locateObject(heart2, problem->ID(), 1120, 24);
	locateObject(heart3, problem->ID(), 1060, 24);
	showObject(heart1);
	showObject(heart2);
	showObject(heart3);



	const auto num_of_differences = 7;

	DifferencePoint points[num_of_differences] = {     // �ùٸ� ��ġ X, Y ��ǥ, ������ �׸� ũ�Ⱑ �ٸ�
		{ 568, 1186, 594, 54 },     // ��
		{ 99, 717, 551, 17 },          // ��
		{ 383, 1001, 482, 16 },     // ������ ��
		{ 401, 1019, 158, 27 },     // ���ٸ� Ǯ
		{ 61, 679, 255, 36 },          // ���ٸ� ��Ʈ
		{ 592, 1210, 421, 35 },     // ����
		{ 320, 938, 117, 13 },       // ���� ����
	};
	DifferencePoint worng_points[3] = {   // ��x, ��x,  y, �׸� ������ r / ��x-��x = 618 / �߾Ӽ� = 640
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	};

	auto count = 0;
	auto life = 3;                                             // ��� 3�� 
	problem->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {    // problem ������Ʈ�� Ŭ���� �� ��ġ
		auto wrong = true;
		for (auto i = 0; i < num_of_differences; ++i) {
			if (points[i].checkIn(x, y)) {          // ���콺�� �ùٸ��� Ŭ���ߴ°�?
				if (!points[i].found()) {              // �̹� Ŭ���� �ùٸ� ��ġ�� �ƴ� ���
					points[i].show();                   // �ʷ� �׸� ���̴� �Լ�
					count++;
				}
				wrong = false;                             // num_of_differences�� �ùٸ� ��ġ�̶� ������ �� �ϳ��� ������ false��
			}
		}
		if (wrong) {      //���⼭ true ��� �� �ùٸ� ��ġ�� ������ �� �ϳ��� �����ٴ� ��
			if (x > 640) {  // �������� Ŭ��
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
			showMessage("Ÿ�� ���� ���� - 3���� �ڵ� ����");
			game_exit->start();
			return true;
			});
		
		if (life == 0) {
			showMessage("��� ���� ���� - 3���� �ڵ� ����");
			game_exit->start();
		}

		if (count == num_of_differences) {
			timer->stop();
			showMessage("���� - 3���� �ڵ� ����");
			game_exit->start();
		}

		game_exit->setOnTimerCallback([&](TimerPtr)->bool {
			endGame();
			return true;
			});

		return true;
	});

	//showMessage("�¿쿡 Ʋ�� ���� ã�ƺ�����.");
	timer->start();
	startGame(scene);
	return 0;
}
