//#pragma once
//
//#include <Windows.h>
//
//using MouseButton = unsigned int;
//
//// �}�E�X
//class Mouse
//{
//public:
//	static const MouseButton BTN_LEFT = (1 << 0);
//	static const MouseButton BTN_MIDDLE = (1 << 1);
//	static const MouseButton BTN_RIGHT = (1 << 2);
//
//public:
//	Mouse(HWND hWnd);
//	~Mouse() {}
//
//	// �X�V
//	void Update();
//
//
//	// �{�^�����͏�Ԃ̎擾
//	MouseButton GetButton() const { return buttonState[0]; }
//
//	// �{�^��������Ԃ̎擾
//	MouseButton GetButtonDown() const { return buttonDown; }
//
//	// �{�^�������Ԃ̎擾
//	MouseButton GetButtonUp() const { return buttonUp; }
//
//	// �z�C�[���l�̐ݒ�
//	void SetWheel(int wheel) { this->wheel[0] += wheel; }
//
//	// �z�C�[���l�̎擾
//	int GetWheel() const { return wheel[1]; }
//
//	// �}�E�X�J�[�\��X���W�擾
//	int GetPositionX() const { return positionX[0]; }
//
//	// �}�E�X�J�[�\��Y���W�擾
//	int GetPositionY() const { return positionY[0]; }
//
//	// �O��̃}�E�X�J�[�\��X���W�擾
//	int GetOldPositionX() const { return positionX[1]; }
//
//	// �O��̃}�E�X�J�[�\��Y���W�擾
//	int GetOldPositionY() const { return positionY[1]; }
//
//	// �X�N���[�����ݒ�
//	void SetScreenWidth(int width) { screenWidth = width; }
//
//	// �X�N���[�������ݒ�
//	void SetScreenHeight(int height) { screenHeight = height; }
//
//	// �X�N���[�����擾
//	int GetScreenWidth() const { return screenWidth; }
//
//	// �X�N���[�������擾
//	int GetScreenHeight() const { return screenHeight; }
//
//private:
//	MouseButton		buttonState[2] = { 0 };
//	MouseButton		buttonDown = 0;
//	MouseButton		buttonUp = 0;
//	int				positionX[2];
//	int				positionY[2];
//	int				wheel[2];
//	int				screenWidth = 0;
//	int				screenHeight = 0;
//	HWND			hWnd = nullptr;
//};

#pragma once

#include <Windows.h>

using MouseButton = unsigned int;

// �}�E�X
class Mouse
{
public:
    static const MouseButton BTN_LEFT = (1 << 0);
    static const MouseButton BTN_MIDDLE = (1 << 1);
    static const MouseButton BTN_RIGHT = (1 << 2);

public:
    Mouse(HWND hWnd);
    ~Mouse() {}

    // �X�V
    void Update();

    void init() { isInit[0] = isInit[1] = true; }

    void setCenter() { SetCursorPos(_viewportW * 0.5f, _viewportH * 0.5f); }

    //�Q�[�����̍X�V
    void updataNormal(bool isPause);

    //2D�����蔻��
    bool mouseVsRect(const float posX, const float posY, const float width, const float height);

    // �{�^�����͏�Ԃ̎擾
    MouseButton GetButton() const { return _buttonState[0]; }

    // �{�^��������Ԃ̎擾
    MouseButton GetButtonDown() const { return _buttonDown; }

    // �{�^�������Ԃ̎擾
    MouseButton GetButtonUp() const { return _buttonUp; }

    // �z�C�[���l�̐ݒ�
    void SetWheel(int setWheel) { this->_wheel[0] += setWheel; }

    // �z�C�[���l�̎擾
    int GetWheel() const { return _wheel[1]; }

    // �}�E�X�J�[�\��X���W�擾
    int GetPositionX() const { return _positionX[0]; }

    // �}�E�X�J�[�\��Y���W�擾
    int GetPositionY() const { return _positionY[0]; }

    // �O��̃}�E�X�J�[�\��X���W�擾
    int GetOldPositionX() const { return _positionX[1]; }

    // �O��̃}�E�X�J�[�\��Y���W�擾
    int GetOldPositionY() const { return _positionY[1]; }

    int GetDeltaX() const { return _deltaX; }
    int GetDeltaY() const { return _deltaY; }
private:
    bool isInit[2] = { false, false };
    MouseButton		_buttonState[2] = { 0 };
    MouseButton		_buttonDown = 0;
    MouseButton		_buttonUp = 0;
    int				_positionX[2];
    int				_positionY[2];
    int             _deltaX = 0;
    int             _deltaY = 0;
    int				_wheel[2];
    int				_viewportW = 0;
    int				_viewportH = 0;
    HWND			_hWnd = nullptr;
};
