#include "AStarSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"
#include "Actor/AStarPlayer.h"

#include <string>

AStarSimulatorLevel::AStarSimulatorLevel()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();

	player = new AStarPlayer(Vector2(minX, minY));
	AddActor(player);
}

AStarSimulatorLevel::~AStarSimulatorLevel()
{
}

void AStarSimulatorLevel::Tick(float deltaTime)
{
	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}

	// 스페이스바 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		pause = !pause;
	}

	if (pause)
	{
		if (Input::Get().GetKeyDown('c') || Input::Get().GetKeyDown('C'))
		{
			AllClear();
			inProcess = 0;
			pause = false;
		}
		return;
	}

	super::Tick(deltaTime);

	if (inProcess)
	{
		if (inProcess == 1)
		{
			StepAStar();
			return;
		}
		else if (inProcess == 2)
		{
			if (Input::Get().GetKeyDown(VK_RETURN))
			{
				AllClear();
				inProcess = 0;
			}
			return;
		}
	}
}

void AStarSimulatorLevel::Render()
{
	if(pause) Game::Get().WriteToBuffer(Vector2::Zero, "Pause", Color::Red);
	else if(inProcess == 1) Game::Get().WriteToBuffer(Vector2::Zero, "Processing", Color::Red);
	else if(inProcess == 2) Game::Get().WriteToBuffer(Vector2::Zero, "Finished", Color::Red);
	else Game::Get().WriteToBuffer(Vector2::Zero, "A Star Simulator Level", Color::White);

	if (pause) Game::Get().WriteToBuffer(Vector2(0, 1), "일시 정지 해제 : SPACE", Color::Red);
	else if (inProcess);
	else Game::Get().WriteToBuffer(Vector2(0, 1), "S : 시작 지점, W : 벽, G : 목표 지점, C : 초기화, SPACE : 일시 정지, ENTER : 시뮬레이터 시작", Color::White);

	// 스크린 테두리
	Vector2 drawPosition(minX - 1, minY - 1);
	int width = maxX - minX + 3;
	int height = maxY - minY + 1;

	std::string screenLine(width, '#');

	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), Color::White);
	drawPosition.y++;
	Vector2 drawPosition2(maxX + 1, drawPosition.y);

	for (int i = 0; i < height; i++)
	{
		Game::Get().WriteToBuffer(drawPosition, "#", Color::White);
		Game::Get().WriteToBuffer(drawPosition2, "#", Color::White);
		drawPosition.y++;
		drawPosition2.y++;
	}
	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), Color::White);

	super::Render();
}

void AStarSimulatorLevel::SetGoal(Vector2 targetPosition)
{
	// 그 자리에 이미 다른 액터가 있다면 그 액터를 삭제
	DeleteObject(targetPosition);
	
	// 이미 골을 표시하는 액터가 있다면 그 액터를 이동
	if (goal) goal->SetPosition(targetPosition);
	// 없다면 생성
	else
	{
		goal = new Actor("G", Color::GreenIntensity, targetPosition);
		goal->SetSortingOrder(2);
		AddActor(goal);
	}
}

void AStarSimulatorLevel::SetWall(Vector2 targetPosition)
{
	DeleteObject(targetPosition);
	Actor* wall = new Actor("#", Color::White, targetPosition);
	// 벽을 의미하는 id 는 1
	wall->SetId(1);
	AddActor(wall);
}

void AStarSimulatorLevel::SetStart(Vector2 targetPosition)
{
	// 그 자리에 이미 다른 액터가 있다면 그 액터를 삭제
	DeleteObject(targetPosition);

	// 이미 시작점을 표시하는 액터가 있다면 그 액터를 이동
	if (start) start->SetPosition(targetPosition);
	// 없다면 생성
	else
	{
		start = new Actor("S", Color::RedIntensity, targetPosition);
		start->SetSortingOrder(2);
		AddActor(start);
	}
}

void AStarSimulatorLevel::DeleteObject(Vector2 targetPosition)
{
	for (Actor* actor : actors)
	{
		if (actor->Position() == targetPosition && actor != (Actor*)player)
		{
			if (actor == goal) goal = nullptr;
			else if (actor == start) start = nullptr;
			actor->Destroy();
			break;
		}
	}
}

void AStarSimulatorLevel::AllClear()
{
	for (Actor* actor : actors)
	{
		actor->Destroy();
	}
	player = nullptr;
	goal = nullptr;
	start = nullptr;

	player = new AStarPlayer(Vector2(minX, minY));
	AddActor(player);
}

void AStarSimulatorLevel::Simulator()
{
	if (!goal || !start) return;

	player->Destroy();
	player = nullptr;
	StartAStar();
}

void AStarSimulatorLevel::StartAStar()
{
	// open 큐 초기화
	Vector2 goalPos = goal->Position();
	open = decltype(open)(CompareNodePtr(goalPos));

	// 시작 노드 생성 (g = 0)
	Vector2 startPos = start->Position();
	AStarNode* startNode = new AStarNode(startPos, 0, goalPos);

	AddActor(startNode);

	open.push(startNode);
	inProcess = 1;
}

void AStarSimulatorLevel::StepAStar()
{
	// 큐가 비어있으면 경로 불가능
	if (open.empty()) {
		Finish(); // 경로 없음
		return;
	}

	// open 에서 하나 꺼내기
	AStarNode* current = open.top();
	open.pop();

	// 목표 도착 여부 확인
	if (current->Position() == goal->Position())
	{
		Finish(current);
		return;
	}

	// 현재 노드 방문 처리
	current->visited = true;

	// 4. 이웃 좌표 정의 (상, 하, 좌, 우)
	static const int dx[4] = { 1, -1, 0, 0 };
	static const int dy[4] = { 0, 0, 1, -1 };

	for (int i = 0; i < 4; i++)
	{
		Vector2 neighborPos(current->Position().x + dx[i],
			current->Position().y + dy[i]);

		// ① 맵 경계 확인
		if (neighborPos.x < minX || neighborPos.x > maxX ||
			neighborPos.y < minY || neighborPos.y > maxY)
			continue;

		// ② 벽 체크 (actors에 "#" 있는지 검사)
		bool isWall = false;
		for (Actor* actor : actors) {
			if (actor->Position() == neighborPos &&
				actor->GetId() == 1) {
				isWall = true;
				break;
			}
		}
		if (isWall) continue;

		// ③ 이미 방문한 노드인지 확인
		bool visited = false;
		for (Actor* actor : actors) {
			AStarNode* node = dynamic_cast<AStarNode*>(actor);
			if (node && node->Position() == neighborPos && node->visited) {
				visited = true;
				break;
			}
		}
		if (visited) continue;

		// ④ 새 이웃 노드 생성
		AStarNode* neighbor = new AStarNode(neighborPos, current->GetG() + 1, goal->Position());
		neighbor->SetParent(current);
		AddActor(neighbor);

		// ⑤ open 큐에 push
		open.push(neighbor);
	}
}

void AStarSimulatorLevel::Finish(AStarNode* node)
{
	// 경로 색칠
	inProcess = 2;
	AStarNode* cur = node;

	while (cur->GetParent())
	{
		cur->SetSortingOrder(1);
		cur->ChangeColor(Color::GreenIntensity);
		cur = cur->GetParent();
	}
}

void AStarSimulatorLevel::Finish()
{
	inProcess = 2;
}
