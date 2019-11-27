#include "Waves.h"
#include "../Renderer/Renderer.h"
#include <stdlib.h>		/* srand, rand */

Waves::Waves(Color* background, int heightWave, int minFramesBetweenWaves, int maxFramesBetweenWaves) :
	_background(background), _heightWave(heightWave),
	_minFramesBetweenWaves(minFramesBetweenWaves), _maxFramesBetweenWaves(maxFramesBetweenWaves)
{
	Init();
}

Waves::~Waves()
{
	//Limpiamos memoria
	for (int i = 0; i < numBuffers; ++i)
		delete[] buffers[i];

	delete[] buffers;
	buffers = nullptr;
	delete[] diffHeight;
	diffHeight = nullptr;
}

void Waves::Init()
{
	//Calculo del offset de posición del fuego
	width = Renderer::GetWidth();
	height = Renderer::GetHeight();

	bufferSize = width * height;
	numBuffers = Renderer::GetNumBuffers() + 1;
	bufferIndex = 0;

	//Cada valor representa la altura de la onda
	buffers = new int* [numBuffers];

	for (int i = 0; i < numBuffers; ++i)
	{
		buffers[i] = new int[bufferSize];
		for (int j = 0; j < bufferSize; j++)
			buffers[i][j] = 0;
	}
	diffHeight = new int[bufferSize];

	for (int i = 0; i < bufferSize; ++i)
	{
		diffHeight[i] = 0;
	}

	//Generamos el frame en el que se generará la primera onda
	nextWaveFrame = _minFramesBetweenWaves + (rand() % (_maxFramesBetweenWaves - _minFramesBetweenWaves));

	//Inicialización comando de renderizado de la lluvia
	renderCommand.Type = RendererCommandType::RENDER_RAIN_EFFECT;
	renderCommand.Param.RainParams.Background = _background;
	renderCommand.Param.RainParams.HeightDiff = diffHeight;
}

void Waves::Update(int delta)
{
	bufferIndex = delta % numBuffers;

	//Actualiza todos los pixeles del buffer actual
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			UpdatePixel(j, i);
		}
	}

	//Se genera onda si coincide el tick actual con el aleatorio
	if (delta == nextWaveFrame)
	{
		//Calculo posiciones aleatorias
		int x = rand() % width;
		int y = rand() % height;

		//Se mete el valor en el buffer actual
		buffers[bufferIndex][y * width + x] = _heightWave;

		//Generamos el frame en el que se generará la primera onda
		nextWaveFrame = delta + _minFramesBetweenWaves + (rand() % (_maxFramesBetweenWaves - _minFramesBetweenWaves));
	}

	//Actualiza dibujado
	UpdateRenderCommand(delta);
}

void Waves::UpdatePixel(int x, int y)
{
	int oneFrameOldIndex = bufferIndex - 1;

	if (oneFrameOldIndex < 0)
		oneFrameOldIndex = numBuffers - 1;

	//Se suma cada una de las direcciones multiplicadas por 0.5
	int sumHeight = 0;
	for (auto dir : dirs)
	{
		int adjX = x + dir.First;
		int adjY = y + dir.Second;
		if (Correct(adjX, adjY))
			sumHeight += buffers[oneFrameOldIndex][adjY * width + adjX];	//Aritmetica entera
	}

	sumHeight = sumHeight >> 1;

	int twoFramesOldIndex = bufferIndex - 2;

	if (twoFramesOldIndex < 0)
		twoFramesOldIndex = numBuffers + twoFramesOldIndex;

	//Al resultado se le suma el valor de hace 2 frames
	buffers[bufferIndex][y * width + x] = sumHeight - buffers[twoFramesOldIndex][y * width + x];

	//Le reducimos un poco la altura por la perdida de energia
	// v *= 31/32
	buffers[bufferIndex][y * width + x] -= (buffers[bufferIndex][y * width + x] >> 5);	//Aritmetica entera
}

void Waves::UpdateRenderCommand(int delta)
{
	//Si no estan inicializados todos los buffers, se pinta toda la pantalla
	if (delta < numBuffers)
		renderCommand.Param.RainParams.ForcePaint = true;

	//Si están inicializados, se dibujan en función del numero de Buffers
	else
		renderCommand.Param.RainParams.ForcePaint = false;

	//Actualiza vector de diferencias
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int leftOld = 0;
			int rightOld = 0;
			int leftCurrent = 0;
			int rightCurrent = 0;

			int bufferAntIndex = (bufferIndex + 1) % numBuffers;

			//Calculamos las diferencias entre el antiguo frame y el actual
			if (Correct(j - 1, i))
			{
				leftOld = buffers[bufferAntIndex][i * width + j - 1];
				leftCurrent = buffers[bufferIndex][i * width + j - 1];
			}

			if (Correct(j + 1, i))
			{
				rightOld = buffers[bufferAntIndex][i * width + j + 1];
				rightCurrent = buffers[bufferIndex][i * width + j + 1];
			}

			int heightDiffCurrent = leftCurrent - rightCurrent;//incr
			int heightDiffOld = leftOld - rightOld;//incr

			//Si son diferentes, se tendrá que pintar
			if (heightDiffCurrent != heightDiffOld)
			{
				heightDiffCurrent = heightDiffCurrent << 1;
				heightDiffCurrent |= 1;
			}
			else
				heightDiffCurrent = heightDiffCurrent << 1;

			diffHeight[i * width + j] = heightDiffCurrent;
		}
	}

	renderCommand.Param.RainParams.HeightDiff = diffHeight;
}

bool Waves::Correct(int x, int y)
{
	return (x >= 0 && x < width && y >= 0 && y < height);
}