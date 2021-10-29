#include "../Headers/DLLScriptHandler.h"


void DLLScriptHandler::setObj(GeometricObject* ref, int id){
	obj_id = id;
	obj = gameobject();
	obj.pos = ref->GetPosPtr();
	obj.rot = ref->GetRotPtr();
	obj.size = ref->GetSizePtr();
}

void DLLScriptHandler::Update(){
	if (error)
		throw std::exception("Using script with error");
	update(&obj);
}

void DLLScriptHandler::Start(){
	if(error)
		throw std::exception("Using script with error");
	start(&obj);
}

int DLLScriptHandler::SetDLL(const char* path) {
	hInst = ::LoadLibraryA(path);
	
	this->path = path;

	if (error = hInst == 0) 
		throw std::exception("DLL loading error");
	

	start = (STARTFUNCPTR)GetProcAddress(hInst, "start");
	update = (STARTFUNCPTR)GetProcAddress(hInst, "update");

	return 0;
}