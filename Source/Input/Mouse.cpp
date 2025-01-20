//#include "Input/Mouse.h"
//
//static const int KeyMap[] =
//{
//	VK_LBUTTON,		// ���{�^��
//	VK_MBUTTON,		// ���{�^��
//	VK_RBUTTON,		// �E�{�^��
//};
//
//// �R���X�g���N�^
//Mouse::Mouse(HWND hWnd)
//	: hWnd(hWnd)
//{
//	RECT rc;
//	GetClientRect(hWnd, &rc);
//	screenWidth = rc.right - rc.left;
//	screenHeight = rc.bottom - rc.top;
//}
//
//// �X�V
//void Mouse::Update()
//{
//	// �X�C�b�`���
//	MouseButton newButtonState = 0;
//
//	for (int i = 0; i < ARRAYSIZE(KeyMap); ++i)
//	{
//		if (::GetAsyncKeyState(KeyMap[i]) & 0x8000)
//		{
//			newButtonState |= (1 << i);
//		}
//	}
//
//	// �z�C�[��
//	wheel[1] = wheel[0];
//	wheel[0] = 0;
//
//	// �{�^�����X�V
//	buttonState[1] = buttonState[0];	// �X�C�b�`����
//	buttonState[0] = newButtonState;
//
//	buttonDown = ~buttonState[1] & newButtonState;	// �������u��
//	buttonUp = ~newButtonState & buttonState[1];	// �������u��
//
//	// �J�[�\���ʒu�̎擾
//	POINT cursor;
//	::GetCursorPos(&cursor);
//	::ScreenToClient(hWnd, &cursor);
//
//	// ��ʂ̃T�C�Y���擾����B
//	RECT rc;
//	GetClientRect(hWnd, &rc);
//	UINT screenW = rc.right - rc.left;
//	UINT screenH = rc.bottom - rc.top;
//	UINT viewportW = screenWidth;
//	UINT viewportH = screenHeight;
//
//	// ��ʕ␳
//	positionX[1] = positionX[0];
//	positionY[1] = positionY[0];
//	positionX[0] = (LONG)(cursor.x / static_cast<float>(viewportW) * static_cast<float>(screenW));
//	positionY[0] = (LONG)(cursor.y / static_cast<float>(viewportH) * static_cast<float>(screenH));
//}

#include "Input/Mouse.h"
#include "Input/Input.h"

static const int KeyMap[] =
{
    VK_LBUTTON,		// ���{�^��
    VK_MBUTTON,		// ���{�^��
    VK_RBUTTON,		// �E�{�^��
};

// �R���X�g���N�^
Mouse::Mouse(HWND hWnd)
    : _hWnd(hWnd)
{
    RECT rc;
    GetClientRect(hWnd, &rc);
    _viewportW = rc.right - rc.left;
    _viewportH = rc.bottom - rc.top;
}

// �X�V
void Mouse::Update()
{
    // �X�C�b�`���
    MouseButton newButtonState = 0;

    for (int i = 0; i < ARRAYSIZE(KeyMap); ++i)
    {
        if (::GetAsyncKeyState(KeyMap[i]) & 0x8000)
        {
            newButtonState |= (1 << i);
        }
    }

    // �z�C�[��
    _wheel[1] = _wheel[0];
    _wheel[0] = 0;

    // �{�^�����X�V
    _buttonState[1] = _buttonState[0];	// �X�C�b�`����
    _buttonState[0] = newButtonState;

    _buttonDown = ~_buttonState[1] & newButtonState;	// �������u��
    _buttonUp = ~newButtonState & _buttonState[1];	// �������u��

    if (isInit[0])
    {
        SetCursorPos(_viewportW / 2, _viewportH / 2);
    }

    // �J�[�\���ʒu�̎擾
    POINT cursor;
    ::GetCursorPos(&cursor);
    ::ScreenToClient(_hWnd, &cursor);

    // ��ʂ̃T�C�Y���擾����B
    RECT rc;
    GetClientRect(_hWnd, &rc);
    UINT screenW = rc.right - rc.left;
    UINT screenH = rc.bottom - rc.top;
    UINT viewportW = _viewportW;
    UINT viewportH = _viewportH;

    if (isInit[0])
    {
        isInit[0] = false;
        _positionX[1] = _positionX[0] = (LONG)(cursor.x / static_cast<float>(viewportW) * static_cast<float>(screenW));
        _positionY[1] = _positionY[0] = (LONG)(cursor.y / static_cast<float>(viewportH) * static_cast<float>(screenH));
        //positionX[1] = positionX[0] = 952;
        //positionY[1] = positionY[0] = 500;

    }
    else
    {
        // ��ʕ␳
        _positionX[1] = _positionX[0];
        _positionY[1] = _positionY[0];
        _positionX[0] = (LONG)(cursor.x / static_cast<float>(viewportW) * static_cast<float>(screenW));
        _positionY[0] = (LONG)(cursor.y / static_cast<float>(viewportH) * static_cast<float>(screenH));
        //positionX[0] = cursor.x;
        //positionY[0] = cursor.y;
        _deltaX = _positionX[0] - _positionX[1];
        _deltaY = _positionY[0] - _positionY[1];

        //        float centerX = static_cast<float>((_viewportW * 0.5f) / static_cast<float>(viewportW) * static_cast<float>(screenW));
        //        float centerY = static_cast<float>((_viewportH * 0.5f) / static_cast<float>(viewportH) * static_cast<float>(screenH));
        //        _deltaX = _positionX[0] - static_cast<int>(centerX);
        //        _deltaY = _positionY[0] - static_cast<int>(centerY);
    }
}

void Mouse::updataNormal(bool isCameraControll)
{
    if (isCameraControll)
    {
        RECT rc;
        GetClientRect(_hWnd, &rc);
        UINT screenW = rc.right - rc.left;
        UINT screenH = rc.bottom - rc.top;
        float centerX = static_cast<float>((_viewportW * 0.5f) / static_cast<float>(_viewportW) * static_cast<float>(screenW));
        float centerY = static_cast<float>((_viewportH * 0.5f) / static_cast<float>(_viewportH) * static_cast<float>(screenH));
        _positionX[1] = static_cast<int>(centerX);
        _positionY[1] = static_cast<int>(centerY);
        POINT cursor;
        cursor.x = static_cast<int>(centerX);
        cursor.y = static_cast<int>(centerY);
        ClientToScreen(_hWnd, &cursor);
        SetCursorPos(cursor.x, cursor.y);
    }
}

bool Mouse::mouseVsRect(const float posX, const float posY, const float width, const float height)
{
    return (
        _positionX[0] > posX &&
        _positionX[0] < posX + width &&
        _positionY[0] > posY &&
        _positionY[0] < posY + height);
}