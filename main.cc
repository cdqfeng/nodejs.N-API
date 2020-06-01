#include <node_api.h>
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

// C++插件(N-API实现)
namespace myDemo {

    napi_value SetParent(napi_env env, napi_callback_info info) {

        // 获取参数
        // 参数只有一个，值为win32窗口句柄（16进制字符串，如"0x005207D2"）
        size_t argc = 1;
        napi_value argv[1];
        napi_get_cb_info(env, info, &argc, argv, 0, 0);

        // 类型转换 napi_value to string
        char buf[1024];
        size_t str_len;
        napi_get_value_string_utf8(env, argv[0], (char*)&buf, 1024, &str_len);

        // 16进制字符串转整型 获取窗口句柄
        long i = stoi(buf, NULL, 16);
        HWND hWnd = reinterpret_cast<HWND>(i);

        
        // 设置父窗口
        napi_value temp;
        napi_status status;

        // 也可通过FindWindow函数直接创建窗口句柄
        //HWND hWnd = FindWindow(NULL, "计算器");

        if (hWnd == 0)
        {
            status = napi_create_string_utf8(env, "child window is not found", NAPI_AUTO_LENGTH, &temp);
            if (status != napi_ok) return nullptr;
            return temp;
        }

        // 寻找桌面背景窗口句柄（注意不是桌面窗口,可使用visual studio提供的spy++工具查看窗口层级）
        HWND p = NULL;
        HWND t = NULL;
        do
        {
            p = FindWindowExA(NULL, p, "WorkerW", NULL);
            if (p != 0)
            {
                HWND c = FindWindowExA(p, NULL, "SHELLDLL_DefView", NULL);
                if (c != 0)
                {
                    t = c;
                }
            }
        } while (p != 0);

        if (t == 0)
        {
            status = napi_create_string_utf8(env, "parent window is not found", NAPI_AUTO_LENGTH, &temp);
            if (status != napi_ok) return nullptr;
            return temp;
        }

        HWND res = SetParent(hWnd, t);

        if (res == 0)
        {
            status = napi_create_string_utf8(env, "set parent window failed", NAPI_AUTO_LENGTH, &temp);
            if (status != napi_ok) return nullptr;
            return temp;
        }

        status = napi_create_string_utf8(env, "set parent window success", NAPI_AUTO_LENGTH, &temp);
        if (status != napi_ok) return nullptr;
        return temp;
    }


    napi_value init(napi_env env, napi_value exports) {
        napi_status status;
        napi_value fn;

        status = napi_create_function(env, nullptr, 0, SetParent, nullptr, &fn);
        if (status != napi_ok) return nullptr;

        status = napi_set_named_property(env, exports, "setParent", fn);
        if (status != napi_ok) return nullptr;

        return exports;
    }

    NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
}