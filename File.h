#pragma once
#include <shlobj.h>
namespace tytas::file {

	HRESULT New();
	HRESULT Open();
	HRESULT Save();
	HRESULT SaveAs();

}
