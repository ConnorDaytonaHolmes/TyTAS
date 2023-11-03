#include "FileDialogEventHandler.h"

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace tytas::file {

	PCIDLIST_ABSOLUTE defaultFolderIDList;
	
	IFACEMETHODIMP FileDialogEventHandler::QueryInterface(REFIID riid, void** ppv) {
		static const QITAB qit[]{
			QITABENT(FileDialogEventHandler, IFileDialogEvents),
			QITABENT(FileDialogEventHandler, IFileDialogControlEvents),
			{ 0 },
		};
		return QISearch(this, qit, riid, ppv);
	}

	IFACEMETHODIMP_(ULONG) FileDialogEventHandler::AddRef() {
		return InterlockedIncrement(&_cRef);
	}

	IFACEMETHODIMP_(ULONG) FileDialogEventHandler::Release() {
		long cRef = InterlockedDecrement(&_cRef);
		if (!cRef)
			delete this;
		return cRef;
	}

	IFACEMETHODIMP FileDialogEventHandler::OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem) {
		IFileDialog* pfd = NULL;
		HRESULT hr = pfdc->QueryInterface(&pfd);
		if (SUCCEEDED(hr))
		{
			pfd->Release();
		}
		return hr;
	}

	IFACEMETHODIMP FileDialogEventHandler::OnFileOk(IFileDialog* pfd) {
		IShellItem* containingFolder;
		HRESULT hr = pfd->GetFolder(&containingFolder);
		if (SUCCEEDED(hr)) {
			PIDLIST_ABSOLUTE newDefFolder;
			hr = SHGetIDListFromObject(containingFolder, &newDefFolder);
			if (SUCCEEDED(hr)) {
				defaultFolderIDList = newDefFolder;				
			}
			else {
				Log::Error("Failed to retrieve IDList from containing foldler.", hr);
			}
		}
		else {
			Log::Error("Failed to get current folder from FileDialog instance.", hr);
		}
		return hr;
	}

	PCIDLIST_ABSOLUTE FileDialogEventHandler_GetDefaultFolderIDList() {
		return defaultFolderIDList;
	}

	HRESULT FileDialogEventHandler_CreateInstance(REFIID riid, void** ppv) {
		*ppv = NULL;
		FileDialogEventHandler* fDialogEventHandler = new (std::nothrow) FileDialogEventHandler();
		HRESULT hr = fDialogEventHandler ? S_OK : E_OUTOFMEMORY;
		if (SUCCEEDED(hr)) {
			hr = fDialogEventHandler->QueryInterface(riid, ppv);
			fDialogEventHandler->Release();
		}
		return hr;
	}
}