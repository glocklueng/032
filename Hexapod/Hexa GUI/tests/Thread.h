#ifndef _THREAD_H_
#define _THREAD_H_ ( 1 )

#include <windows.h>

#include <windows.h>
#include <iostream>
#include <process.h>
#include <memory>
 
namespace Multi
{
    typedef unsigned int (__stdcall CThreadFunction)(void*);
    template<typename T> CThreadFunction* CThreadFunctionCast(
        unsigned int (__stdcall* pFunction)(T*))
    {
        return reinterpret_cast<CThreadFunction*>(pFunction);
    }
 
    class auto_handle
    {
    public:
        auto_handle(HANDLE Handle): m_h(Handle) {}
        ~auto_handle() { CloseHandle(m_h); }
        HANDLE get() { return m_h; }
 
    private:
        HANDLE m_h;
    };
 
    template<typename T> class reference_wrapper
    {
        reference_wrapper(T& value): m_value(value) {}
 
    private:
        T& m_value;
    };
 
    int get_err_no();

    namespace Thread
    {
        struct Error
        {
            int CreationError;
            DWORD ResumeError;
            DWORD TerminateError;
        };
 
        template<typename ParamType, typename ClassType, typename ReturnType>
            struct Params
        {
            ParamType* Param;
            ClassType* Instance;
            ReturnType (ClassType::* Function)(ParamType* Param);
        };
 
        template<typename ParamType, typename ClassType, typename ReturnType>
            unsigned int __stdcall ThreadFnc(
            Params<ParamType, ClassType, ReturnType>* pParam)
        {
            (pParam->Instance->*pParam->Function)(pParam->Param);
            delete pParam;
            return 0;
        }
 
        template<typename ParamType, typename ClassType, typename ReturnType>
            Error Create(ReturnType (ClassType::* Function)(ParamType* Param),
            ClassType* Instance, ParamType* Param)
        {
            const DWORD dwFail = static_cast<DWORD>(-1);
            Error ReturnErr = {};
            std::auto_ptr< Params<ParamType, ClassType, ReturnType> > pParams(
                new Params<ParamType, ClassType, ReturnType>);
            pParams->Param = Param;
            pParams->Instance = Instance;
            pParams->Function = Function;
            Multi::auto_handle hThread = reinterpret_cast<HANDLE>(
                _beginthreadex(NULL, 0, Multi::CThreadFunctionCast(
                    &ThreadFnc<ParamType, ClassType, ReturnType>),
                pParams.get(), CREATE_SUSPENDED, NULL) );
            if (hThread.get() == 0)
            {
                ReturnErr.CreationError = Multi::get_err_no();
                return ReturnErr;
            }
            if (ResumeThread(hThread.get()) == dwFail)
            {
                ReturnErr.ResumeError = GetLastError();
                if ( !TerminateThread(hThread.get(), 1) )
                    ReturnErr.TerminateError = GetLastError();
                return ReturnErr;
            }
            pParams.release();
            return ReturnErr;
        }
    }
}
 
#endif
