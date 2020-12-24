#pragma once
#include <json.hpp>

namespace Akkad {

	class ProjectSerializer
	{
	public:
		static bool NewProject(std::string name, std::string path);
	};

}

