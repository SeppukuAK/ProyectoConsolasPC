#include "DeathBackground.h"
#include "../Renderer/RendererThread.h"


DeathBackground::DeathBackground() : Entity(0, 0)
{
}

void DeathBackground::Render()
{
	if (changedState)
		RendererThread::EnqueueClearCommand();
}