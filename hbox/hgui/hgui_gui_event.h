/***************************************************************
 * Name:      hgui_gui_event.h
 * Purpose:   声明event接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2024-12-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#ifndef __HGUI_GUI_EVENT_H__
#define __HGUI_GUI_EVENT_H__
#include "hgui_gui_common.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/** \brief 事件处理回调
 *
 * \param type uint8_t 事件类型，见hgui_gui_event_type_t。
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return bool 事件是否处理成功，一般用于用户事件
 *
 */
typedef bool (*hgui_gui_event_callback_t)(uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

typedef enum
{
    HGUI_GUI_EVENT_TYPE_KEY=0,              //按键事件
    HGUI_GUI_EVENT_TYPE_MOUSE_MOTION=1,     //鼠标移动事件
    HGUI_GUI_EVENT_TYPE_MOUSE_BUTTON=2,     //鼠标按键事件
    HGUI_GUI_EVENT_TYPE_TOUCH=3,            //触摸事件
    HGUI_GUI_EVENT_TYPE_JOY_AXIS=4,         //游戏杆(joystick)轴事件
    HGUI_GUI_EVENT_TYPE_JOY_BALL=5,         //游戏杆(joystick)轨迹球事件
    HGUI_GUI_EVENT_TYPE_JOY_HAT=6,          //游戏杆(joystick)帽开关事件
    HGUI_GUI_EVENT_TYPE_JOY_BUTTON=7,       //游戏杆(joystick)按键事件
    HGUI_GUI_EVENT_TYPE_USR_START=100,      //用户事件起始，大于此值的事件类型用户可自行使用。
} hgui_gui_event_type_t;


typedef enum
{
    HGUI_GUI_EVENT_KEY_VALUE_UNKNOWN = 0,
    HGUI_GUI_EVENT_KEY_VALUE_FIRST = 0,
    HGUI_GUI_EVENT_KEY_VALUE_BACKSPACE = 8,
    HGUI_GUI_EVENT_KEY_VALUE_TAB = 9,
    HGUI_GUI_EVENT_KEY_VALUE_CLEAR = 12,
    HGUI_GUI_EVENT_KEY_VALUE_RETURN = 13,
    HGUI_GUI_EVENT_KEY_VALUE_PAUSE = 19,
    HGUI_GUI_EVENT_KEY_VALUE_ESCAPE = 27,
    HGUI_GUI_EVENT_KEY_VALUE_SPACE = 32,
    HGUI_GUI_EVENT_KEY_VALUE_EXCLAIM = 33,
    HGUI_GUI_EVENT_KEY_VALUE_QUOTEDBL = 34,
    HGUI_GUI_EVENT_KEY_VALUE_HASH = 35,
    HGUI_GUI_EVENT_KEY_VALUE_DOLLAR = 36,
    HGUI_GUI_EVENT_KEY_VALUE_AMPERSAND = 38,
    HGUI_GUI_EVENT_KEY_VALUE_QUOTE = 39,
    HGUI_GUI_EVENT_KEY_VALUE_LEFTPAREN = 40,
    HGUI_GUI_EVENT_KEY_VALUE_RIGHTPAREN = 41,
    HGUI_GUI_EVENT_KEY_VALUE_ASTERISK = 42,
    HGUI_GUI_EVENT_KEY_VALUE_PLUS = 43,
    HGUI_GUI_EVENT_KEY_VALUE_COMMA = 44,
    HGUI_GUI_EVENT_KEY_VALUE_MINUS = 45,
    HGUI_GUI_EVENT_KEY_VALUE_PERIOD = 46,
    HGUI_GUI_EVENT_KEY_VALUE_SLASH = 47,
    HGUI_GUI_EVENT_KEY_VALUE_0 = 48,
    HGUI_GUI_EVENT_KEY_VALUE_1 = 49,
    HGUI_GUI_EVENT_KEY_VALUE_2 = 50,
    HGUI_GUI_EVENT_KEY_VALUE_3 = 51,
    HGUI_GUI_EVENT_KEY_VALUE_4 = 52,
    HGUI_GUI_EVENT_KEY_VALUE_5 = 53,
    HGUI_GUI_EVENT_KEY_VALUE_6 = 54,
    HGUI_GUI_EVENT_KEY_VALUE_7 = 55,
    HGUI_GUI_EVENT_KEY_VALUE_8 = 56,
    HGUI_GUI_EVENT_KEY_VALUE_9 = 57,
    HGUI_GUI_EVENT_KEY_VALUE_COLON = 58,
    HGUI_GUI_EVENT_KEY_VALUE_SEMICOLON = 59,
    HGUI_GUI_EVENT_KEY_VALUE_LESS = 60,
    HGUI_GUI_EVENT_KEY_VALUE_EQUALS = 61,
    HGUI_GUI_EVENT_KEY_VALUE_GREATER = 62,
    HGUI_GUI_EVENT_KEY_VALUE_QUESTION = 63,
    HGUI_GUI_EVENT_KEY_VALUE_AT = 64,
    HGUI_GUI_EVENT_KEY_VALUE_LEFTBRACKET = 91,
    HGUI_GUI_EVENT_KEY_VALUE_BACKSLASH = 92,
    HGUI_GUI_EVENT_KEY_VALUE_RIGHTBRACKET = 93,
    HGUI_GUI_EVENT_KEY_VALUE_CARET = 94,
    HGUI_GUI_EVENT_KEY_VALUE_UNDERSCORE = 95,
    HGUI_GUI_EVENT_KEY_VALUE_BACKQUOTE = 96,
    HGUI_GUI_EVENT_KEY_VALUE_a = 97,
    HGUI_GUI_EVENT_KEY_VALUE_b = 98,
    HGUI_GUI_EVENT_KEY_VALUE_c = 99,
    HGUI_GUI_EVENT_KEY_VALUE_d = 100,
    HGUI_GUI_EVENT_KEY_VALUE_e = 101,
    HGUI_GUI_EVENT_KEY_VALUE_f = 102,
    HGUI_GUI_EVENT_KEY_VALUE_g = 103,
    HGUI_GUI_EVENT_KEY_VALUE_h = 104,
    HGUI_GUI_EVENT_KEY_VALUE_i = 105,
    HGUI_GUI_EVENT_KEY_VALUE_j = 106,
    HGUI_GUI_EVENT_KEY_VALUE_k = 107,
    HGUI_GUI_EVENT_KEY_VALUE_l = 108,
    HGUI_GUI_EVENT_KEY_VALUE_m = 109,
    HGUI_GUI_EVENT_KEY_VALUE_n = 110,
    HGUI_GUI_EVENT_KEY_VALUE_o = 111,
    HGUI_GUI_EVENT_KEY_VALUE_p = 112,
    HGUI_GUI_EVENT_KEY_VALUE_q = 113,
    HGUI_GUI_EVENT_KEY_VALUE_r = 114,
    HGUI_GUI_EVENT_KEY_VALUE_s = 115,
    HGUI_GUI_EVENT_KEY_VALUE_t = 116,
    HGUI_GUI_EVENT_KEY_VALUE_u = 117,
    HGUI_GUI_EVENT_KEY_VALUE_v = 118,
    HGUI_GUI_EVENT_KEY_VALUE_w = 119,
    HGUI_GUI_EVENT_KEY_VALUE_x = 120,
    HGUI_GUI_EVENT_KEY_VALUE_y = 121,
    HGUI_GUI_EVENT_KEY_VALUE_z = 122,
    HGUI_GUI_EVENT_KEY_VALUE_DELETE = 127,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_0 = 160,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_1 = 161,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_2 = 162,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_3 = 163,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_4 = 164,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_5 = 165,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_6 = 166,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_7 = 167,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_8 = 168,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_9 = 169,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_10 = 170,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_11 = 171,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_12 = 172,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_13 = 173,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_14 = 174,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_15 = 175,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_16 = 176,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_17 = 177,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_18 = 178,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_19 = 179,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_20 = 180,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_21 = 181,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_22 = 182,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_23 = 183,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_24 = 184,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_25 = 185,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_26 = 186,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_27 = 187,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_28 = 188,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_29 = 189,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_30 = 190,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_31 = 191,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_32 = 192,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_33 = 193,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_34 = 194,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_35 = 195,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_36 = 196,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_37 = 197,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_38 = 198,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_39 = 199,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_40 = 200,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_41 = 201,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_42 = 202,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_43 = 203,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_44 = 204,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_45 = 205,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_46 = 206,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_47 = 207,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_48 = 208,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_49 = 209,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_50 = 210,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_51 = 211,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_52 = 212,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_53 = 213,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_54 = 214,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_55 = 215,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_56 = 216,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_57 = 217,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_58 = 218,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_59 = 219,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_60 = 220,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_61 = 221,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_62 = 222,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_63 = 223,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_64 = 224,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_65 = 225,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_66 = 226,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_67 = 227,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_68 = 228,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_69 = 229,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_70 = 230,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_71 = 231,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_72 = 232,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_73 = 233,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_74 = 234,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_75 = 235,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_76 = 236,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_77 = 237,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_78 = 238,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_79 = 239,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_80 = 240,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_81 = 241,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_82 = 242,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_83 = 243,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_84 = 244,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_85 = 245,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_86 = 246,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_87 = 247,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_88 = 248,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_89 = 249,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_90 = 250,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_91 = 251,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_92 = 252,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_93 = 253,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_94 = 254,
    HGUI_GUI_EVENT_KEY_VALUE_WORLD_95 = 255,
    HGUI_GUI_EVENT_KEY_VALUE_KP0 = 256,
    HGUI_GUI_EVENT_KEY_VALUE_KP1 = 257,
    HGUI_GUI_EVENT_KEY_VALUE_KP2 = 258,
    HGUI_GUI_EVENT_KEY_VALUE_KP3 = 259,
    HGUI_GUI_EVENT_KEY_VALUE_KP4 = 260,
    HGUI_GUI_EVENT_KEY_VALUE_KP5 = 261,
    HGUI_GUI_EVENT_KEY_VALUE_KP6 = 262,
    HGUI_GUI_EVENT_KEY_VALUE_KP7 = 263,
    HGUI_GUI_EVENT_KEY_VALUE_KP8 = 264,
    HGUI_GUI_EVENT_KEY_VALUE_KP9 = 265,
    HGUI_GUI_EVENT_KEY_VALUE_KP_PERIOD = 266,
    HGUI_GUI_EVENT_KEY_VALUE_KP_DIVIDE = 267,
    HGUI_GUI_EVENT_KEY_VALUE_KP_MULTIPLY = 268,
    HGUI_GUI_EVENT_KEY_VALUE_KP_MINUS = 269,
    HGUI_GUI_EVENT_KEY_VALUE_KP_PLUS = 270,
    HGUI_GUI_EVENT_KEY_VALUE_KP_ENTER = 271,
    HGUI_GUI_EVENT_KEY_VALUE_KP_EQUALS = 272,
    HGUI_GUI_EVENT_KEY_VALUE_UP = 273,
    HGUI_GUI_EVENT_KEY_VALUE_DOWN = 274,
    HGUI_GUI_EVENT_KEY_VALUE_RIGHT = 275,
    HGUI_GUI_EVENT_KEY_VALUE_LEFT = 276,
    HGUI_GUI_EVENT_KEY_VALUE_INSERT = 277,
    HGUI_GUI_EVENT_KEY_VALUE_HOME = 278,
    HGUI_GUI_EVENT_KEY_VALUE_END = 279,
    HGUI_GUI_EVENT_KEY_VALUE_PAGEUP = 280,
    HGUI_GUI_EVENT_KEY_VALUE_PAGEDOWN = 281,
    HGUI_GUI_EVENT_KEY_VALUE_F1 = 282,
    HGUI_GUI_EVENT_KEY_VALUE_F2 = 283,
    HGUI_GUI_EVENT_KEY_VALUE_F3 = 284,
    HGUI_GUI_EVENT_KEY_VALUE_F4 = 285,
    HGUI_GUI_EVENT_KEY_VALUE_F5 = 286,
    HGUI_GUI_EVENT_KEY_VALUE_F6 = 287,
    HGUI_GUI_EVENT_KEY_VALUE_F7 = 288,
    HGUI_GUI_EVENT_KEY_VALUE_F8 = 289,
    HGUI_GUI_EVENT_KEY_VALUE_F9 = 290,
    HGUI_GUI_EVENT_KEY_VALUE_F10 = 291,
    HGUI_GUI_EVENT_KEY_VALUE_F11 = 292,
    HGUI_GUI_EVENT_KEY_VALUE_F12 = 293,
    HGUI_GUI_EVENT_KEY_VALUE_F13 = 294,
    HGUI_GUI_EVENT_KEY_VALUE_F14 = 295,
    HGUI_GUI_EVENT_KEY_VALUE_F15 = 296,
    HGUI_GUI_EVENT_KEY_VALUE_NUMLOCK = 300,
    HGUI_GUI_EVENT_KEY_VALUE_CAPSLOCK = 301,
    HGUI_GUI_EVENT_KEY_VALUE_SCROLLOCK = 302,
    HGUI_GUI_EVENT_KEY_VALUE_RSHIFT = 303,
    HGUI_GUI_EVENT_KEY_VALUE_LSHIFT = 304,
    HGUI_GUI_EVENT_KEY_VALUE_RCTRL = 305,
    HGUI_GUI_EVENT_KEY_VALUE_LCTRL = 306,
    HGUI_GUI_EVENT_KEY_VALUE_RALT = 307,
    HGUI_GUI_EVENT_KEY_VALUE_LALT = 308,
    HGUI_GUI_EVENT_KEY_VALUE_RMETA = 309,
    HGUI_GUI_EVENT_KEY_VALUE_LMETA = 310,
    HGUI_GUI_EVENT_KEY_VALUE_LSUPER = 311,
    HGUI_GUI_EVENT_KEY_VALUE_RSUPER = 312,
    HGUI_GUI_EVENT_KEY_VALUE_MODE = 313,
    HGUI_GUI_EVENT_KEY_VALUE_COMPOSE = 314,
    HGUI_GUI_EVENT_KEY_VALUE_HELP = 315,
    HGUI_GUI_EVENT_KEY_VALUE_PRINT = 316,
    HGUI_GUI_EVENT_KEY_VALUE_SYSREQ = 317,
    HGUI_GUI_EVENT_KEY_VALUE_BREAK = 318,
    HGUI_GUI_EVENT_KEY_VALUE_MENU = 319,
    HGUI_GUI_EVENT_KEY_VALUE_POWER = 320,
    HGUI_GUI_EVENT_KEY_VALUE_EURO = 321,
    HGUI_GUI_EVENT_KEY_VALUE_UNDO = 322,
    HGUI_GUI_EVENT_KEY_VALUE_LAST
} hgui_gui_event_key_value_t;

typedef enum
{
    HGUI_GUI_EVENT_KEY_MODE_NONE = 0x0000,
    HGUI_GUI_EVENT_KEY_MODE_LSHIFT= 0x0001,
    HGUI_GUI_EVENT_KEY_MODE_RSHIFT= 0x0002,
    HGUI_GUI_EVENT_KEY_MODE_LCTRL = 0x0040,
    HGUI_GUI_EVENT_KEY_MODE_RCTRL = 0x0080,
    HGUI_GUI_EVENT_KEY_MODE_LALT = 0x0100,
    HGUI_GUI_EVENT_KEY_MODE_RALT = 0x0200,
    HGUI_GUI_EVENT_KEY_MODE_LMETA = 0x0400,
    HGUI_GUI_EVENT_KEY_MODE_RMETA = 0x0800,
    HGUI_GUI_EVENT_KEY_MODE_NUM = 0x1000,
    HGUI_GUI_EVENT_KEY_MODE_CAPS = 0x2000,
    HGUI_GUI_EVENT_KEY_MODE_MODE = 0x4000,
    HGUI_GUI_EVENT_KEY_MODE_RESERVED = 0x8000
} hgui_gui_event_key_mode_t;

#define HGUI_GUI_EVENT_KEY_MODE_CTRL (HGUI_GUI_EVENT_KEY_MODE_LCTRL|HGUI_GUI_EVENT_KEY_MODE_RCTRL)
#define HGUI_GUI_EVENT_KEY_MODE_SHIFT (HGUI_GUI_EVENT_KEY_MODE_LSHIFT|HGUI_GUI_EVENT_KEY_MODE_RSHIFT)
#define HGUI_GUI_EVENT_KEY_MODE_ALT (HGUI_GUI_EVENT_KEY_MODE_LALT|HGUI_GUI_EVENT_KEY_MODE_RALT)
#define HGUI_GUI_EVENT_KEY_MODE_META (HGUI_GUI_EVENT_KEY_MODE_LMETA|HGUI_GUI_EVENT_KEY_MODE_RMETA)


typedef struct
{
    uint32_t key_value:15;//按键值,见hgui_gui_event_key_value_t。
    uint32_t key_press_or_release:1;//1=按键按下,0=按键弹起,注意：按键按下是必须实现的，而按键弹起的实现是可选的。
    uint32_t key_mode:16;//按键模式,见hgui_gui_event_key_mode_t。
} hgui_gui_event_key_t; /**< 事件类型为HGUI_GUI_EVENT_TYPE_KEY时的事件参数*/


/** \brief 发射按键事件
 *
 * \param key const hgui_gui_event_key_t* 按键事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_key_emit(const hgui_gui_event_key_t *key,hgui_gui_event_callback_t event_cb,void *usr);


/** \brief 获取按键事件
 *
 * \param key hgui_gui_event_key_t* 按键，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_key_t* 失败返回NULL
 *
 */
hgui_gui_event_key_t *hgui_gui_event_key_get(hgui_gui_event_key_t *key,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);


typedef enum
{
    HGUI_GUI_EVENT_MOUSE_BUTTON_UNKNOWN = 0,
    HGUI_GUI_EVENT_MOUSE_BUTTON_LEFT = 1,       /**< 左键*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_MIDDLE = 2,     /**< 中键*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_RIGHT = 3,      /**< 右键*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_WHEELUP = 4,    /**< 滚轮向上*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_WHEELDOWN = 5,  /**< 滚轮向下*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_X1 = 6,         /**< 扩展键1*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_X2 = 7          /**< 扩展键2*/
} hgui_gui_event_mouse_button_value_t;

typedef enum
{
    HGUI_GUI_EVENT_MOUSE_BUTTON_STATE_RELEASED = 0, /**< 按键弹起*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_STATE_PRESSED = 1   /**< 按键按下*/
} hgui_gui_event_mouse_button_state_t;

typedef enum
{
    HGUI_GUI_EVENT_MOUSE_BUTTON_MASK_LEFT = 0x01,   /**< 左键按下*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_MASK_MIDDLE = 0x02, /**< 中键按下*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_MASK_RIGHT = 0x04,  /**< 右键按下*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_MASK_X1 = 0x08,     /**< 扩展键1按下*/
    HGUI_GUI_EVENT_MOUSE_BUTTON_MASK_X2 = 0x10      /**< 扩展键2按下*/
} hgui_gui_event_mouse_button_mask_t;

/**< 事件类型为HGUI_GUI_EVENT_TYPE_MOUSE_MOTION时的事件参数*/
typedef struct
{
    uint32_t mouse_which;       /**< 鼠标设备索引*/
    uint16_t mouse_state;       /**< 鼠标按键状态位图,见hgui_gui_event_mouse_button_mask_t*/
    uint16_t reserved;          /**< 保留*/
    int32_t mouse_x;            /**< 鼠标X坐标(像素)*/
    int32_t mouse_y;            /**< 鼠标Y坐标(像素)*/
    int32_t mouse_xrel;         /**< 相对上次的X坐标变化量(像素)*/
    int32_t mouse_yrel;         /**< 相对上次的Y坐标变化量(像素)*/
} hgui_gui_event_mouse_motion_t;

/**< 事件类型为HGUI_GUI_EVENT_TYPE_MOUSE_BUTTON时的事件参数*/
typedef struct
{
    uint32_t mouse_which;               /**< 鼠标设备索引*/
    uint8_t mouse_button;               /**< 按键,见hgui_gui_event_mouse_button_value_t*/
    uint8_t mouse_press_or_release;     /**< 1=按键按下,0=按键弹起,见hgui_gui_event_mouse_button_state_t*/
    uint16_t reserved;                  /**< 保留*/
    int32_t mouse_x;                    /**< 鼠标X坐标(像素)*/
    int32_t mouse_y;                    /**< 鼠标Y坐标(像素)*/
} hgui_gui_event_mouse_button_t;

typedef enum
{
    HGUI_GUI_EVENT_TOUCH_ACTION_UNKNOWN = 0,
    HGUI_GUI_EVENT_TOUCH_ACTION_DOWN = 1,   /**< 触摸点按下*/
    HGUI_GUI_EVENT_TOUCH_ACTION_UP = 2,     /**< 触摸点弹起*/
    HGUI_GUI_EVENT_TOUCH_ACTION_MOTION = 3  /**< 触摸点移动*/
} hgui_gui_event_touch_action_t;

/**< 事件类型为HGUI_GUI_EVENT_TYPE_TOUCH时的事件参数，坐标采用像素(非归一化坐标)*/
typedef struct
{
    uint32_t touch_which;               /**< 触摸设备索引*/
    uint32_t touch_id;                  /**< 触摸点ID(手指ID)*/
    int32_t touch_x;                    /**< 触摸点X坐标(像素)*/
    int32_t touch_y;                    /**< 触摸点Y坐标(像素)*/
    int32_t touch_dx;                   /**< 相对上次的X坐标变化量(像素)*/
    int32_t touch_dy;                   /**< 相对上次的Y坐标变化量(像素)*/
    uint16_t touch_pressure;            /**< 压力,0~65535,0表示不支持*/
    uint8_t touch_action;               /**< 触摸动作,见hgui_gui_event_touch_action_t*/
    uint8_t reserved;                   /**< 保留*/
} hgui_gui_event_touch_t;

typedef enum
{
    HGUI_GUI_EVENT_JOY_HAT_CENTERED = 0x00,      /**< 居中*/
    HGUI_GUI_EVENT_JOY_HAT_UP = 0x01,            /**< 上*/
    HGUI_GUI_EVENT_JOY_HAT_RIGHT = 0x02,         /**< 右*/
    HGUI_GUI_EVENT_JOY_HAT_DOWN = 0x04,          /**< 下*/
    HGUI_GUI_EVENT_JOY_HAT_LEFT = 0x08,          /**< 左*/
    HGUI_GUI_EVENT_JOY_HAT_RIGHTUP = 0x03,       /**< 右上*/
    HGUI_GUI_EVENT_JOY_HAT_RIGHTDOWN = 0x06,     /**< 右下*/
    HGUI_GUI_EVENT_JOY_HAT_LEFTUP = 0x09,        /**< 左上*/
    HGUI_GUI_EVENT_JOY_HAT_LEFTDOWN = 0x0C       /**< 左下*/
} hgui_gui_event_joy_hat_value_t;

/**< 事件类型为HGUI_GUI_EVENT_TYPE_JOY_AXIS时的事件参数*/
typedef struct
{
    uint8_t joy_which;          /**< 游戏杆设备索引*/
    uint8_t joy_axis;           /**< 轴索引*/
    int16_t joy_value;          /**< 轴的值,-32768~32767*/
} hgui_gui_event_joy_axis_t;

/**< 事件类型为HGUI_GUI_EVENT_TYPE_JOY_BALL时的事件参数*/
typedef struct
{
    uint8_t joy_which;          /**< 游戏杆设备索引*/
    uint8_t joy_ball;           /**< 轨迹球索引*/
    int16_t joy_xrel;           /**< 相对X坐标变化量*/
    int16_t joy_yrel;           /**< 相对Y坐标变化量*/
} hgui_gui_event_joy_ball_t;

/**< 事件类型为HGUI_GUI_EVENT_TYPE_JOY_HAT时的事件参数*/
typedef struct
{
    uint8_t joy_which;          /**< 游戏杆设备索引*/
    uint8_t joy_hat;            /**< 帽开关索引*/
    uint8_t joy_value;          /**< 帽开关的值,见hgui_gui_event_joy_hat_value_t*/
    uint8_t reserved;           /**< 保留*/
} hgui_gui_event_joy_hat_t;

/**< 事件类型为HGUI_GUI_EVENT_TYPE_JOY_BUTTON时的事件参数*/
typedef struct
{
    uint8_t joy_which;                  /**< 游戏杆设备索引*/
    uint8_t joy_button;                 /**< 按键索引*/
    uint8_t joy_press_or_release;       /**< 1=按键按下,0=按键弹起*/
    uint8_t reserved;                   /**< 保留*/
} hgui_gui_event_joy_button_t;


/** \brief 发射鼠标移动事件
 *
 * \param motion const hgui_gui_event_mouse_motion_t* 鼠标移动事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_mouse_motion_emit(const hgui_gui_event_mouse_motion_t *motion,hgui_gui_event_callback_t event_cb,void *usr);

/** \brief 获取鼠标移动事件
 *
 * \param motion hgui_gui_event_mouse_motion_t* 鼠标移动事件，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_mouse_motion_t* 失败返回NULL
 *
 */
hgui_gui_event_mouse_motion_t *hgui_gui_event_mouse_motion_get(hgui_gui_event_mouse_motion_t *motion,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

/** \brief 发射鼠标按键事件
 *
 * \param button const hgui_gui_event_mouse_button_t* 鼠标按键事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_mouse_button_emit(const hgui_gui_event_mouse_button_t *button,hgui_gui_event_callback_t event_cb,void *usr);

/** \brief 获取鼠标按键事件
 *
 * \param button hgui_gui_event_mouse_button_t* 鼠标按键事件，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_mouse_button_t* 失败返回NULL
 *
 */
hgui_gui_event_mouse_button_t *hgui_gui_event_mouse_button_get(hgui_gui_event_mouse_button_t *button,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

/** \brief 发射触摸事件
 *
 * \param touch const hgui_gui_event_touch_t* 触摸事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_touch_emit(const hgui_gui_event_touch_t *touch,hgui_gui_event_callback_t event_cb,void *usr);

/** \brief 获取触摸事件
 *
 * \param touch hgui_gui_event_touch_t* 触摸事件，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_touch_t* 失败返回NULL
 *
 */
hgui_gui_event_touch_t *hgui_gui_event_touch_get(hgui_gui_event_touch_t *touch,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

/** \brief 发射游戏杆轴事件
 *
 * \param axis const hgui_gui_event_joy_axis_t* 游戏杆轴事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_joy_axis_emit(const hgui_gui_event_joy_axis_t *axis,hgui_gui_event_callback_t event_cb,void *usr);

/** \brief 获取游戏杆轴事件
 *
 * \param axis hgui_gui_event_joy_axis_t* 游戏杆轴事件，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_joy_axis_t* 失败返回NULL
 *
 */
hgui_gui_event_joy_axis_t *hgui_gui_event_joy_axis_get(hgui_gui_event_joy_axis_t *axis,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

/** \brief 发射游戏杆轨迹球事件
 *
 * \param ball const hgui_gui_event_joy_ball_t* 游戏杆轨迹球事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_joy_ball_emit(const hgui_gui_event_joy_ball_t *ball,hgui_gui_event_callback_t event_cb,void *usr);

/** \brief 获取游戏杆轨迹球事件
 *
 * \param ball hgui_gui_event_joy_ball_t* 游戏杆轨迹球事件，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_joy_ball_t* 失败返回NULL
 *
 */
hgui_gui_event_joy_ball_t *hgui_gui_event_joy_ball_get(hgui_gui_event_joy_ball_t *ball,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

/** \brief 发射游戏杆帽开关事件
 *
 * \param hat const hgui_gui_event_joy_hat_t* 游戏杆帽开关事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_joy_hat_emit(const hgui_gui_event_joy_hat_t *hat,hgui_gui_event_callback_t event_cb,void *usr);

/** \brief 获取游戏杆帽开关事件
 *
 * \param hat hgui_gui_event_joy_hat_t* 游戏杆帽开关事件，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_joy_hat_t* 失败返回NULL
 *
 */
hgui_gui_event_joy_hat_t *hgui_gui_event_joy_hat_get(hgui_gui_event_joy_hat_t *hat,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

/** \brief 发射游戏杆按键事件
 *
 * \param button const hgui_gui_event_joy_button_t* 游戏杆按键事件
 * \param event_cb hgui_gui_event_callback_t 事件回调
 * \param usr void* 用户参数
 * \return bool 是否发射成功
 *
 */
bool hgui_gui_event_joy_button_emit(const hgui_gui_event_joy_button_t *button,hgui_gui_event_callback_t event_cb,void *usr);

/** \brief 获取游戏杆按键事件
 *
 * \param button hgui_gui_event_joy_button_t* 游戏杆按键事件，用于存储从事件中接收的数据,为NULL时返回NULL
 * \param type uint8_t 事件类型
 * \param eventparam void* 事件参数
 * \param eventparam_length size_t 事件参数长度
 * \param usr void* 用户参数
 * \return hgui_gui_event_joy_button_t* 失败返回NULL
 *
 */
hgui_gui_event_joy_button_t *hgui_gui_event_joy_button_get(hgui_gui_event_joy_button_t *button,uint8_t type,void *eventparam,size_t eventparam_length,void *usr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __HGUI_GUI_EVENT_H__
