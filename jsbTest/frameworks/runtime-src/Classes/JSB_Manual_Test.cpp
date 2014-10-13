#include "JSB_Manual_Test.h"
#include "MyClass.h"
#include <iostream>

JSB_Manual::JSB_Manual()
{
}

JSB_Manual::~JSB_Manual()
{
}

JSClass  *jsb_my_class;
JSObject *jsb_my_prototype;

static void js_my_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOGINFO("jsbindings: finalizing JS object %p (LsLeafsoar)", obj);
}
static void js_myClass_Callback(void* selector,int value){
	JSObject *jsobj = (JSObject *)selector;
	jsval param[] = {
		INT_TO_JSVAL(value)
	};
	jsval retval;
	ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsobj), "callback", 1, param, &retval);
}

bool js_my_functionTest(JSContext *cx, uint32_t argc, jsval *vp)
{
	cocos2d::log("test functionTest by js");
	bool ok = true;
	JSB_PRECONDITION2(argc==1, cx, false, "Invalid Native Object");
	jsval* argvp = JS_ARGV(cx, vp);
	JSObject* jsobj = (JSObject*)JS_THIS_OBJECT(cx, vp);
	MyClass *myclass = (MyClass*)JS_GetPrivate(jsobj);

	myclass->testFunction(JSVAL_TO_INT(argvp[0]));
	JS_SET_RVAL(cx, vp, JSVAL_VOID);

	return ok;
}

bool js_myClass_constructor(JSContext *cx, uint32_t argc, jsval *vp){
	cocos2d::CCLog("js myClass constructor ..");

	if (argc == 0){
		JSObject *jsobj = JS_NewObject(cx, jsb_my_class, jsb_my_prototype, NULL);
		MyClass *newObject = new MyClass(jsobj, js_myClass_Callback);
		JS_SetPrivate(jsobj, (void *)newObject);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
		return true;
	}
	JS_ReportError(cx, "wrong number of arguments");
	return false;
}

void js_register_myClass(JSContext *cx, JSObject *global) {
	jsb_my_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_my_class->name = "MyClass";
	jsb_my_class->addProperty = JS_PropertyStub;
	jsb_my_class->delProperty = JS_DeletePropertyStub;
	jsb_my_class->getProperty = JS_PropertyStub;
	jsb_my_class->setProperty = JS_StrictPropertyStub;
	jsb_my_class->enumerate = JS_EnumerateStub;
	jsb_my_class->resolve = JS_ResolveStub;
	jsb_my_class->convert = JS_ConvertStub;
	jsb_my_class->finalize = js_my_finalize;
	jsb_my_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		{ 0, 0, 0, 0, 0 }
	};
	static JSFunctionSpec funcs[] = { 
		JS_FN("functionTest", js_my_functionTest, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FS_END
	};
	static JSFunctionSpec st_funcs[] = {
		JS_FS_END
	};
	jsb_my_prototype = JS_InitClass(
		cx, global,
		NULL, // parent proto
		jsb_my_class,
		js_myClass_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);	
}

void register_jsb_mine(JSContext* cx, JSObject* obj){
	JSObject *myBinding = JS_NewObject(cx, NULL, NULL, NULL);
	JS::RootedValue  myBindingVal(cx);
	myBindingVal = OBJECT_TO_JSVAL(myBinding);
	JS_SetProperty(cx, obj, "MyBinding", myBindingVal);

	js_register_myClass(cx, myBinding);		
}

