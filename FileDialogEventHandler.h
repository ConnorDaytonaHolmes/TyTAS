#pragma once
#define STRICT_TYPED_ITEMIDS
#include <windows.h>
#include <shlobj.h>
#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <shlwapi.h>
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC
#include <new>
#include "Log.h"

namespace tytas::file {	

	class FileDialogEventHandler : public IFileDialogEvents, public IFileDialogControlEvents {
	public:

		FileDialogEventHandler() : _cRef(1) { };

		// IUnknown methods
		IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
		IFACEMETHODIMP_(ULONG) AddRef();
		IFACEMETHODIMP_(ULONG) Release();

		// IFileDialogEvents methods
		IFACEMETHODIMP OnFileOk(IFileDialog* pfd);
		IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
		IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
		IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
		IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
		IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
		IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };
		IFACEMETHODIMP OnTypeChange(IFileDialog* pfd) { return S_OK; };

		// IFileDialogControlEvents methods
		IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem);
		IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
		IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
		IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

	private:
		~FileDialogEventHandler() {};
		long _cRef;
	};

	HRESULT FileDialogEventHandler_CreateInstance(REFIID riid, void** ppv);
	PCIDLIST_ABSOLUTE FileDialogEventHandler_GetDefaultFolderIDList();
}