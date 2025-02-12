using System.Collections;
using System.Reflection;

namespace System
{
	struct DbgRawAllocData
	{
		public Type mType;
		public void* mMarkFunc;
		public int32 mMaxStackTrace;

		public struct Unmarked<T>
		{
			public static DbgRawAllocData sRawAllocData;
			public static this()
			{
				sRawAllocData.mMarkFunc = null;
				sRawAllocData.mMaxStackTrace = 1;
				sRawAllocData.mType = typeof(T);
			}
		}
	}

	[CRepr]
	struct VarArgs
	{
#if BF_PLATFORM_WINDOWS
		void* mVAList;
#else
		int[3] mVAList; // Conservative size for va_list
#endif

		[Intrinsic("va_start")]
		static extern void Start(void* vaList);
		[Intrinsic("va_end")]
		static extern void End(void* vaList);
		[Intrinsic("va_arg")]
		static extern void Arg(void* vaList, void* destPtr, int32 typeId);

		[Inline]
		public mixin Start() mut
		{
			Start(&mVAList);
		}

		[Inline]
		public mixin End() mut
		{
			End(&mVAList);
		}

		[Inline]
		public mixin Get<T>() mut
		{
			T val = ?;
			Arg(&mVAList, &val, (.)typeof(T).TypeId);
			val
		}

		public void* ToVAList() mut
		{
#if BF_PLATFORM_WINDOWS
			return mVAList;
#else
			return &mVAList;
#endif
		}
	}

	[AlwaysInclude]
    static class Internal
    {
		[Intrinsic("cast")]
		public static extern Object UnsafeCastToObject(void* ptr);
		[Intrinsic("cast")]
		public static extern void* UnsafeCastToPtr(Object obj);
		[CallingConvention(.Cdecl), NoReturn]
		public static extern void ThrowIndexOutOfRange(int stackOffset = 0);
		[CallingConvention(.Cdecl), NoReturn]
		public static extern void FatalError(String error, int stackOffset = 0);
		[Intrinsic("memcpy")]
		public static extern void MemCpy(void* dest, void* src, int length, int32 align = 1, bool isVolatile = false);
		[Intrinsic("memmove")]
		public static extern void MemMove(void* dest, void* src, int length, int32 align = 1, bool isVolatile = false);
		[Intrinsic("memset")]
		public static extern void MemSet(void* addr, uint8 val, int length, int32 align = 1, bool isVolatile = false);
		[Intrinsic("malloc")]
		public static extern void* Malloc(int size);
		[Intrinsic("free")]
		public static extern void Free(void* ptr);
		[LinkName("malloc")]
		public static extern void* StdMalloc(int size);
		[LinkName("free")]
		public static extern void StdFree(void* ptr);
		[Intrinsic("returnaddress")]
		public static extern void* GetReturnAddress(int32 level = 0);
		[CallingConvention(.Cdecl)]
		public static extern void* VirtualAlloc(int size, bool canExecute, bool canWrite);
		[CallingConvention(.Cdecl)]
		public static extern int32 CStrLen(char8* charPtr);
		[CallingConvention(.Cdecl)]
		public static extern int64 GetTickCountMicro();
		[CallingConvention(.Cdecl)]
		public static extern void BfDelegateTargetCheck(void* target);
		[CallingConvention(.Cdecl), AlwaysInclude]
		public static extern void* LoadSharedLibrary(char8* filePath);
		[CallingConvention(.Cdecl), AlwaysInclude]
		public static extern void LoadSharedLibraryInto(char8* filePath, void** libDest);
		[CallingConvention(.Cdecl), AlwaysInclude]
		public static extern void* GetSharedProcAddress(void* libHandle, char8* procName);
		[CallingConvention(.Cdecl), AlwaysInclude]
		public static extern void GetSharedProcAddressInto(void* libHandle, char8* procName, void** procDest);
		[CallingConvention(.Cdecl)]
		public static extern char8* GetCommandLineArgs();
		[CallingConvention(.Cdecl)]
		public static extern void ProfilerCmd(char8* str);
		[CallingConvention(.Cdecl)]
		public static extern void ReportMemory();
		[CallingConvention(.Cdecl)]
		public static extern void ObjectDynCheck(Object obj, int32 typeId, bool allowNull);
		[CallingConvention(.Cdecl)]
		public static extern void ObjectDynCheckFailed(Object obj, int32 typeId);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_ObjectCreated(Object obj, int size, ClassVData* classVData);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_ObjectCreatedEx(Object obj, int size, ClassVData* classVData);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_ObjectAllocated(Object obj, int size, ClassVData* classVData);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_ObjectAllocatedEx(Object obj, int size, ClassVData* classVData);
		[CallingConvention(.Cdecl)]
		public static extern int Dbg_PrepareStackTrace(int baseAllocSize, int maxStackTraceDepth);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_ObjectStackInit(Object object, ClassVData* classVData);
		[CallingConvention(.Cdecl)]
		public static extern Object Dbg_ObjectAlloc(TypeInstance typeInst, int size);
		[CallingConvention(.Cdecl)]
		public static extern Object Dbg_ObjectAlloc(ClassVData* classVData, int size, int align, int maxStackTraceDepth);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_ObjectPreDelete(Object obj);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_ObjectPreCustomDelete(Object obj);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_MarkObjectDeleted(Object obj);
		[CallingConvention(.Cdecl)]
		public static extern void* Dbg_RawAlloc(int size);
		[CallingConvention(.Cdecl)]
		public static extern void* Dbg_RawObjectAlloc(int size);
		[CallingConvention(.Cdecl)]
		public static extern void* Dbg_RawAlloc(int size, DbgRawAllocData* rawAllocData);
		[CallingConvention(.Cdecl)]
		public static extern void Dbg_RawFree(void* ptr);

