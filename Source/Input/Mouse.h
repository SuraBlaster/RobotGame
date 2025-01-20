//#pragma once
//
//#include <Windows.h>
//
//using MouseButton = unsigned int;
//
//// マウス
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
//	// 更新
//	void Update();
//
//
//	// ボタン入力状態の取得
//	MouseButton GetButton() const { return buttonState[0]; }
//
//	// ボタン押下状態の取得
//	MouseButton GetButtonDown() const { return buttonDown; }
//
//	// ボタン押上状態の取得
//	MouseButton GetButtonUp() const { return buttonUp; }
//
//	// ホイール値の設定
//	void SetWheel(int wheel) { this->wheel[0] += wheel; }
//
//	// ホイール値の取得
//	int GetWheel() const { return wheel[1]; }
//
//	// マウスカーソルX座標取得
//	int GetPositionX() const { return positionX[0]; }
//
//	// マウスカーソルY座標取得
//	int GetPositionY() const { return positionY[0]; }
//
//	// 前回のマウスカーソルX座標取得
//	int GetOldPositionX() const { return positionX[1]; }
//
//	// 前回のマウスカーソルY座標取得
//	int GetOldPositionY() const { return positionY[1]; }
//
//	// スクリーン幅設定
//	void SetScreenWidth(int width) { screenWidth = width; }
//
//	// スクリーン高さ設定
//	void SetScreenHeight(int height) { screenHeight = height; }
//
//	// スクリーン幅取得
//	int GetScreenWidth() const { return screenWidth; }
//
//	// スクリーン高さ取得
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

// マウス
class Mouse
{
public:
    static const MouseButton BTN_LEFT = (1 << 0);
    static const MouseButton BTN_MIDDLE = (1 << 1);
    static const MouseButton BTN_RIGHT = (1 << 2);

public:
    Mouse(HWND hWnd);
    ~Mouse() {}

    // 更新
    void Update();

    void init() { isInit[0] = isInit[1] = true; }

    void setCenter() { SetCursorPos(_viewportW * 0.5f, _viewportH * 0.5f); }

    //ゲーム中の更新
    void updataNormal(bool isPause);

    //2D当たり判定
    bool mouseVsRect(const float posX, const float posY, const float width, const float height);

    // ボタン入力状態の取得
    MouseButton GetButton() const { return _buttonState[0]; }

    // ボタン押下状態の取得
    MouseButton GetButtonDown() const { return _buttonDown; }

    // ボタン押上状態の取得
    MouseButton GetButtonUp() const { return _buttonUp; }

    // ホイール値の設定
    void SetWheel(int setWheel) { this->_wheel[0] += setWheel; }

    // ホイール値の取得
    int GetWheel() const { return _wheel[1]; }

    // マウスカーソルX座標取得
    int GetPositionX() const { return _positionX[0]; }

    // マウスカーソルY座標取得
    int GetPositionY() const { return _positionY[0]; }

    // 前回のマウスカーソルX座標取得
    int GetOldPositionX() const { return _positionX[1]; }

    // 前回のマウスカーソルY座標取得
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
