///
///  @file Input.cpp
///  @brief globals that deals with control input

#include "Input.h"

using namespace kelp;

GLFWwindow * Input::window = NULL; // Needs to be set

std::vector<int> Input::Keyboard::s_keys = std::vector<int>();
std::vector<int> Input::Keyboard::s_keys_p = std::vector<int>();//pressed
std::vector<int> Input::Keyboard::s_keys_r = std::vector<int>();//released
std::vector<int> Input::Keyboard::s_keys_h = std::vector<int>();//held-down


bool Input::Keyboard::s_initInput = false;

void Input::Keyboard::init()
{
    s_keys.push_back(KEY_SPACE         );
    s_keys.push_back(KEY_APOSTROPHE    );
    s_keys.push_back(KEY_COMMA         );
    s_keys.push_back(KEY_MINUS         );
    s_keys.push_back(KEY_PERIOD        );
    s_keys.push_back(KEY_SLASH         );
    s_keys.push_back(KEY_0             );
    s_keys.push_back(KEY_1             );
    s_keys.push_back(KEY_2             );
    s_keys.push_back(KEY_3             );
    s_keys.push_back(KEY_4             );
    s_keys.push_back(KEY_5             );
    s_keys.push_back(KEY_6             );
    s_keys.push_back(KEY_7             );
    s_keys.push_back(KEY_8             );
    s_keys.push_back(KEY_9             );
    s_keys.push_back(KEY_SEMICOLON     );
    s_keys.push_back(KEY_EQUAL         );
    s_keys.push_back(KEY_A             );
    s_keys.push_back(KEY_B             );
    s_keys.push_back(KEY_C             );
    s_keys.push_back(KEY_D             );
    s_keys.push_back(KEY_E             );
    s_keys.push_back(KEY_F             );
    s_keys.push_back(KEY_G             );
    s_keys.push_back(KEY_H             );
    s_keys.push_back(KEY_I             );
    s_keys.push_back(KEY_J             );
    s_keys.push_back(KEY_K             );
    s_keys.push_back(KEY_L             );
    s_keys.push_back(KEY_M             );
    s_keys.push_back(KEY_N             );
    s_keys.push_back(KEY_O             );
    s_keys.push_back(KEY_P             );
    s_keys.push_back(KEY_Q             );
    s_keys.push_back(KEY_R             );
    s_keys.push_back(KEY_S             );
    s_keys.push_back(KEY_T             );
    s_keys.push_back(KEY_U             );
    s_keys.push_back(KEY_V             );
    s_keys.push_back(KEY_W             );
    s_keys.push_back(KEY_X             );
    s_keys.push_back(KEY_Y             );
    s_keys.push_back(KEY_Z             );
    s_keys.push_back(KEY_LEFT_BRACKET  );
    s_keys.push_back(KEY_BACKSLASH     );
    s_keys.push_back(KEY_RIGHT_BRACKET );
    s_keys.push_back(KEY_GRAVE_ACCENT  );
    s_keys.push_back(KEY_WORLD_1       );
    s_keys.push_back(KEY_WORLD_2       );

    s_keys.push_back(KEY_ESCAPE        );
    s_keys.push_back(KEY_ENTER         );
    s_keys.push_back(KEY_TAB           );
    s_keys.push_back(KEY_BACKSPACE     );
    s_keys.push_back(KEY_INSERT        );
    s_keys.push_back(KEY_DELETE        );
    s_keys.push_back(KEY_RIGHT         );
    s_keys.push_back(KEY_LEFT          );
    s_keys.push_back(KEY_DOWN          );
    s_keys.push_back(KEY_UP            );
    s_keys.push_back(KEY_PAGE_UP       );
    s_keys.push_back(KEY_PAGE_DOWN     );
    s_keys.push_back(KEY_HOME          );
    s_keys.push_back(KEY_END           );
    s_keys.push_back(KEY_CAPS_LOCK     );
    s_keys.push_back(KEY_SCROLL_LOCK   );
    s_keys.push_back(KEY_NUM_LOCK      );
    s_keys.push_back(KEY_PRINT_SCREEN  );
    s_keys.push_back(KEY_PAUSE         );
    s_keys.push_back(KEY_F1            );
    s_keys.push_back(KEY_F2            );
    s_keys.push_back(KEY_F3            );
    s_keys.push_back(KEY_F4            );
    s_keys.push_back(KEY_F5            );
    s_keys.push_back(KEY_F6            );
    s_keys.push_back(KEY_F7            );
    s_keys.push_back(KEY_F8            );
    s_keys.push_back(KEY_F9            );
    s_keys.push_back(KEY_F10           );
    s_keys.push_back(KEY_F11           );
    s_keys.push_back(KEY_F12           );
    s_keys.push_back(KEY_F13           );
    s_keys.push_back(KEY_F14           );
    s_keys.push_back(KEY_F15           );
    s_keys.push_back(KEY_F16           );
    s_keys.push_back(KEY_F17           );
    s_keys.push_back(KEY_F18           );
    s_keys.push_back(KEY_F19           );
    s_keys.push_back(KEY_F20           );
    s_keys.push_back(KEY_F21           );
    s_keys.push_back(KEY_F22           );
    s_keys.push_back(KEY_F23           );
    s_keys.push_back(KEY_F24           );
    s_keys.push_back(KEY_F25           );
    s_keys.push_back(KEY_KP_0          );
    s_keys.push_back(KEY_KP_1          );
    s_keys.push_back(KEY_KP_2          );
    s_keys.push_back(KEY_KP_3          );
    s_keys.push_back(KEY_KP_4          );
    s_keys.push_back(KEY_KP_5          );
    s_keys.push_back(KEY_KP_6          );
    s_keys.push_back(KEY_KP_7          );
    s_keys.push_back(KEY_KP_8          );
    s_keys.push_back(KEY_KP_9          );
    s_keys.push_back(KEY_KP_DECIMAL    );
    s_keys.push_back(KEY_KP_DIVIDE     );
    s_keys.push_back(KEY_KP_MULTIPLY   );
    s_keys.push_back(KEY_KP_SUBTRACT   );
    s_keys.push_back(KEY_KP_ADD        );
    s_keys.push_back(KEY_KP_ENTER      );
    s_keys.push_back(KEY_KP_EQUAL      );
    s_keys.push_back(KEY_LEFT_SHIFT    );
    s_keys.push_back(KEY_LEFT_CONTROL  );
    s_keys.push_back(KEY_LEFT_ALT      );
    s_keys.push_back(KEY_LEFT_SUPER    );
    s_keys.push_back(KEY_RIGHT_SHIFT   );
    s_keys.push_back(KEY_RIGHT_CONTROL );
    s_keys.push_back(KEY_RIGHT_ALT     );
    s_keys.push_back(KEY_RIGHT_SUPER   );
    s_keys.push_back(KEY_MENU          );
    s_initInput = true;
}

