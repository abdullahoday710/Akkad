#include "GUISliderComponentSerializer.h"

#include "Akkad/ECS/Components/GUISliderComponent.h"
namespace Akkad 
{
	void GUISliderComponentSerializer::Serialize(Entity entity, json& entity_data)
	{
		auto& slider = entity.GetComponent<GUISliderComponent>();
		entity_data["GUISliderComponent"]["SliderColor"][0] = slider.slider.GetSliderColor().r;
		entity_data["GUISliderComponent"]["SliderColor"][1] = slider.slider.GetSliderColor().g;
		entity_data["GUISliderComponent"]["SliderColor"][2] = slider.slider.GetSliderColor().b;

		entity_data["GUISliderComponent"]["KnobColor"][0] = slider.slider.GetKnobColor().r;
		entity_data["GUISliderComponent"]["KnobColor"][1] = slider.slider.GetKnobColor().g;
		entity_data["GUISliderComponent"]["KnobColor"][2] = slider.slider.GetKnobColor().b;
	}

	void GUISliderComponentSerializer::Deserialize(Entity entity, json& component_data)
	{
		glm::vec3 sliderColor;
		sliderColor.r = component_data["SliderColor"][0];
		sliderColor.g = component_data["SliderColor"][1];
		sliderColor.b = component_data["SliderColor"][2];

		glm::vec3 knobColor;
		knobColor.r = component_data["KnobColor"][0];
		knobColor.g = component_data["KnobColor"][1];
		knobColor.b = component_data["KnobColor"][2];

		auto& slider = entity.AddComponent<GUISliderComponent>();

		slider.slider.SetKnobColor(knobColor);
		slider.slider.SetSliderColor(sliderColor);
	}
}

