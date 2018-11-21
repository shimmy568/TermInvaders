#pragma once

enum KeyCode { UP, DOWN, LEFT, RIGHT, SPACE };

class KeyboardListener {
    public:
    virtual void KeyPressed(KeyCode code) {};
};
