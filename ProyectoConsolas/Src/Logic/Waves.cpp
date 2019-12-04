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
		delete[] diffBuffers[i];

	delete[] diffBuffers;
	diffBuffers = nullptr;

	delete[] currentHeightBuffer;
	currentHeightBuffer = nullptr;

	delete[] oldHeightBuffer;
	oldHeightBuffer = nullptr;
}

void Waves::Init()
{
	width = Renderer::GetWidth();
	height = Renderer::GetHeight();
	numBuffers = Renderer::GetNumBuffers() + 1;

	//Inicialización buffers
	int bufferSize = width * height;

	diffBuffers = new int*[numBuffers];

	for (int i = 0; i < numBuffers; ++i)
	{
		diffBuffers[i] = new int[bufferSize];
		for (int j = 0; j < bufferSize; j++)
			diffBuffers[i][j] = 0;
	}

	currentHeightBuffer = new int[bufferSize];
	oldHeightBuffer = new int[bufferSize];

	for (int i = 0; i < bufferSize; ++i)
	{
		currentHeightBuffer[i] = 0;
		oldHeightBuffer[i] = 0;
	}

	currentDiffBuffer = nullptr;
	oldDiffBuffer = nullptr;

	//Generamos el frame en el que se generará la primera onda
	nextWaveFrame = _minFramesBetweenWaves + (rand() % (_maxFramesBetweenWaves - _minFramesBetweenWaves));

	//Inicialización comando de renderizado de la lluvia
	renderCommand.Type = RendererCommandType::RENDER_RAIN_EFFECT;
	renderCommand.Param.RainParams.Background = _background;
	renderCommand.Param.RainParams.HeightDiff = nullptr;
}

void Waves::Update(int delta)
{
	//Obtenención del índice actual necesario para la actualización
	int bufferIndex = delta % numBuffers;

	//Obtención de las referencias a los buffers de diferencias actuales
	currentDiffBuffer = diffBuffers[bufferIndex];
	oldDiffBuffer = diffBuffers[(bufferIndex + 1) % numBuffers];

	//Actualiza todos los pixeles del buffer actual
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			UpdatePixelHeight(j, i);
		}
	}

	//Se genera onda si coincide el tick actual con el aleatorio
	if (delta == nextWaveFrame)
	{
		//Calculo posiciones aleatorias
		int x = rand() % width;
		int y = rand() % height;

		//Generamos el frame en el que se generará la siguiente onda
		nextWaveFrame = delta + _minFramesBetweenWaves + (rand() % (_maxFramesBetweenWaves - _minFramesBetweenWaves));

		//Se mete el valor en el buffer actual
		currentHeightBuffer[y * width + x] = _heightWave;
	}

	//Actualiza la matriz de diferencias para el dibujado
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			UpdatePixelDiff(j, i);
		}
	}

	//Swap de buffers
	int * temp = currentHeightBuffer;
	currentHeightBuffer = oldHeightBuffer;
	oldHeightBuffer = temp;

	//Actualización comando renderizado

	//Si no estan inicializados todos los buffers, se rellena todo el buffer
	//Si están inicializados, se dibujan en función del numero de Buffers
	renderCommand.Param.RainParams.ForcePaint = (delta < (numBuffers - 1));

	renderCommand.Param.RainParams.HeightDiff = currentDiffBuffer;
}

void Waves::UpdatePixelHeight(const int & x, const int & y)
{
	//Se suma cada una de las direcciones multiplicadas por 0.5
	int sumHeight = 0;

	if (((x - 1) >= 0) && ((x + 1) < width) && ((y - 1) >= 0) && ((y + 1) < height))
	{
		sumHeight = oldHeightBuffer[y * width + x - 1]
			+ oldHeightBuffer[y * width + x + 1]
			+ oldHeightBuffer[(y - 1) * width + x]
			+ oldHeightBuffer[(y + 1) * width + x];

		sumHeight = sumHeight >> 1;
	}

	int pixelIndex = y * width + x;

	//Al resultado se le suma el valor de hace 2 frames
	currentHeightBuffer[pixelIndex] = sumHeight - currentHeightBuffer[pixelIndex];

	//Le reducimos un poco la altura por la perdida de energia
	// v *= 31/32
	currentHeightBuffer[pixelIndex] -= (currentHeightBuffer[pixelIndex] >> 5);	//Aritmetica entera
}

void Waves::UpdatePixelDiff(const int & x, const int & y)
{
	int leftCurrent = 0;
	int rightCurrent = 0;

	int pixelIndex = y * width + x;

	//Obtención de las diferencias
	if (x - 1 >= 0)
		leftCurrent = currentHeightBuffer[pixelIndex - 1];

	if (x + 1 < width)
		rightCurrent = currentHeightBuffer[pixelIndex + 1];

	int heightDiffCurrent = leftCurrent - rightCurrent;//diferencia de alturas
	int heightDiffOld = oldDiffBuffer[pixelIndex] >> 1;

	//Si son diferentes, se tendrá que pintar
	if (heightDiffCurrent != heightDiffOld)
	{
		heightDiffCurrent = heightDiffCurrent << 1;
		heightDiffCurrent |= 1;
	}
	else
		heightDiffCurrent = heightDiffCurrent << 1;

	currentDiffBuffer[pixelIndex] = heightDiffCurrent;
}

