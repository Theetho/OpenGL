#pragma once

#include "Loader.h"

class OBJLoader
{
public:
	static Model LoadFromFile(const std::string & objPath, Loader & loader);

private:
	OBJLoader();
	~OBJLoader();

	enum class ParsingState { NONE, VERTEX, TEXTURE, NORMAL, FACE };
	enum class ParsingIndicesState { VERTEX, TEXTURE, NORMAL };
	enum class Vertex { X, Y, Z };
	enum class Texture { X, Y };
	enum class Normal { X, Y, Z };
};