		[CallingConvention(.Cdecl), AlwaysInclude]
		static extern void Shutdown();
		[CallingConvention(.Cdecl)]
		static extern void Test_Init(char8* testData);
		[CallingConvention(.Cdecl)]
		static extern void Test_Error(char8* error);
		[CallingConvention(.Cdecl)]
		static extern void Test_Write(char8* str);
		[CallingConvention(.Cdecl)]
		static extern int32 Test_Query();
		[CallingConvention(.Cdecl)]
		static extern void Test_Finish();

		static void* sModuleHandle;
		[AlwaysInclude]
		static void SetModuleHandle(void* handle)
		{
			sModuleHandle = handle;
		}
		[AlwaysInclude]
		static void AddRtFlags(int32 flags)
		{
			Runtime.[Friend]sExtraFlags |= (.)flags;
		}

		public static T* AllocRawArrayUnmarked<T>(int size)
		{
#if BF_ENABLE_REALTIME_LEAK_CHECK
			if (Compiler.IsComptime)
				return new T[size]*(?);
			// We don't want to use the default mark function because the GC will mark the entire array,
			//  whereas we have a custom marking routine because we only want to mark up to mSize
			return (T*)Internal.Dbg_RawAlloc(size * strideof(T), &DbgRawAllocData.Unmarked<T>.sRawAllocData);
#else
			return new T[size]*(?);
#endif
		}

		public static Object ObjectAlloc(TypeInstance typeInst, int size)
		{
#if BF_ENABLE_OBJECT_DEBUG_FLAGS
			return Dbg_ObjectAlloc(typeInst, size);
#else
			void* ptr = Malloc(size);
			return *(Object*)(&ptr);
#endif
		}

