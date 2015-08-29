@set ADB_PATH="\platform-tools\adb"
%ANDROID_SDK_ROOT%%ADB_PATH% install "..\bin\debug\android\c2xa-osc-app-debug.apk"
%ANDROID_SDK_ROOT%%ADB_PATH% shell am start -n "org.jagsc.c2xaoscapp/org.cocos2dx.cpp.AppActivity"