#include "GUIFactory.h"

#include <Akkad/ECS/Components/Components.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Asset/AssetManager.h>

namespace Akkad {
	using namespace GUI;
	Entity GUIFactory::AddGuiText()
	{
		auto assetmanager = Application::GetAssetManager();

		auto scene = EditorLayer::GetActiveScene();
		CheckGuiContainer();

		Entity text = Entity(scene->m_Registry.create(), scene.get());
		text.AddComponent<RelationShipComponent>();
		text.AddComponent<RectTransformComponent>();
		text.AddComponent<GUITextComponent>();

		auto& uitext = text.GetComponent<GUITextComponent>();
		auto defaultFont = assetmanager->GetFontByName("Roboto-Medium");
		uitext.fontAssetID = defaultFont.assetID;
		uitext.text = "Text";

		auto& tag = text.AddComponent<TagComponent>();
		tag.Tag = "text";
		scene->AssignEntityToParent(scene->GetGuiContainer(), text);

		return text;
	}

	Entity GUIFactory::AddGuiRect()
	{
		auto scene = EditorLayer::GetActiveScene();
		CheckGuiContainer();

		Entity rect = Entity(scene->m_Registry.create(), scene.get());
		rect.AddComponent<RelationShipComponent>();
		rect.AddComponent<RectTransformComponent>();
		auto& tag = rect.AddComponent<TagComponent>();
		tag.Tag = "rect";
		scene->AssignEntityToParent(scene->GetGuiContainer(), rect);

		return rect;
	}

	Entity GUIFactory::AddGuiButton()
	{
		auto scene = EditorLayer::GetActiveScene();
		CheckGuiContainer();

		Entity button = Entity(scene->m_Registry.create(), scene.get());
		button.AddComponent<RelationShipComponent>();
		button.AddComponent<GUIButtonComponent>();
		auto& buttonrect = button.AddComponent<RectTransformComponent>();

		buttonrect.rect.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 0.2 });
		buttonrect.rect.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 0.2 });

		auto& tag = button.AddComponent<TagComponent>();
		tag.Tag = "button";

		scene->AssignEntityToParent(scene->GetGuiContainer(), button);

		Entity text = AddGuiText();
		auto& textcomp = text.GetComponent<GUITextComponent>();
		textcomp.alignment = GUI::GUIText::Alignment::CENTER;
		textcomp.fittingMode = GUI::GUIText::FittingMode::SCALE_TO_FIT;
		textcomp.text = "Button";

		textcomp.color = { 0,0,0 };

		auto& textrect = text.GetComponent<RectTransformComponent>();

		textrect.rect.SetXConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });
		textrect.rect.SetYConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });

		textrect.rect.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 1 });
		textrect.rect.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 0.2 });

		scene->AssignEntityToParent(button, text);

		return button;


	}

	Entity GUIFactory::AddGuiPanel()
	{
		auto scene = EditorLayer::GetActiveScene();
		CheckGuiContainer();

		Entity panel = Entity(scene->m_Registry.create(), scene.get());
		panel.AddComponent<RelationShipComponent>();
		panel.AddComponent<GUIPanelComponent>();
		auto& panelRect = panel.AddComponent<RectTransformComponent>();

		panelRect.rect.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 0.5 });
		panelRect.rect.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 0.2 });

		auto& tag = panel.AddComponent<TagComponent>();
		tag.Tag = "GUI panel";

		scene->AssignEntityToParent(scene->GetGuiContainer(), panel);

		return panel;
	}

	void GUIFactory::CheckGuiContainer()
	{
		Entity container = EditorLayer::GetActiveScene()->GetGuiContainer();

		if (!container.IsValid())
		{
			EditorLayer::GetActiveScene()->AddGuiContainer();
		}
	}
}

