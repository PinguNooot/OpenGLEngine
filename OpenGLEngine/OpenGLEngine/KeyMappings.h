#pragma once
#include <GLFW/glfw3.h> 

// Define key mappings for easier reference
namespace KeyMappings
{
    const int FORWARD            = GLFW_KEY_W;               // Key to move the camera forward
    const int BACKWARD           = GLFW_KEY_S;               // Key to move the camera backward
    const int LEFT               = GLFW_KEY_A;               // Key to move the camera left
    const int RIGHT              = GLFW_KEY_D;               // Key to move the camera right
    const int UP                 = GLFW_KEY_E;               // Key to move the camera up
    const int DOWN               = GLFW_KEY_Q;               // Key to move the camera down
    const int SPRINT             = GLFW_KEY_LEFT_SHIFT;      // Key to speed up the camera
    const int CROUCH             = GLFW_KEY_LEFT_CONTROL;    // Key to slow down the camera
                                                             
                                                             
    const int FORWARD_ALT        = GLFW_KEY_UP;              // Alternative Key to move the camera forward
    const int BACKWARD_ALT       = GLFW_KEY_DOWN;            // Alternative Key to move the camera backward
    const int LEFT_ALT           = GLFW_KEY_LEFT;            // Alternative Key to move the camera left
    const int RIGHT_ALT          = GLFW_KEY_RIGHT;           // Alternative Key to move the camera right
                                                                                                            
    const int ENABLE_LIGHT       = GLFW_KEY_L;               // Enable / Disable the Light

    const int SKYBOX_CLOUD       = GLFW_KEY_1;               // Switch the Skybox to Stylized Cloud
    const int SKYBOX_CLOUD_NIGHT = GLFW_KEY_2;               // Switch the Skybox to Stylized Cloud Night
    const int SKYBOX_CUPERTIN    = GLFW_KEY_3;               // Switch the Skybox to Cupertin
    const int SKYBOX_MINECRAFT   = GLFW_KEY_0;               // Switch the Skybox to Minecraft
}