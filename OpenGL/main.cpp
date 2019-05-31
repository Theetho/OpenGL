#include <pch.h>
#include <GameManager.h>
#undef main // SDL macro to create a main function

int main(int argc, char * argv[])
{
	GameManager app;
	app.Run();
	return 0;
}

