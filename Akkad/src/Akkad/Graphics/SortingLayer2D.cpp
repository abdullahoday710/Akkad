#include "SortingLayer2D.h"

namespace Akkad {
	std::vector<SortingLayer2D> SortingLayer2DHandler::s_RegisteredLayers;

	void SortingLayer2DHandler::RegisterLayer(std::string layerName)
	{
		for (auto it : s_RegisteredLayers)
		{
			if (it.name == layerName)
			{
				AK_ASSERT(false, "Trying to register a layer with a name that is already taken !");
				return;
			}
		}

		SortingLayer2D layer;
		layer.name = layerName;
		s_RegisteredLayers.push_back(layer);
	}

	void SortingLayer2DHandler::ClearRegisteredLayers()
	{
		s_RegisteredLayers.clear();
	}
}