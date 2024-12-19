# 说明

本场景主要针对适用于处理器性能不高，屏幕分辨率较低的场景。

# 应用

应用的参数一般由宏定义配置，可配置的宏定义如下：

|               宏定义                | 说明                                               | 备注                                                         |
| :---------------------------------: | -------------------------------------------------- | ------------------------------------------------------------ |
|      `HGUI_SCENE1_APP_DRIVER`       | Gui驱动指针，NULL表示使用默认GUI驱动。             |                                                              |
|       `HGUI_SCENE1_APP_WIDTH`       | 屏幕宽度，默认为128                                |                                                              |
|      `HGUI_SCENE1_APP_HEIGHT`       | 屏幕 高度，默认为64                                |                                                              |
|  `HGUI_SCENE1_APP_ON_INIT_SUCCESS`  | 定义当应用初始化成功时执行的语句，默认为空语句块。 | 语句中可使用局部变量`app`访问应用数据，可使用局部变量`usr`访问用户自定义参数。 |
|  `HGUI_SCENE1_APP_ON_INIT_FAILURE`  | 定义当应用初始化失败时执行的语句，默认为空语句块。 | 语句中可使用局部变量`app`访问应用数据，可使用局部变量`usr`访问用户自定义参数。 |
|  `HGUI_SCENE1_APP_ON_UPDATE_BEGIN`  | 定义当应用更新开始时执行的语句，默认为空语句块。   | 语句中可使用局部变量`app`访问应用数据，可使用局部变量`usr`访问用户自定义参数。 |
|   `HGUI_SCENE1_APP_ON_UPDATE_END`   | 定义当应用结束时执行的语句，默认为空语句块。       | 语句中可使用局部变量`app`访问应用数据，可使用局部变量`usr`访问用户自定义参数。 |
| `HGUI_SCENE1_APP_ON_UPDATE_SUCCESS` | 定义当应用更新成功时执行的语句，默认为空语句块。   | 语句中可使用局部变量`app`访问应用数据，可使用局部变量`usr`访问用户自定义参数。 |
| `HGUI_SCENE1_APP_ON_UPDATE_FAILURE` | 定义当应用更新失败时执行的语句，默认为空语句块。   | 语句中可使用局部变量`app`访问应用数据，可使用局部变量`usr`访问用户自定义参数。 |

配置完宏定义后，用户需要使用`#include HGUI_SCENE1_APP_IMPLEMENT`实现应用。

示例如下：

```c++
/*
 * 本示例内容应当存在于用户的C/C++源代码中，且APP应当只实现一次
 */

//定义Gui驱动指针
#define HGUI_SCENE1_APP_DRIVER      NULL

//定义屏幕宽度
#define HGUI_SCENE1_APP_WIDTH       128

//定义屏幕高度
#define HGUI_SCENE1_APP_HEIGHT      64

//定义初始化成功时执行的语句
#define HGUI_SCENE1_APP_ON_INIT_SUCCESS     {}

//定义初始化失败时执行的语句
#define HGUI_SCENE1_APP_ON_INIT_FAILURE     {}

//定义初始化开始时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_BEGIN     {}

//定义初始化结束时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_END       {}

//定义初始化更新成功时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_SUCCESS   {}

//定义初始化更新失败时执行的语句
#define HGUI_SCENE1_APP_ON_UPDATE_FAILURE   {}

//实现APP
#include HGUI_SCENE1_APP_IMPLEMENT
```

