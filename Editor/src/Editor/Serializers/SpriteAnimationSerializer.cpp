#pragma once
#include "SpriteAnimationSerializer.h"

#include "Editor/EditorLayer.h"

#include <Akkad/Graphics/Sprite.h>

#include <fstream>
#include <json.hpp>

namespace Akkad {

	void SpriteAnimationSerializer::Serialize(SharedPtr<Graphics::SpriteAnimation> animation, std::string animationName, std::string outputPath)
	{
		auto& project = EditorLayer::GetActiveProject();
		nlohmann::json animation_data;

		animation_data["Name"] = animationName;
		animation_data["SpriteSheetID"] = animation->spriteSheetAssetID;
		animation_data["StartingRow"] = animation->startingRow;
		animation_data["StartingColumn"] = animation->startingColumn;
		animation_data["EndRow"] = animation->endRow;
		animation_data["Interval"] = animation->interval;

		std::ofstream output;
		output.open(outputPath, std::ios::trunc);
		output << std::setw(4) << animation_data << std::endl;
		output.close();


	}
}

