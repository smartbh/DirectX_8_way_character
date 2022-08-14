#include "stdafx.h"

Boss::Boss()
{
	col = new ObRect();
	col->scale = Vector2(128.0f, 127.0f);
	col->isFilled = false;

	idle = new ObImage(L"boss.bmp");
	idle->scale = Vector2(128.0f, 127.0f);
	idle->maxFrame = Int2(1, 8);
	idle->SetParentRT(*col);

	detect = new ObCircle();
	detect->scale = Vector2(800.0f, 800.0f);
	detect->isFilled = false;
	detect->SetParentRT(*col);

	follow = new ObCircle();
	follow->scale = Vector2(600.0f, 600.0f);
	follow->isFilled = false;
	follow->SetParentRT(*col);

	attack = new ObCircle();
	attack->scale = Vector2(200.0f, 200.0f);
	attack->isFilled = false;
	attack->SetParentRT(*col);

	frameY[Dir_R] = 0;
	frameY[Dir_T] = 4;
	frameY[Dir_L] = 1;
	frameY[Dir_B] = 5;
	frameY[Dir_RB] = 2;
	frameY[Dir_LB] = 6;
	frameY[Dir_LT] = 3;
	frameY[Dir_RT] = 7;
	//우,왼,우하,좌상,상,하,좌하,우상
	bossState = BossState::IDLE;
	attackTime = 0.0f;
}

Boss::~Boss()
{
	SafeDelete(col);
	SafeDelete(idle);
	SafeDelete(detect);
	SafeDelete(follow);
	SafeDelete(attack);
}

void Boss::Update(ObRect* _player)
{
	switch (bossState)
	{
	case BossState::IDLE:
		Idle(_player);
		break;
	case BossState::WALK:
		Walk(_player);
		break;
	case BossState::ATTACK:
		attackStance(_player);
		break;
	}

	col->Update();
	idle->Update();
	detect->Update();
	follow->Update();
	attack->Update();
}

void Boss::Render()
{
	col->Render();
	idle->Render();
	detect->Render();
	follow->Render();
	attack->Render();
}

void Boss::Idle(ObRect* _player)
{
	if (detect->Intersect(_player))
	{
		LookTarget(_player->GetWorldPos(), idle);
		//감지도중 두번째 서클로 들어 온다면
		if (follow->Intersect(_player))
		{
			bossState = BossState::WALK;
		}
	}
}

void Boss::Walk(ObRect* _player)
{
	if (follow->Intersect(_player) && !attack->Intersect(_player))
	{
		LookTarget(_player->GetWorldPos(), idle);
		col->MoveWorldPos(bossDir * 0.5f * DELTA);
	}
	else if(attack->Intersect(_player))
	{
		bossState = BossState::ATTACK;
	}
	else
		bossState = BossState::IDLE;
}

void Boss::attackStance(ObRect* _player)
{
	attackTime += DELTA;

	LookTarget(_player->GetWorldPos(), idle);
	idle->scale.y -= 100.0f * DELTA;
	idle->scale.x += 100.0f * DELTA;
	if (attackTime > 0.5f)
	{
		idle->scale = Vector2(128.0f, 127.0f);
		attackTime = 0.0f;
	}

	if (attack->Intersect(_player))
	{
		bossState = BossState::ATTACK;
	}
	else if (!attack->Intersect(_player) && follow->Intersect(_player))
	{
		idle->scale = Vector2(128.0f, 127.0f);
		bossState = BossState::WALK;
	}
		

}


void Boss::LookTarget(Vector2 target, ObImage* img)
{
	Vector2 dir = target - col->GetWorldPos();
	bossDir = dir;
	float radian = Utility::DirToRadian(dir);

	//오른쪽
	if (-DIV8PI <= radian && radian <= DIV8PI)
	{
		dirState = Dir_R;
	}
	else if (DIV8PI <= radian && radian <= DIV4PI + DIV8PI)
	{
		dirState = Dir_RT;
	}
	else if (DIV4PI + DIV8PI <= radian && radian <= DIV8PI + DIV4PI * 2.0f)
	{
		dirState = Dir_T;
	}
	else if (DIV8PI + DIV4PI * 2.0f <= radian && radian <= DIV8PI + DIV4PI * 3.0f)
	{
		dirState = Dir_LT;
	}
	else if (-(DIV8PI + DIV4PI * 2.0f) >= radian && radian >= -(DIV8PI + DIV4PI * 3.0f))
	{
		dirState = Dir_LB;
	}
	else if (-(DIV8PI + DIV4PI) >= radian && radian >= -(DIV8PI + DIV4PI * 2.0f))
	{
		dirState = Dir_B;
	}
	else if (-(DIV8PI) >= radian && radian >= -(DIV8PI + DIV4PI))
	{
		dirState = Dir_RB;
	}
	else
	{
		dirState = Dir_L;
	}

	img->frame.y = frameY[dirState];
}

ObRect* Boss::getCol()
{
	return col;
}

ObCircle* Boss::getAttackCircle()
{
	return attack;
}
