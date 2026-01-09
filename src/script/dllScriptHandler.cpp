#include "dllScriptHandler.h"

void DLLScriptHandler::setObj(GeometricObject* ref, int id){
	obj_id = id;
	obj = gameobject();
	obj.pos = ref->GetPosPtr();
	obj.rot = ref->GetRotPtr();
	obj.size = ref->GetSizePtr();
}

void DLLScriptHandler::Update(){
	
	if (error)
		return;
	
	update(&obj);
}

void DLLScriptHandler::Start(){
	if (!error)
		start(&obj);
}

int DLLScriptHandler::SetDLL(const char* path) {
	hInst = ::LoadLibraryA(path);
	
	this->path = path;

	if (hInst == 0) {
		std::cout << "DLL loading error";
		error = true;
		return -1;
	}

	start = (STARTFUNCPTR)GetProcAddress(hInst, "start");
	update = (STARTFUNCPTR)GetProcAddress(hInst, "update");

	return 0;
}