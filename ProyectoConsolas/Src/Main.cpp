#include "Platform/Platform.h"
#include "Renderer/RendererThread.h"

#include "Logic/Waves.h"

#include <stdlib.h>		/* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>		/* fopen */
#include <iostream>		/* cout */		

using namespace std;

//Atributos de la ventana
const int NUM_BUFFERS = 1;							//PC : 1(Modo ventana) O 2(Modo fullscreen). PS4 2-16 //TODO: DETECTAR ERROR?
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;						//PS4: Tiene que tener relación de aspecto 16:9. Solo se tiene en cuenta el height
const Color SCREEN_CLEAR_COLOR = { 0,0,0,255 };

//Waves
//const float ENERGY_WAVE = 31/ 32;
const int HEIGHT_WAVE = 5000;

//Tiempo minimo y maximo entre 1 gota y la siguiente
const int MIN_FRAMES_BETWEEN_WAVES = 50;
const int MAX_FRAMES_BETWEEN_WAVES = 150;

enum ImageType { DOOR_FRAME, DOORS, CLIENT, THIEF, DOLLARS, BANG, SIZE };

void LoadResources(Image** images)
{
	//TODO: Esto esta bien o un array de string o qué?

	//Pared con puerta. //TODO: 3 De ellas consecutivas crean la escena principal de juego
	images[DOOR_FRAME] = Platform::LoadImage("SpritesSheetsWestBank/marcoPuerta.rgba");

	//Contiene las cuatro posibles posiciones de una puerta. Todos los sprites tienen el mismo ancho.
	//La zona abierta no debería pintarse, sino sustituirse por el personaje que abre la puerta
	images[DOORS] = Platform::LoadImage("SpritesSheetsWestBank/puertas.rgba");

	//Distintas "poses" de ambos personajes
	//Tienen el mismo ancho que el ancho que deja una puerta completamente abierta
	images[CLIENT] = Platform::LoadImage("SpritesSheetsWestBank/client.rgba");
	images[THIEF] = Platform::LoadImage("SpritesSheetsWestBank/ladron.rgba");

	//Distintos "sprites" de las "cajas" que indican el estado de una puerta. (Visible o No) (Recibe ingreso o no)
	images[DOLLARS] = Platform::LoadImage("SpritesSheetsWestBank/dolares.rgba");

	//Distintos "frames" de la animación de "Bang" mostrada cuando se recibe un disparo de un ladrón
	images[BANG] = Platform::LoadImage("SpritesSheetsWestBank/bang.rgba");
}

/*
	Aplicación que muestra un efecto de lluvia que cae sobre una imagen de la facultad.
	Multihebra: 1 Lógica y 1 Render -> Cada una va a un nucleo
*/
int main(int argc, char* args[])
{
	//Inicialización
	Platform::Init(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_BUFFERS);

	//Inicializa la semilla de aleatoriedad random.
	srand(time(NULL));

	//Carga de recursos. Asumen una resolución de juego de 640 x 360
	Image* images[ImageType::SIZE];
	LoadResources(images);

	//Se lanza la hebra de renderizado
	RendererThread* rendererThread = new RendererThread(); // TODO: NO HACER NEW PARA CREAR LA CLASE RENDERTRHEAD
	rendererThread->Start();

	//Inicialización ondas
	//Waves* waves = new Waves(background, HEIGHT_WAVE, MIN_FRAMES_BETWEEN_WAVES, MAX_FRAMES_BETWEEN_WAVES);

	//Creación de comandos iniciales
	RenderCommand clearCommand;
	clearCommand.Type = RendererCommandType::CLEAR;
	clearCommand.Param.ClearParams.Color = SCREEN_CLEAR_COLOR;

	RenderCommand presentCommand;
	presentCommand.Type = RendererCommandType::END_FRAME;

	//Limpieza inicial de los buffers
	for (int i = 0; i < NUM_BUFFERS; i++)
	{
		rendererThread->EnqueueCommand(clearCommand);
		rendererThread->EnqueueCommand(presentCommand);
	}

	//Contención inicial. No empieza el bucle principal hasta que se limpien todos los buffers
	while (rendererThread->GetPendingFrames() > 0)
		;

	int tick = 0;

	RenderCommand drawRectCommand;
	drawRectCommand.Type = RendererCommandType::DRAW_RECT;
	drawRectCommand.Param.DrawRectParams.Image = images[ImageType::BANG];
	drawRectCommand.Param.DrawRectParams.PosX = 50;
	drawRectCommand.Param.DrawRectParams.PosY = 30;
	drawRectCommand.Param.DrawRectParams.Width = drawRectCommand.Param.DrawRectParams.Image->GetWidth();
	drawRectCommand.Param.DrawRectParams.Height = drawRectCommand.Param.DrawRectParams.Image->GetHeight();
	drawRectCommand.Param.DrawRectParams.OffsetX =0;
	drawRectCommand.Param.DrawRectParams.OffsetY = 0;

	//Bucle principal. Hebra lógica.
	while (Platform::Tick())
	{
		//Input(); // No en esta práctica
		//rendererThread->EnqueueCommand(clearCommand); // En esta práctica no se hace clear para mantener la coherencia de frames

		//Paso de simulación
		//waves->Update(tick);

		//Contención. Se para la hebra si el render va muy retrasado
		while (rendererThread->GetPendingFrames() >= NUM_BUFFERS)
			;

		//Comunicación logica con render: se le envia a la hebra de render el comando con las nuevas condiciones de la logica.
		//rendererThread->EnqueueCommand(waves->GetRenderCommand());


		rendererThread->EnqueueCommand(drawRectCommand);

		rendererThread->EnqueueCommand(presentCommand);

		//Delay
		tick++;
	}

	rendererThread->Stop();

	//delete waves;
	//waves = nullptr;

	delete rendererThread;
	rendererThread = nullptr;

	for (int i = 0; i < ImageType::SIZE; i++)
	{
		delete images[i];
		images[i] = nullptr;
	}

	Platform::Release();

	return 0;
}
