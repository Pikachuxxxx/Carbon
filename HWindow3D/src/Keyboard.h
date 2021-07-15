#pragma once

#include "Win.h"

#include <queue>
#include <bitset>

class Keyboard
{
    friend class Window;
public:
    class Event
    {

    };
public:
    Keyboard() = default;
    //NONCOPYABLE(Keyboard)

    // Key Event stuff
    bool KeyIsPressed(unsigned char keycode) const;
    Event ReadKey();
    bool KeyIsEmpty() const;
    void FlushKey();

    // Char Event stuff
    char ReadChar();
    bool CharIsEmpty() const;
    void FlushChar();
    void Flush();

    // Auto repeat control
    void EnableAutoRepeat();
    void DisabeleAutoRepeat();
    bool AutoRepeadIsEnabled();
private:
    void OnKeyPressed(unsigned char keycode);
    void OnKeyReleased(unsigned char keycode);
    void OnChar(char character);
    void ClearState();
    template <typename T>
    static void TrimBuffer(std:::queue<T>& buffer);
private:
    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    bool autoRepeatEnabled = false;
    std::bitset<nKeys> keystates;
    std::queue<Event> keybuffer;
    std::queue<char> charbuffer;
};

