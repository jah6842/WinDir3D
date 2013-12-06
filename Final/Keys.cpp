#include "Keys.h"

bool Keys::A = false;
bool Keys::B = false;
bool Keys::C = false;
bool Keys::D = false;
bool Keys::E = false;
bool Keys::F = false;
bool Keys::G = false;
bool Keys::H = false;
bool Keys::I = false;
bool Keys::J = false;
bool Keys::K = false;
bool Keys::L = false;
bool Keys::M = false;
bool Keys::N = false;
bool Keys::O = false;
bool Keys::P = false;
bool Keys::Q = false;
bool Keys::R = false;
bool Keys::S = false;
bool Keys::T = false;
bool Keys::U = false;
bool Keys::V = false;
bool Keys::W = false;
bool Keys::X = false;
bool Keys::Y = false;
bool Keys::Z = false;
bool Keys::UP = false;
bool Keys::DOWN = false;
bool Keys::LEFT = false;
bool Keys::RIGHT = false;
bool Keys::LSHIFT = false;
bool Keys::RSHIFT = false;
bool Keys::SHIFT = false;
bool Keys::SPACE = false;

void Keys::Input(int key, int scancode, int action, int mods){
	switch(key){
	case GLFW_KEY_A: A = static_cast<bool>(action); break;
	case GLFW_KEY_B: B = static_cast<bool>(action); break;
	case GLFW_KEY_C: C = static_cast<bool>(action); break;
	case GLFW_KEY_D: D = static_cast<bool>(action); break;
	case GLFW_KEY_E: E = static_cast<bool>(action); break;
	case GLFW_KEY_F: F = static_cast<bool>(action); break;
	case GLFW_KEY_G: G = static_cast<bool>(action); break;
	case GLFW_KEY_H: H = static_cast<bool>(action); break;
	case GLFW_KEY_I: I = static_cast<bool>(action); break;
	case GLFW_KEY_J: J = static_cast<bool>(action); break;
	case GLFW_KEY_K: K = static_cast<bool>(action); break;
	case GLFW_KEY_L: L = static_cast<bool>(action); break;
	case GLFW_KEY_M: M = static_cast<bool>(action); break;
	case GLFW_KEY_N: N = static_cast<bool>(action); break;
	case GLFW_KEY_O: O = static_cast<bool>(action); break;
	case GLFW_KEY_P: P = static_cast<bool>(action); break;
	case GLFW_KEY_Q: Q = static_cast<bool>(action); break;
	case GLFW_KEY_R: R = static_cast<bool>(action); break;
	case GLFW_KEY_S: S = static_cast<bool>(action); break;
	case GLFW_KEY_T: T = static_cast<bool>(action); break;
	case GLFW_KEY_U: U = static_cast<bool>(action); break;
	case GLFW_KEY_V: V = static_cast<bool>(action); break;
	case GLFW_KEY_W: W = static_cast<bool>(action); break;
	case GLFW_KEY_X: X = static_cast<bool>(action); break;
	case GLFW_KEY_Y: Y = static_cast<bool>(action); break;
	case GLFW_KEY_Z: Z = static_cast<bool>(action); break;
	case GLFW_KEY_LEFT_SHIFT: LSHIFT = static_cast<bool>(action); break;
	case GLFW_KEY_RIGHT_SHIFT: RSHIFT = static_cast<bool>(action); break;
	case GLFW_KEY_UP: UP = static_cast<bool>(action); break;
	case GLFW_KEY_DOWN: DOWN = static_cast<bool>(action); break;
	case GLFW_KEY_LEFT: LEFT = static_cast<bool>(action); break;
	case GLFW_KEY_RIGHT: RIGHT = static_cast<bool>(action); break;
	case GLFW_KEY_SPACE: SPACE = static_cast<bool>(action); break;
	default: std::cout << "Unknown Key Pressed: " << key << std::endl; break;
	}

	if(LSHIFT || RSHIFT)
		SHIFT = true;
	else
		SHIFT = false;
}