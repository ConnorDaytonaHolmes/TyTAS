#include <format>
#include <string>
#include "FileDialogEventHandler.h"
#include "File.h"

namespace tytas::file {

    const COMDLG_FILTERSPEC SAVETYPES[] = {
        { L"Ty TAS (*.tyt)", L"*.tyt" }
    };

    HRESULT New()
    {
        return E_NOTIMPL;
    }

    HRESULT Open()
    {
        //Create an instance of FileOpenDialog
        IFileDialog* pfd = NULL;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pfd));
        if (FAILED(hr)) {
            Log::Error("Failed to create FileOpenDialog Instance");
            return hr;
        }

        //Create a FileDiaLog::EventHandler
        IFileDialogEvents* pfde = NULL;
        hr = FileDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
        if (FAILED(hr)) {
            Log::Error("Failed to create FileDiaLog::EventHandler Instance");
            pfd->Release();
            return hr;
        }

        //Hook the handler to the dialog instance
        DWORD dwCookie;
        hr = pfd->Advise(pfde, &dwCookie);
        if (FAILED(hr)) {
            Log::Error("Failed to hook event handler to dialog instance.", hr);
            pfde->Release();
            pfd->Release();
            return hr;
        }

        //Retrieve the FileDialog options
        DWORD dwFlags;
        hr = pfd->GetOptions(&dwFlags);
        if (FAILED(hr)) {
            Log::Error("Failed to get dialog options.", hr);
            pfd->Unadvise(dwCookie);
            pfd->Release();
            pfde->Release();
            return hr;
        }

        //Append the FORCEFILESYSTEM setting to the FileDialog options
        hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
        if (SUCCEEDED(hr)) {
            //Set the file type to only "*.tyt"
            hr = pfd->SetFileTypes(ARRAYSIZE(SAVETYPES), SAVETYPES);
            if (SUCCEEDED(hr)) {
                //Set the default index to be 1 (*.tyt) [zero is all documents]
                hr = pfd->SetFileTypeIndex(1);
                if (SUCCEEDED(hr)) {
                    //Set the default extension to be "*.tyt" file
                    hr = pfd->SetDefaultExtension(L"tyt");
                    if (SUCCEEDED(hr)) {
                        //Try set the default folder to open in, if not, defaults to Documents
                        IShellItem* defaultFolder;
                        PCIDLIST_ABSOLUTE folderIDList = FileDialogEventHandler_GetDefaultFolderIDList();
                        hr = SHCreateItemFromIDList(folderIDList, IID_PPV_ARGS(&defaultFolder));
                        if (SUCCEEDED(hr)) {
                            Log::WriteLine("Successfully set default folder.");
                            pfd->SetDefaultFolder(defaultFolder);
                            defaultFolder->Release();
                        }
                        else {
                            Log::Error("Failed to set default folder.", hr);
                        }
                        //Show the file dialog
                        hr = pfd->Show(NULL);
                        if (SUCCEEDED(hr)) {
                            //Obtain the result once the user clicks the open button
                            IShellItem* psiResult;
                            hr = pfd->GetResult(&psiResult);
                            if (SUCCEEDED(hr)) {
                                PWSTR pszFilePath = NULL;
                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                                if (SUCCEEDED(hr)) {
                                    TaskDialog(
                                        NULL,
                                        NULL,
                                        L"TY TAS file opened.", //Window title
                                        pszFilePath,
                                        NULL,
                                        TDCBF_OK_BUTTON,
                                        TD_INFORMATION_ICON,
                                        NULL);
                                    CoTaskMemFree(pszFilePath);
                                }
                                psiResult->Release();
                            }
                        }                        
                    }
                }
            }
        }
        pfd->Unadvise(dwCookie);
        pfd->Release();
        pfde->Release();
        return hr;
    }

    HRESULT Save()
    {
        return E_NOTIMPL;
    }

    HRESULT SaveAs()
    {
        return E_NOTIMPL;
    }
}