#include <iostream>
#include <stdio.h>
#include <ctime>
#include <chrono>
#include <fmt/core.h>

#include <Wireframe.h>
#include "Pixy.h"

#include "GameObjects.h"

#ifdef WINDOW_API_GLFW
#define VALID
#endif

#ifndef VALID
#error Sorry, but Snek doesn't support the current Window API that you're using
#endif

typedef std::array<std::array<std::array<float, 4>, 32>, 32> Tilemap;

template <typename type>
void OnTick(Graphics* graphics, std::chrono::steady_clock::time_point& timer, Pixy<type>& pixy, Tilemap& tilemap, Snake<Tilemap>& snake, Apple<Tilemap>& apple) {
	snake.Move<Tilemap>(apple);
	
	graphics->SetTitle(fmt::format(fmt::runtime(
		"Snek {}"
	), snake.score).c_str());

	pixy.Update(tilemap.data());
	timer = std::chrono::high_resolution_clock::now();                                                                                       
}

void TestGraphics(Graphics* graphics) {
	std::chrono::steady_clock::time_point timer = std::chrono::high_resolution_clock::now();

	Tilemap tilemap = Tilemap();
	Snake<Tilemap> snake = Snake(tilemap);
	Apple<Tilemap> apple = Apple(tilemap);
	Pixy pixy = Pixy(tilemap.data(), sizeof(tilemap), 0, graphics->width, graphics->height, tilemap.size(), tilemap[0].size());
	while (!graphics->WindowShouldClose()) {
		graphics->ClearColor(BACKGROUND_COLOR);
		graphics->Clear(GL_COLOR_BUFFER_BIT);

		{ // Input
			if (glfwGetKey(graphics->window, GLFW_KEY_W) == GLFW_PRESS && !(snake.head->back->y == snake.head->y + 1))
				snake.direction = Direction::Up;
			if (glfwGetKey(graphics->window, GLFW_KEY_A) == GLFW_PRESS && !(snake.head->back->x == snake.head->x - 1))
				snake.direction = Direction::Left;
			if (glfwGetKey(graphics->window, GLFW_KEY_S) == GLFW_PRESS && !(snake.head->back->y == snake.head->y - 1))
				snake.direction = Direction::Down;
			if (glfwGetKey(graphics->window, GLFW_KEY_D) == GLFW_PRESS && !(snake.head->back->x == snake.head->x + 1))
				snake.direction = Direction::Right;
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timer).count() > 125)
			OnTick(graphics, timer, pixy, tilemap, snake, apple);

		pixy.Draw();

		graphics->SwapBuffers();
		graphics->PollEvents();
	}
}

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));
	Graphics::WindowLibInit();
	Graphics::WindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	Graphics::WindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	Graphics::WindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	Graphics graphics;
	graphics.Init("Snek 0", 800, 800, nullptr, TestGraphics, nullptr);
}