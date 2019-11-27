#include "RendererThread.h"
#include "Renderer.h"

using namespace std;

void RendererThread::Start() 
{
	if (t != nullptr)
		Stop();

	Renderer::Init();

	//Recibe la funcion a ejecutar
	t = new thread(&RendererThread::RenderLoop, this);

	quitRequested = false;
	pendingFrames = 0;
}

void RendererThread::Stop() {
	if (t != nullptr) {
		quitRequested = true;

		//Esperamos a que acabe.
		t->join();

		delete t;
		t = nullptr;

		Renderer::Release();
	}
}

void RendererThread::EnqueueCommand(RenderCommand renderCommand)
{
	if (renderCommand.Type == RendererCommandType::END_FRAME)
		pendingFrames++;

	commandQueue.push(renderCommand);
}

void RendererThread::RenderLoop() {
	while (!quitRequested)
	{
		RenderCommand sigCommand = commandQueue.pop();

		while (sigCommand.Type != RendererCommandType::END_FRAME)
		{
			switch (sigCommand.Type)
			{
			case RendererCommandType::CLEAR:
				Renderer::Clear(sigCommand.Param.ClearParams.Color);
				break;
			case RendererCommandType::PUT_PIXEL:
				Renderer::PutPixel(sigCommand.Param.PutPixelParams.X, sigCommand.Param.PutPixelParams.Y, sigCommand.Param.PutPixelParams.Color);
				break;
			case RendererCommandType::RENDER_RAIN_EFFECT:
				DrawRain(sigCommand.Param.RainParams.Background, sigCommand.Param.RainParams.HeightDiff, sigCommand.Param.RainParams.ForcePaint);
				break;
			}

			sigCommand = commandQueue.pop();
		}

		pendingFrames--;
		Renderer::Present();
	}
}


//en waveheight hay que desplazar todo a la izq y en el bit
//menos significativo meter un 0 o un 1 en funcion de si hay que cambiar o no el pixel
void RendererThread::DrawRain(Color* background, int* heightDiffs, bool forcePaint)
{
	int height = Renderer::GetHeight();
	int width = Renderer::GetWidth();

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Color color = background[i * width + j];
			
			bool heightChanged = (heightDiffs[i * width + j] & 1) == 1;

			//Solo se modifica el color si ha cambiado la altura
			if (forcePaint || heightChanged)
			{
				int heightDiff = heightDiffs[i * width + j] >> 1;

				//Se obtiene el color en funci�n de la altura de la onda y la imagen de fondo
				color.R = Clamp(0, 255, (color.R - heightDiff));
				color.G = Clamp(0, 255, (color.G - heightDiff));
				color.B = Clamp(0, 255, (color.B - heightDiff));

				Renderer::PutPixel(j, i, color);
			}
		}
	}

}

int RendererThread::Clamp(int min, int max, int value)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}
