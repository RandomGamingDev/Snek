#ifndef GAME_OBJECTS_HEADER
#define GAME_OBJECTS_HEADER

#include <iostream>
#include <array>

#include <Wireframe.h>
#include "Pixy.h"

#define COLOR_BLACK 0, 0, 0, 0
#define COLOR_RED 1, 0, 0, 0
#define COLOR_GREEN 0, 1, 0, 0
#define BACKGROUND_COLOR COLOR_BLACK
#define APPLE_COLOR COLOR_RED
#define SNAKE_COLOR COLOR_GREEN

enum Direction {
	Up,
	Down,
	Left,
	Right
};

template <typename Tilemap>
void ClearTilemap(Tilemap& tilemap) {
	for (size_t i = 0; i < tilemap.size(); i++)
		for (size_t j = 0; j < tilemap[0].size(); j++)
			for (size_t k = 0; k < tilemap[0][0].size(); k++)
				tilemap[i][j][k] = 0;
}

template <typename Tile>
void SetTileColor(Tile& tile, std::array<float, 4> color) {
	for (size_t i = 0; i < color.size(); i++)
		tile[i] = color[i];
}

template <typename Tile>
inline bool TileIs(Tile& tilemap, std::array<float, 4> color) {
	for (size_t i = 0; i < color.size(); i++)
		if (tilemap[i] != color[i])
			return false;
	return true;
}

template <typename Tilemap>
class Apple {
public:
	Tilemap& tilemap;
	size_t x;
	size_t y;

	Apple(Tilemap& tilemap) : tilemap(tilemap) {
		this->x = 0;
		this->y = 0;
		NewPos();
		Draw();
	}

	void Clear() {
		const std::array<float, 4> clearColor = { BACKGROUND_COLOR };
		for (size_t i = 0; i < clearColor.size(); i++)
			tilemap[x][y][i] = clearColor[i];
	}

	void Draw() {
		SetTileColor(tilemap[x][y], { APPLE_COLOR });
	}

	void NewPos() {
		Clear();
		do {
			x = rand() % tilemap.size();
			y = rand() % tilemap[0].size();
		} while (!TileIs(tilemap[x][y], std::array<float, 4>({BACKGROUND_COLOR})));
		Draw();
	}
};

struct SnakeNode {
	size_t x;
	size_t y;
	SnakeNode* front;
	SnakeNode* back;
	SnakeNode(size_t x, size_t y, SnakeNode* front, SnakeNode* back) {
		this->x = x;
		this->y = y;
		this->front = front;
		this->back = back;
	}
};

template <typename Tilemap>
class Snake {
public:
	Tilemap& tilemap;
	SnakeNode* head;
	SnakeNode* tail;
	Direction direction;
	size_t score;

	Snake(Tilemap& tilemap) : tilemap(tilemap) {
		Default();
	}

	~Snake() {
		Delete();
	}

	void Default() {
		SnakeNode* body = new SnakeNode(4, tilemap[0].size() / 2,
			new SnakeNode(5, tilemap[0].size() / 2, static_cast<SnakeNode*>(nullptr), static_cast<SnakeNode*>(nullptr)),
			new SnakeNode(3, tilemap[0].size() / 2, static_cast<SnakeNode*>(nullptr), static_cast<SnakeNode*>(nullptr)));
		body->front->back = body;
		body->back->front = body;

		this->head = body->front;
		this->tail = body->back;
		this->score = 0;
		Draw();
	}

	void Reset() {
		Delete();
		Default();
	}

	void Clear() {
		SnakeNode* currentNode = head;
		while (currentNode != static_cast<SnakeNode*>(nullptr)) {
			SetTileColor(tilemap[currentNode->x][currentNode->y], { BACKGROUND_COLOR });
			currentNode = currentNode->back;
		}
	}

	void Draw() {
		SnakeNode* currentNode = head;
		while (currentNode != static_cast<SnakeNode*>(nullptr)) {
			SetTileColor(tilemap[currentNode->x][currentNode->y], { SNAKE_COLOR });
			currentNode = currentNode->back;
		}
	}

	template <typename type>
	void Move(Apple<type>& apple) {
		// Grow
		head->back->front = new SnakeNode(head->x, head->y, head, head->back);
		head->back = head->back->front;
		switch (direction) {
			case Direction::Up:
				if (++head->y > tilemap[0].size() - 1) {
					head->y--;
					Reset();
				}
				break;
			case Direction::Down:
				if (--head->y > tilemap[0].size() - 1) {
					head->y++;
					Reset();
				}
				break;
			case Direction::Left:
				if (--head->x > tilemap.size() - 1) {
					head->x++;
					Reset();
				}
				break;
			case Direction::Right:
				if (++head->x > tilemap.size() - 1) {
					head->x--;
					Reset();
				}
				break;
		}
		if (TileIs(tilemap[head->x][head->y], { BACKGROUND_COLOR })) {
			SetTileColor(tilemap[tail->x][tail->y], { BACKGROUND_COLOR });
			tail = tail->front;
			delete tail->back;
			tail->back = static_cast<SnakeNode*>(nullptr);
		}
		else if (TileIs(tilemap[head->x][head->y], { SNAKE_COLOR })) {
			Reset();
		}
		else if (TileIs(tilemap[head->x][head->y], { APPLE_COLOR })) {
			apple.NewPos();
			score++;
		}
		SetTileColor(tilemap[head->x][head->y], { SNAKE_COLOR });
	}

	void Delete() {
		Clear();
		SnakeNode* currentNode = head->back;
		while (currentNode->back != static_cast<SnakeNode*>(nullptr)) {
			delete currentNode->front;
			currentNode = currentNode->back;
		}
		delete currentNode->front;
		delete currentNode;
	}
};

#endif
