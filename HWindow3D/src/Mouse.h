#pragma once


#include "Win.h"

#include <queue>

class Mouse
{
	friend class WindowsWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Enter, Leave, Invalid
		};
	private:
		Type type;
		int x, y;
		bool leftIsPressed;
		bool rightIsPressed;
	public:
		Event()
			: type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), x(0), y(0)
		{}
		Event(Type type, const Mouse& parent)
			: type(type), leftIsPressed(parent.leftIsPressed), rightIsPressed(parent.rightIsPressed), x(parent.x), y(parent.y)
		{}
		bool IsValid() const { return type != Type::Invalid; }
		Type GetType() const { return type; }
		std::pair<int, int> GetPos() const { return { x, y }; }
		int GetPosX() const { return x; }
		int GetPosY() const { return y; }
		bool LeftIsPressed() const { return leftIsPressed; }
		bool RightIsPressed() const { return rightIsPressed; }
	}; 
public:
	Mouse() = default;
	NONCOPYABLE(Mouse)
	
	inline std::pair<int, int> GetPos() const { return { x, y }; }
	inline int GetPosX() const { return x; }
	inline int GetPosY() const { return y; }
	inline bool IsInWindow() const { return isInWindow; }
	inline bool LeftIsPressed() const { return leftIsPressed; }
	inline bool RightIsPressed() const { return rightIsPressed; }
	inline  int GetWheelDelta() const { return wheelDeltaCarry; }
	Mouse::Event Read();
	inline bool IsEmpty() const { return buffer.empty(); }
	void Flush();
private:
	void OnMouseMove(int x, int y);
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void TrimBuffer();
	void OnWheelDelta(int x, int y, int delta);
private:
	static constexpr unsigned int bufferSize = 16u;
	int x, y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};
