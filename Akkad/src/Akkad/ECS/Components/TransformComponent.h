#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Akkad {
	struct TransformComponent {
	public:
		TransformComponent() {
			m_Position = glm::vec3(0.0f);
			m_TransformMatrix = glm::mat4(1.0f);
		}

		glm::vec3& GetPosition() {
			return m_Position;
		}

		void SetPostion(glm::vec3 newpos) {
			m_Position = newpos;
			RecalculateTransformMatrix();
		}

	private:
		glm::vec3 m_Position;
		glm::mat4 m_TransformMatrix;

		void RecalculateTransformMatrix() {
			m_TransformMatrix = glm::translate(m_TransformMatrix, m_Position);
		}
	};
}