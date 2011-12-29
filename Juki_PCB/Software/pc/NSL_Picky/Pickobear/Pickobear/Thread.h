#ifndef _THREAD_H_
#define _THREAD_H_ ( 1 )

#include <windows.h>

template<typename ParamType, typename ClassType, typename ReturnType> struct ThreadParams
{
    ParamType* Param;
    ClassType* Instance;
    ReturnType (ClassType::* Function)(ParamType* Param);
    HANDLE hProcess;
};
 
template<typename ParamType, typename ClassType, typename ReturnType> DWORD WINAPI ThreadFnc(void* pParam)
{
    ThreadParams<ParamType, ClassType, ReturnType>* pParams = (ThreadParams<ParamType, ClassType, ReturnType>*)pParam;
    (pParams->Instance->*pParams->Function)(pParams->Param);
    CloseHandle(pParams->hProcess);
    delete pParams;
    return 0;
}
 
template<typename ParamType, typename ClassType, typename ReturnType> DWORD CPPCreateThread(ReturnType (ClassType::* Function)(ParamType* Param), ClassType* Instance, ParamType* Param)
{
    ThreadParams<ParamType, ClassType, ReturnType>* pParams = new ThreadParams<ParamType, ClassType, ReturnType>;
    pParams->Param = Param;
    pParams->Instance = Instance;
    pParams->Function = Function;
    HANDLE hProcess = CreateThread(NULL, NULL, &ThreadFnc<ParamType, ClassType, ReturnType>, pParams, CREATE_SUSPENDED, NULL);
    if (hProcess == NULL)
        return GetLastError();
    pParams->hProcess = hProcess;
    ResumeThread(hProcess);
    DWORD dwErr = GetLastError();
    if (dwErr != ERROR_SUCCESS)
    {
        CloseHandle(hProcess);
        delete pParams;
    }
    return dwErr;
}

#endif
