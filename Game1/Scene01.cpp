#include "stdafx.h"

void Scene01::Init()
{
	pl = new Player();
	bs = new Boss();
}

void Scene01::Release()
{
}

void Scene01::Update()
{
	pl->Update(bs->getAttackCircle(), bs->getCol());
	bs->Update(pl->getCol());
	

}

void Scene01::LateUpdate()
{
	
}

void Scene01::Render()
{
	pl->Render();
	bs->Render();
}

void Scene01::ResizeScreen()
{

}
