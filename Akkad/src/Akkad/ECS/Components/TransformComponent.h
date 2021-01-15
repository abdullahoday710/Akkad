#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Akkad {
	struct TransformComponent {
	public:
		TransformComponent() {
			m_Position = glm::vec3(0.0f);
			m_Rotation = glm::vec3(0.0f);
			m_TransformMatrix = glm::mat4(1.0f);
			RecalculateTransformMatrix();
		}

		glm::vec3& GetPosition() {
			return m_Position;
		}

		glm::vec3& GetRotation() {
			return m_Rotation;
		}

		glm::mat4& GetTransformMatrix() {
			return m_TransformMatrix;
		}

		void SetPostion(glm::vec3 newpos) {
			m_Position = newpos;
			RecalculateTransformMatrix();
		}

		void SetRotation(glm::vec3 rotation) {
			m_Rotation = rotation;
			RecalculateTransformMatrix();
		}

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::mat4 m_TransformMatrix;

		void RecalculateTransformMatrix() {
			m_TransformMatrix = glm::mat4(1.0f);
			m_TransformMatrix = glm::translate(m_TransformMatrix, m_Position);

			m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.x, { 1,0,0 });
			m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.y, { 0,1,0 });
			m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.z, { 0,0,1 });
		}

		friend class Scene;
	};
}