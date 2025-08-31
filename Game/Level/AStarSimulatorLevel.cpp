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
	Vector2 goalPos = goal->Position();
	open = decltype(open)(CompareNodePtr(goalPos));
	Vector2 startPos = start->Position();
	auto* startNode = new AStarNode(startPos, 0, goalPos);
	gScore.clear(); closed.clear();
	gScore[startPos] = 0;

	AddActor(startNode);
	open.push(startNode);
	inProcess = 1;
}

void AStarSimulatorLevel::StepAStar()
{
	// 유효 노드를 찾을 때까지 pop (프레임당 1개 확장)
	while (!open.empty())
	{
		// open 에서 하나 꺼내기
		AStarNode* current = open.top();
		open.pop();
		Vector2 cpos = current->Position();

		// 이미 확정(closed)이면 스킵 (lazy deletion)
		if (closed.count(cpos)) continue;

		// 낡은 엔트리면 스킵 (큐 안에 더 좋은 g 가 있었던 경우)
		auto itg = gScore.find(cpos);
		if (itg != gScore.end() && current->GetG() > itg->second) continue;

		// 목표 도착 여부 확인
		if (cpos == goal->Position())
		{
			Finish(current);
			return;
		}

		// 현재 노드 확정 처리
		current->visited = true;      // (시각화용)
		closed.insert(cpos);

		// 4방 이웃
		static const int dx[4] = { 1, -1, 0, 0 };
		static const int dy[4] = { 0, 0, 1, -1 };

		for (int i = 0; i < 4; i++)
		{
			Vector2 npos(cpos.x + dx[i], cpos.y + dy[i]);

			// ① 맵 경계 확인
			if (npos.x < minX || npos.x > maxX || npos.y < minY || npos.y > maxY)
				continue;

			// ② 벽 체크 (actors에 "#" 있는지 검사: GetId()==1)
			bool isWall = false;
			for (Actor* a : actors)
			{
				if (a->Position() == npos && a->GetId() == 1) {
					isWall = true;
					break;
				}
			}
			if (isWall) continue;

			// 이미 확정이면 패스
			if (closed.count(npos)) continue;

			// g 후보
			const int tentative = current->GetG() + 1;

			// 더 좋을 때만 갱신/푸시
			auto itg2 = gScore.find(npos);
			if (itg2 == gScore.end() || tentative < itg2->second)
			{
				gScore[npos] = tentative;

				AStarNode* neighbor = new AStarNode(npos, tentative, goal->Position());
				neighbor->SetParent(current);
				AddActor(neighbor);

				open.push(neighbor);
			}
		}

		// 이 프레임에서는 유효 노드 1개만 확장
		return;
	}

	// 큐를 다 비웠는데도 목표를 못 찾았으면 실패
	Finish(); // 경로 없음
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