void Input::Keyboard::update(int _key, int _action)
{
    if(!s_initInput)
        init();
    for(int i = 0; i < s_keys.size(); i++)
        if(_key == s_keys[i])
        {
            switch(_action)
            {
                case GLFW_PRESS:
                    s_keys_p.push_back(_key);
                    s_keys_h.push_back(_key);
                    break;
                case GLFW_RELEASE:
                    s_keys_r.push_back(_key);
                    for(int j = 0; j < s_keys_h.size(); j++)
                        if(s_keys_h[j] == _key)
                            s_keys_h.erase(s_keys_h.begin() + j);
                    break;
            }
            break;
        }
}

void Input::Keyboard::clear()
{
    s_keys_p.clear();
    s_keys_r.clear();
}

void Input::Keyboard::printKeyList()
{
    for(int i = 0; i < s_keys_p.size(); i++)
        printf("%d pressed\n", s_keys_p[i]);
    
    for(int i = 0; i < s_keys_r.size(); i++)
        printf("%d released\n", s_keys_r[i]);
    
    for(int i = 0; i < s_keys_h.size(); i++)
        printf("%d held\n", s_keys_h[i]);
}

bool Input::Keyboard::is(KeyboardKey _kk, KeyboardAction _ka)
{
    switch(_ka)
    {
        case KeyboardAction::PRESSED :
            for(int i = 0; i < s_keys_p.size(); i++)
                if(s_keys_p[i] == _kk)
                    return true;
            break;
            
        case KeyboardAction::RELEASED :
            for(int i = 0; i < s_keys_r.size(); i++)
                if(s_keys_r[i] == _kk)
                    return true;
            break;
            
        case KeyboardAction::HELD :
            for(int i = 0; i < s_keys_h.size(); i++)
                if(s_keys_h[i] == _kk)
                    return true;
            break;
    }
    return false;
}


std::vector<int> Input::Mouse::s_keys = std::vector<int>();
std::vector<int> Input::Mouse::s_keys_p = std::vector<int>();//pressed
std::vector<int> Input::Mouse::s_keys_r = std::vector<int>();//released
std::vector<int> Input::Mouse::s_keys_h = std::vector<int>();//held-down

