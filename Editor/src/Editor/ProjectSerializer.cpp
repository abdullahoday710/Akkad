#include "ProjectSerializer.h"

#include <fstream>
#include <iomanip>
namespace Akkad {
	using json = nlohmann::json;

	bool ProjectSerializer::NewProject(std::string name, std::string path)
	{
		json data;
		std::ofstream output;
		data["project"]["name"] = name;

		output.open(path + "/"+name+".AKPROJ");
		output << std::setw(4) << data << std::endl;
		output.close();
		return true;
	}
}