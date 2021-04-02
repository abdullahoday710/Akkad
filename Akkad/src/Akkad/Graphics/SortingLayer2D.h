#pragma once
#include "Akkad/core.h"

#include <string>
#include <vector>

namespace Akkad {

	struct SortingLayer2D
	{
		SortingLayer2D() {}
		SortingLayer2D(std::string layerName) { name = layerName; }
		std::string name;
	};

	class SortingLayer2DHandler {
	public:
		SortingLayer2DHandler(std::string layerName) { m_Name = layerName; }
		static std::vector<SortingLayer2D>& GetRegisteredLayers() { return s_RegisteredLayers; }
		static void RegisterLayer(std::string layerName);
		static void ClearRegisteredLayers();
	private:
		std::string m_Name;
		static std::vector<SortingLayer2D> s_RegisteredLayers;
	};
}
