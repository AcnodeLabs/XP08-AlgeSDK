#ifndef XDLL_HPP
#define XDLL_HPP



#ifdef WINDOWS

class XDyLib {
public:
	string path;
	HINSTANCE h;

	XDyLib(string path, string name) {
		this->path = path;
		h = Load(name);
	}

	XDyLib(HINSTANCE handle) {
		h = handle;
	}

	string getPath() { return path; }

	HINSTANCE Load(string crate) {
		MYPROC ProcAdd;
		BOOL fRunTimeLinkSuccess = FALSE;
		string  fp = path + "\\" + crate + ".dll";
		HINSTANCE hinstLib = LoadLibraryA(fp.c_str());
		if (hinstLib != NULL)
		{
			ProcAdd = (MYPROC)GetProcAddress(hinstLib, "init");

			// If the function address is valid, call the function.

			if (NULL != ProcAdd)
			{
				fRunTimeLinkSuccess = TRUE;
				(ProcAdd)(L"Message sent to the DLL function\n");
			}
		}
		return hinstLib;
	}

	void call(string func) {
		if (h) GetProcAddress(h, func.c_str())();
	}

	~XDyLib() {
		BOOL freeResult;
		freeResult = FreeLibrary(h);
		printf("Freed Lib");
	}
};
#else  //not windows

class XDyLib {
public:
	string path;
	//HINSTANCE h;
    int h;

	XDyLib(string path, string name) {
		this->path = path;
		h = Load(name);
	}

	//XDyLib(HINSTANCE handle) {
	//	h = handle;
	//}

	string getPath() { return path; }

    int Load(string name) {
        return 0;
    }

/*
	HINSTANCE Load(string crate) {
		MYPROC ProcAdd;
		BOOL fRunTimeLinkSuccess = FALSE;
		string  fp = path + "\\" + crate + ".dll";
		HINSTANCE hinstLib = LoadLibraryA(fp.c_str());
		if (hinstLib != NULL)
		{
			ProcAdd = (MYPROC)GetProcAddress(hinstLib, "init");

			// If the function address is valid, call the function.

			if (NULL != ProcAdd)
			{
				fRunTimeLinkSuccess = TRUE;
				(ProcAdd)(L"Message sent to the DLL function\n");
			}
		}
		return hinstLib;
	}
*/
	void call(string func) {
//		if (h) GetProcAddress(h, func.c_str())();
	}

	~XDyLib() {
//		BOOL freeResult;
//		freeResult = FreeLibrary(h);
		printf("Freed Lib");
	}
};
#endif
#endif //XDdyLib