class RendererPS4
{
	//TODO:Poner atributos
	//Los m�todos son est�ticos
	static void Clear();

	/*
		Pone un Pixel en el RenderBuffer. No aparece en pantalla hasta que se hace present
	*/
	static void PutPixel();

	/*
		Muestra en pantalla el RenderBuffer.
	*/
	static void Present();
};