#include "Camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
		: position(startPosition), worldUp(startUp), yaw(startYaw), pitch(startPitch), moveSpeed(startMoveSpeed), turnSpeed(startTurnSpeed), front(0.0f, 0.0f, 0.0f) 
{
	update();  // Asegura que `front` se inicializa correctamente
	//Debug();
}

Camera::Camera() {
	cout << "Camera created" << endl;
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	if (EnableMovementKeys) {
		if (keys[GLFW_KEY_W]) position += front * velocity;

		if (keys[GLFW_KEY_S]) position -= front * velocity;

		if (keys[GLFW_KEY_A]) position -= right * velocity;

		if (keys[GLFW_KEY_D]) position += right * velocity;

		if (keys[GLFW_KEY_Q]) position -= up * velocity;

		if (keys[GLFW_KEY_E]) position += up * velocity;
	}
}

void Camera::scrollControl(GLfloat yChange, GLfloat deltaTime)
{
	// cambiamos la velocidad de la c�mara
	moveSpeed += yChange * 0.05f;
	if (moveSpeed < 0.1f) moveSpeed = 0.1f;
	if (moveSpeed > 10.0f) moveSpeed = 10.0f;
}

void Camera::mouseButtons(int* buttons)
{
	EnableMovementKeys = buttons[GLFW_MOUSE_BUTTON_RIGHT] == 1;
	scrollableWindow = buttons[GLFW_MOUSE_BUTTON_LEFT] == 1;
	// ocultamos el mouse
	if (scrollableWindow || EnableMovementKeys) glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
	else glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	if (!EnableMovementKeys) return;

	xChange *= turnSpeed;
	yChange *= turnSpeed;
	
	yaw += xChange;
	pitch += yChange;

	// Limita el pitch para evitar la inversi�n de la c�mara
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	update();  // Actualiza la direcci�n de la c�mara	
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

void Camera::setCameraPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

void Camera::setCameraDirection(glm::vec3 newDirection)
{
	front = newDirection;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update() {
	// Calcula el nuevo vector front
	if (scrollableWindow) {
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	}
	else if (EnableMovementKeys) {
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);
	}

	// Recalcula el vector right y el vector up
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::Debug()
{
	cout << "Camera position: " << position.x << " " << position.y << " " << position.z << endl;
	cout << "Camera front: " << front.x << " " << front.y << " " << front.z << endl;
	cout << "Camera up: " << up.x << " " << up.y << " " << up.z << endl;
	cout << "Camera right: " << right.x << " " << right.y << " " << right.z << endl;
	cout << "Camera yaw: " << yaw << endl;
	cout << "Camera pitch: " << pitch << endl;
	cout << "Camera moveSpeed: " << moveSpeed << endl;
	cout << "Camera turnSpeed: " << turnSpeed << endl;
}

Camera::~Camera()
{
}

