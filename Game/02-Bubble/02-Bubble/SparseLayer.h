#pragma once
#ifndef _SPARSELAYER_INCLUDE
#define _SPARSELAYER_INCLUDE
#include <vector>
class SparseLayer
{
	struct VariableTile
	{
		int index;
		int x, y, depth;
		int width, height;
		// Atributs del tile
		// Per exemple:
		bool bWall;
	};
	protected:
		std::vector<VariableTile*> map;
	public:
		void render();

};

#endif // _SPARSELAYER_INCLUDE