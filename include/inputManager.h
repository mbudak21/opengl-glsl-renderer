#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <unordered_map>

class InputManager {
private:
    std::unordered_map<unsigned char, bool> keys;

public:
    void keyPress(unsigned char key) {
        if (key >= 'A' && key <= 'Z') {
            key = key + 32; // Convert to lowercase
        }
        keys[key] = true;
    }

    void keyRelease(unsigned char key) {
        if (key >= 'A' && key <= 'Z') {
            key = key + 32; // Convert to lowercase
        }
        keys[key] = false;
    }

    bool isPressed(unsigned char key) const {
        if (key >= 'A' && key <= 'Z') {
            key = key + 32; // Convert to lowercase
        }
        // keys[key] doesn't 
        auto it = this->keys.find(key);
        return it != this->keys.end() && it->second;
    }
    void shiftPress() {
        keys[1] = true;
    }
    void shiftRelease() {
        keys[1] = false;
    }
    void ctrlPress() {
        keys[2] = true; 
    }
    void ctrlRelease() {
        keys[2] = false; 
    }

    bool shiftPressed() const{
        return isPressed(1);
    }
    bool ctrlPressed() const{
        return isPressed(2);
    }
    
};
#endif