bool Input::Mouse::s_initInput = false;
double Input::Mouse::x = 0;
double Input::Mouse::y = 0;

double Input::Mouse::xLastFrame = 0;
double Input::Mouse::yLastFrame = 0;

double Input::Mouse::dx = 0;
double Input::Mouse::dy = 0;
bool Input::Mouse::joyStickMode = false;

void Input::Mouse::init()
{
    s_keys.push_back(MOUSE_BUTTON_1      );
    s_keys.push_back(MOUSE_BUTTON_2      );
    s_keys.push_back(MOUSE_BUTTON_3      );
    s_keys.push_back(MOUSE_BUTTON_4      );
    s_keys.push_back(MOUSE_BUTTON_5      );
    s_keys.push_back(MOUSE_BUTTON_6      );
    s_keys.push_back(MOUSE_BUTTON_7      );
    s_keys.push_back(MOUSE_BUTTON_8      );
    s_keys.push_back(MOUSE_BUTTON_LAST   );
    s_keys.push_back(MOUSE_BUTTON_LEFT   );
    s_keys.push_back(MOUSE_BUTTON_RIGHT  );
    s_keys.push_back(MOUSE_BUTTON_MIDDLE );

    s_initInput = true;
    //glfwSetCursorPos(window,Config::s_windowWidth/2, Config::s_windowHeight/2);
}

void Input::Mouse::update(int _key, int _action)
{
    if(!s_initInput)
        init();
    for(int i = 0; i < s_keys.size(); i++)
        if(_key == s_keys[i])
        {
            switch(_action)
            {
                case GLFW_PRESS:
                    s_keys_p.push_back(_key);
                    s_keys_h.push_back(_key);
                    break;
                case GLFW_RELEASE:
                    s_keys_r.push_back(_key);
                    for(int j = 0; j < s_keys_h.size(); j++)
                        if(s_keys_h[j] == _key)
                            s_keys_h.erase(s_keys_h.begin() + j);
                    break;
            }
            break;
        }
}

void Input::Mouse::clear()
{    
    s_keys_p.clear();
    s_keys_r.clear();
}

void Input::Mouse::printKeyList()
{
    for(int i = 0; i < s_keys_p.size(); i++)
        printf("%d pressed\n", s_keys_p[i]);
    
    for(int i = 0; i < s_keys_r.size(); i++)
        printf("%d released\n", s_keys_r[i]);
    
    for(int i = 0; i < s_keys_h.size(); i++)
        printf("%d held\n", s_keys_h[i]);
}

bool Input::Mouse::is(MouseButton _mb, MouseAction _ma)
{
    switch(_ma)
    {
        case MouseAction::PRESSED :
            for(int i = 0; i < s_keys_p.size(); i++)
                if(s_keys_p[i] == _mb)
                    return true;
            break;
            
        case MouseAction::RELEASED :
            for(int i = 0; i < s_keys_r.size(); i++)
                if(s_keys_r[i] == _mb)
                    return true;
            break;
            
        case MouseAction::HELD :
            for(int i = 0; i < s_keys_h.size(); i++)
                if(s_keys_h[i] == _mb)
                    return true;
            break;
    }
    return false;
}

void Input::Mouse::setMouseJoystickMode()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // cursor mode for mouse cam controls//GLFW_CURSOR_DISABLED
    joyStickMode = true;
    
}

void Input::Mouse::setMOuseNormalMode()
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // cursor in normal mode
    joyStickMode = false;
}



////////////////////////////////////////////////////////////////
void Input::update()
{
    glfwGetCursorPos(window, &Mouse::x, &Mouse::y);
    if(Mouse::x > Config::s_windowWidth)
        Mouse::x = Config::s_windowWidth;
    if(Mouse::x < 0)
        Mouse::x = 0;
    if(Mouse::y > Config::s_windowHeight)
        Mouse::y = Config::s_windowHeight;
    if(Mouse::y < 0)
        Mouse::y = 0;
    
    
    Mouse::dx = Mouse::x - Mouse::xLastFrame;
    Mouse::dy = Mouse::y - Mouse::yLastFrame;
    
    Mouse::xLastFrame = Mouse::x;
    Mouse::yLastFrame = Mouse::y;
    //printf(" %f %f \n", Mouse::dx, Mouse::dy);
}
void Input::clear()
{
    Keyboard::clear();
    Mouse::clear();
//     if(Mouse::joyStickMode)
//         glfwSetCursorPos(window,Config::s_windowWidth/2, Config::s_windowHeight/2);
}




