#pragma once
#include "ScriptingCore.h"
#include "jsapi.h"
#include "jsfriendapi.h"
class JSB_Manual
{
public:
	JSB_Manual();
	~JSB_Manual();	
};
//this should be out of class
void register_jsb_mine(JSContext* cx, JSObject* obj);