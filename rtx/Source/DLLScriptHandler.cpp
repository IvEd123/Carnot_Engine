#include "../Headers/DLLScriptHandler.h"


void DLLScriptHandler::setObj(GeometricObject* ref){
	obj = gameobject();
	obj.pos = ref->GetPosPtr();
	obj.rot = ref->GetRotPtr();
	obj.size = ref->GetSizePtr();
}

void DLLScriptHandler::Update(){
	
	if (!error)
		update(&obj);
	else
		return;
}

void DLLScriptHandler::Start(){
	if (!error)
		start(&obj);
	else
		return;
}



int DLLScriptHandler::SetDLL(const char* path) {
	hInst = ::LoadLibraryA(path);
	
	if (hInst == 0) {
		std::cout << "DLL loading error";
		error = true;
		return -1;
	}

	start = (STARTFUNCPTR)GetProcAddress(hInst, "start");
	update = (STARTFUNCPTR)GetProcAddress(hInst, "update");

	return 0;
}