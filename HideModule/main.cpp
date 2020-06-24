#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>


typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _PEB_LDR_DATA {
    ULONG                   Length;
    BOOLEAN                 Initialized;
    PVOID                   SsHandle;
    LIST_ENTRY              InLoadOrderModuleList;
    LIST_ENTRY              InMemoryOrderModuleList;
    LIST_ENTRY              InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY          InLoadOrderModuleList;  
    LIST_ENTRY          InMemoryOrderModuleList;
    LIST_ENTRY          InInitializationOrderModuleList; 
    LPVOID              BaseAddress;  
    LPVOID              EntryPoint;  
    ULONG               SizeOfImage;
    UNICODE_STRING      FullDllName;
    UNICODE_STRING      BaseDllName;
    ULONG               Flags;
    SHORT               LoadCount;
    SHORT               TlsIndex;
    HANDLE              SectionHandle;
    ULONG               CheckSum;
    ULONG               TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;



int main()
{

    
    PPEB_LDR_DATA pLdr;
    PLIST_ENTRY pBack, pNext;
    PLDR_DATA_TABLE_ENTRY pLdm;
    

    HMODULE hModule = GetModuleHandleA("ntdll.dll");

    _asm
    {
        mov eax, fs: [0x30] ;　　　　　　　　　
        mov eax, [eax + 0x0c];
        mov pLdr, eax;
    }



    pBack = &(pLdr->InLoadOrderModuleList);         
    pNext = pBack->Flink;              
    do
    {
        pLdm = CONTAINING_RECORD(pNext, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList); 

        if (hModule == pLdm->BaseAddress)                                    
        {                                             
            pLdm->InLoadOrderModuleList.Blink->Flink =                 
                pLdm->InLoadOrderModuleList.Flink;

            pLdm->InLoadOrderModuleList.Flink->Blink =
                pLdm->InLoadOrderModuleList.Blink;

            pLdm->InInitializationOrderModuleList.Blink->Flink =
                pLdm->InInitializationOrderModuleList.Flink;

            pLdm->InInitializationOrderModuleList.Flink->Blink =
                pLdm->InInitializationOrderModuleList.Blink;

            pLdm->InMemoryOrderModuleList.Blink->Flink =
                pLdm->InMemoryOrderModuleList.Flink;

            pLdm->InMemoryOrderModuleList.Flink->Blink =
                pLdm->InMemoryOrderModuleList.Blink;
            break;
        }
        pNext = pNext->Flink;
    } while (pBack != pNext);
    


    system("pause");
    return 0;
}