		[Error("Cannot be called directly"), SkipCall]
		static void SetDeleted1(void* dest);		
		[Error("Cannot be called directly"), SkipCall]
		static void SetDeleted4(void* dest);		
		[Error("Cannot be called directly"), SkipCall]
		static void SetDeleted8(void* dest);		
		[Error("Cannot be called directly"), SkipCall]
		static void SetDeleted16(void* dest);		
		[Error("Cannot be called directly"), SkipCall]
		static extern void SetDeletedX(void* dest, int size);		
		[Error("Cannot be called directly"), SkipCall]
		static extern void SetDeleted(void* dest, int size, int32 align);
		[Error("Cannot be called directly"), SkipCall]
		static extern void SetDeletedArray(void* dest, int size, int32 align, int arrayCount);

		public static int MemCmp(void* memA, void* memB, int length)
		{
			uint8* p0 = (uint8*)memA;
			uint8* p1 = (uint8*)memB;

			uint8* end0 = p0 + length;
			while (p0 < end0)
			{
				int diff = *(p0++) - *(p1++);
				if (diff != 0)
					return diff;
			}
			return 0;
		}

		[Inline]
		public static int GetArraySize<T>(int length)
		{
			if (sizeof(T) == strideof(T))
			{
				return length * sizeof(T);
			}
			else
			{
				int size = strideof(T) * (length - 1) + sizeof(T);
				if (size < 0)
					return 0;
				return size;
			}
		}

		[AlwaysInclude]
        public static String[] CreateParamsArray()
		{
			char8* cmdLine = GetCommandLineArgs();
			//Windows.MessageBoxA(default, scope String()..AppendF("CmdLine: {0}", StringView(cmdLine)), "HI", 0);

			String[] strVals = null;
			for (int pass = 0; pass < 2; pass++)
			{
				int argIdx = 0;

				void HandleArg(int idx, int len)
				{
					if (pass == 1)
					{
						var str = new String(len);
						char8* outStart = str.Ptr;
						char8* outPtr = outStart;
						bool inQuote = false;

						for (int i < len)
						{
							char8 c = cmdLine[idx + i];

							if (!inQuote)
							{
								if (c == '"')
								{
									inQuote = true;
									continue;
								}
							}
							else
							{
								if (c == '^')
								{
									i++;
									c = cmdLine[idx + i];
								}
								else if (c == '\"')
								{
									if (cmdLine[idx + i + 1] == '\"')
									{
										*(outPtr++) = '\"';
										i++;
										continue;
									}
									inQuote = false;
									continue;
								}
							}

							*(outPtr++) = c;
						}
						str.[Friend]mLength = (.)(outPtr - outStart);
						strVals[argIdx] = str;
					}

					++argIdx;
				}

				int firstCharIdx = -1;
				bool inQuote = false;
				int i = 0;
				while (true)
				{
				    char8 c = cmdLine[i];
					if (c == 0)
						break;
				    if ((c.IsWhiteSpace) && (!inQuote))
				    {
				        if (firstCharIdx != -1)
				        {
				            HandleArg(firstCharIdx, i - firstCharIdx);
				            firstCharIdx = -1;
				        }
				    }
				    else
				    {
				        if (firstCharIdx == -1)
				            firstCharIdx = i;
						if (c == '^')
						{	
							i++;
						}
				        if (c == '"')
				            inQuote = !inQuote;
				        else if ((inQuote) && (c == '\\'))
						{
				            c = cmdLine[i + 1];
							if (c == '"')
								i++;
						}
				    }
					i++;
				}
				if (firstCharIdx != -1)
					HandleArg(firstCharIdx, i - firstCharIdx);
				if (pass == 0)
					strVals = new String[argIdx];
			}

		    return strVals;
		}

		[AlwaysInclude]
        public static void DeleteStringArray(String[] arr)
        {
            for (var str in arr)
                delete str;
            delete arr;
        }

        extern static this();
        extern static ~this();
    }

	struct CRTAlloc
	{
		public void* Alloc(int size, int align)
		{
			return Internal.StdMalloc(size);
		}

		public void Free(void* ptr)
		{
			Internal.StdFree(ptr);
		}
	}

	static
	{
		public static CRTAlloc gCRTAlloc;
	}
}
