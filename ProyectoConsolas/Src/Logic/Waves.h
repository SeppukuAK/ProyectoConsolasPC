//#pragma once
//
//#include "../Renderer/Color.h"
//#include "../Renderer/RendererThread.h"
//
///*
//	Simulaci�n del agua. Aparecen en posiciones y en intervalos aleatorios.
//	Controla la coherencia de frames
//*/
//class Waves
//{
//private:
//	Color* _background;										//Matriz de colores del fondo
//
//	//Par�metros de la gota
//	int _heightWave;										//Altura de la onda que se establece cuando cae una gota
//	int _minFramesBetweenWaves, _maxFramesBetweenWaves;		//Tiempo minimo y maximo entre 1 gota y la siguiente
//
//	//Informaci�n simulaci�n
//	int width, height;										//Dimensiones de la onda
//	int numBuffers;											//N�mero de buffers (uno m�s que renderBuffers)
//
//	//Estado
//	int nextWaveFrame;										//Frame en el que suceder� la proxima onda
//	int** diffBuffers;										//Array de buffers de diferencia de alturas de la simulaci�n(2-17)
//	int * currentHeightBuffer;								//Buffer que contiene el estado actual de alturas
//	int * oldHeightBuffer;									//Buffer que contiene el estado anterior de alturas
//	RenderCommand renderCommand;							//Comando de lluvia que se mandar� al render
//
//	//Referencias auxiliares
//	int * currentDiffBuffer;
//	int * oldDiffBuffer;
//
//public:
//
//	/*
//		Llama a Init
//	*/
//	Waves(Color* background, int heightWave, int minFramesBetweenWaves, int maxFramesBetweenWaves);
//
//	/*
//		Libera memoria
//	*/
//	~Waves();
//
//	/*
//		Avanza en el estado de la lluvia
//		Coloca una gota nueva cada cierto tiempo.
//	*/
//	void Update(int delta);
//
//	/*
//		Devuelve el comando de pintado de la lluvia actual
//	*/
//	RenderCommand GetRenderCommand() { return renderCommand; };
//
//private:
//	/*
//		Inicializa la lluvia y crea el comando de dibujado de la lluvia
//	*/
//	void Init();
//
//	/*
//		Evoluciona un pixel, actualizando su altura.
//	*/
//	void UpdatePixelHeight(const int & x, const int & y);
//
//	/*
//		Evoluciona un pixel, actualizando su diferencia
//	*/
//	void UpdatePixelDiff(const int & x, const int & y);
//
//};
//
