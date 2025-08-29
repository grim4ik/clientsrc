#pragma once

class MPEditor;

extern MPEditor g_Editor;

#define SMALL_RES_X 936
#define SMALL_RES_Y 600

#define LARGE_RES_X 1198
#define LARGE_RES_Y 768

extern bool Is60FPSMode;
#define FPSModifier Is60FPSMode ? 60:30

#define CLIENT_NAME "Lunatic Sea"
#define VERSION_NAME "An Ocean Fantasy (1001 Release)"
#define VERSION_NUM "version 1.0.1 (1001 Release) June 2024"