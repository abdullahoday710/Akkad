#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Akkad {
	struct TransformComponent {
	public:
		TransformComponent() {
			m_Position = glm::vec3(0.0f);
			m_ParentPosition = glm::vec3(0.0f);
			m_Rotation = glm::vec3(0.0f);
			m_ParentRotation = glm::vec3(0.0f);
			m_Scale = glm::vec3(1.0f);
			m_TransformMatrix = glm::mat4(1.0f);
			RecalculateTransformMatrix();
		}

		glm::vec3 GetPosition() {
			return m_Position + m_ParentPosition;
		}

		glm::vec3 GetRotation() {
			return m_Rotation;
		}

		glm::vec3 GetScale() {
			return m_Scale;
		}

		glm::mat4& GetTransformMatrix() {
			return m_TransformMatrix;
		}

		void SetPostion(glm::vec3 newpos) {
			m_Position = newpos;
		}

		void SetRotation(glm::vec3 rotation) {
			m_Rotation = rotation;
		}

		void SetScale(glm::vec3 scale) {
			m_Scale = scale;
		}

	private:
		glm::vec3 m_Position;
		glm::vec3 m_ParentPosition;
		glm::vec3 m_Rotation;
		glm::vec3 m_ParentRotation;
		glm::vec3 m_Scale;
		glm::mat4 m_TransformMatrix;

		void RecalculateTransformMatrix() {
			m_TransformMatrix = glm::mat4(1.0f);
			m_TransformMatrix = glm::translate(m_TransformMatrix, m_Position + m_ParentPosition);
			m_TransformMatrix = glm::scale(m_TransformMatrix, m_Scale);

			m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.x + m_ParentRotation.x, { 1,0,0 });
			m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.y + m_ParentRotation.y, { 0,1,0 });
			m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.z + m_ParentRotation.z, { 0,0,1 });
		}

		friend class Scene;
		friend class PropertyEditorPanel;
		friend class ViewPortPanel;
		friend class TransformComponentSerializer;
	};
}