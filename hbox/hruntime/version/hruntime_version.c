/***************************************************************
 * Name:      hruntime_version.c
 * Purpose:   实现hruntime_version接口
 * Author:    HYH (hyhsystem.cn)
 * Created:   2026-01-25
 * Copyright: HYH (hyhsystem.cn)
 * License:   MIT
 **************************************************************/
#include "hruntime_version.h"

const int hruntime_version_major=
#include "major"
;
const int hruntime_version_minor=
#include "minor"
;
const int hruntime_version_patch=
#include "patch"
;
const int hruntime_version_build=
#include "build"
;
