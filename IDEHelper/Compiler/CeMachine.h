#pragma once

#include "BfSystem.h"
#include "BfModule.h"
#include "BeefySysLib/util/Heap.h"
#include "BeefySysLib/util/AllocDebug.h"

NS_BF_BEGIN

class BfParser;
class BfReducer;
class BfMethodInstance;
class BeModule;
class BeContext;
class BeDbgLoc;
class BeType;
class BeValue;
class BeConstant;
class BeInst;
class BeDbgFile;
class BePhiInst;
class BeFunction;
class BeSwitchInst;
class BeGlobalVariable;
class CeMachine;
class CeFunction;

#define CEOP_SIZED(OPNAME) \
	CeOp_##OPNAME##_8, \
	CeOp_##OPNAME##_16, \
	CeOp_##OPNAME##_32, \
	CeOp_##OPNAME##_64, \
	CeOp_##OPNAME##_X

#define CEOP_SIZED_FLOAT(OPNAME) \
	CeOp_##OPNAME##_F32, \
	CeOp_##OPNAME##_F64

#define CEOP_SIZED_NUMERIC(OPNAME) \
	CeOp_##OPNAME##_I8, \
	CeOp_##OPNAME##_I16, \
	CeOp_##OPNAME##_I32, \
	CeOp_##OPNAME##_I64

#define CEOP_SIZED_UNUMERIC(OPNAME) \
	CeOp_##OPNAME##_U8, \
	CeOp_##OPNAME##_U16, \
	CeOp_##OPNAME##_U32, \
	CeOp_##OPNAME##_U64

#define CEOP_SIZED_NUMERIC_PLUSF(OPNAME) \
	CeOp_##OPNAME##_I8, \
	CeOp_##OPNAME##_I16, \
	CeOp_##OPNAME##_I32, \
	CeOp_##OPNAME##_I64, \
	CeOp_##OPNAME##_F32, \
	CeOp_##OPNAME##_F64

enum CeErrorKind
{
	CeErrorKind_None,
	CeErrorKind_Error,
	CeErrorKind_GlobalVariable,
	CeErrorKind_FunctionPointer,
	CeErrorKind_Intrinsic,
	CeErrorKind_ObjectDynCheckFailed
};

enum CeOp : int16
{
	CeOp_InvalidOp,
	CeOp_Ret,
	CeOp_SetRetType,
	CeOp_Jmp,
	CeOp_JmpIf,
	CeOp_JmpIfNot,

	CeOp_Error,	
	CeOp_DynamicCastCheck,
	CeOp_GetReflectType,
	CeOp_GetString,
	CeOp_Malloc,
	CeOp_Free,
	
	CeOp_MemSet,
	CeOp_MemSet_Const,
	CeOp_MemCpy,
	
	CeOp_FrameAddr_32,
	CeOp_FrameAddr_64,
	CeOp_FrameAddrOfs_32,

	CeOp_ConstData,
	CeOp_ConstDataRef,
	CeOp_Zero,
	CEOP_SIZED(Const),
	CEOP_SIZED(Load),
	CEOP_SIZED(Store),
	CEOP_SIZED(Move),
	CEOP_SIZED(Push),
	CEOP_SIZED(Pop),

	CeOp_AdjustSP,
	CeOp_AdjustSPNeg,
	CeOp_AdjustSPConst,
	CeOp_GetSP,
	CeOp_SetSP,
	CeOp_GetStaticField,
	CeOp_GetMethod,
	CeOp_GetMethod_Inner,
	CeOp_GetMethod_Virt,
	CeOp_GetMethod_IFace,
	CeOp_Call,
	
	CeOp_Conv_I8_I16,
	CeOp_Conv_I8_I32,
	CeOp_Conv_I8_I64,
	CeOp_Conv_I8_F32,
	CeOp_Conv_I8_F64,
	CeOp_Conv_I16_I32,
	CeOp_Conv_I16_I64,
	CeOp_Conv_I16_F32,
	CeOp_Conv_I16_F64,
	CeOp_Conv_I32_I64,
	CeOp_Conv_I32_F32,
	CeOp_Conv_I32_F64,	
	CeOp_Conv_I64_F32,
	CeOp_Conv_I64_F64,
	CeOp_Conv_U8_U16,
	CeOp_Conv_U8_U32,
	CeOp_Conv_U8_U64,
	CeOp_Conv_U8_F32,
	CeOp_Conv_U8_F64,
	CeOp_Conv_U16_U32,
	CeOp_Conv_U16_U64,
	CeOp_Conv_U16_F32,
	CeOp_Conv_U16_F64,
	CeOp_Conv_U32_U64,
	CeOp_Conv_U32_F32,
	CeOp_Conv_U32_F64,
	CeOp_Conv_U64_F32,
	CeOp_Conv_U64_F64,
	CeOp_Conv_F32_I8,
	CeOp_Conv_F32_I16,
	CeOp_Conv_F32_I32,
	CeOp_Conv_F32_I64,
	CeOp_Conv_F32_F64,
	CeOp_Conv_F64_I8,
	CeOp_Conv_F64_I16,
	CeOp_Conv_F64_I32,
	CeOp_Conv_F64_I64,
	CeOp_Conv_F64_F32,

	CEOP_SIZED_NUMERIC_PLUSF(Abs),
	CEOP_SIZED_NUMERIC_PLUSF(AddConst),
	CEOP_SIZED_NUMERIC_PLUSF(Add),
	CEOP_SIZED_NUMERIC_PLUSF(Sub),
	CEOP_SIZED_NUMERIC_PLUSF(Mul),	
	CEOP_SIZED_NUMERIC_PLUSF(Div),
	CEOP_SIZED_UNUMERIC(Div),
	CEOP_SIZED_NUMERIC_PLUSF(Mod),
	CEOP_SIZED_UNUMERIC(Mod),
	CEOP_SIZED_NUMERIC(And),
	CEOP_SIZED_NUMERIC(Or),
	CEOP_SIZED_NUMERIC(Xor),
	CEOP_SIZED_NUMERIC(Shl),
	CEOP_SIZED_NUMERIC(Shr),
	CEOP_SIZED_UNUMERIC(Shr),
	
	CEOP_SIZED_FLOAT(Acos),
	CEOP_SIZED_FLOAT(Asin),
	CEOP_SIZED_FLOAT(Atan),
	CEOP_SIZED_FLOAT(Atan2),
	CEOP_SIZED_FLOAT(Ceiling),
	CEOP_SIZED_FLOAT(Cos),
	CEOP_SIZED_FLOAT(Cosh),
	CEOP_SIZED_FLOAT(Exp),
	CEOP_SIZED_FLOAT(Floor),
	CEOP_SIZED_FLOAT(Log),
	CEOP_SIZED_FLOAT(Log10),	
	CEOP_SIZED_FLOAT(Pow),
	CEOP_SIZED_FLOAT(Round),
	CEOP_SIZED_FLOAT(Sin),
	CEOP_SIZED_FLOAT(Sinh),
	CEOP_SIZED_FLOAT(Sqrt),
	CEOP_SIZED_FLOAT(Tan),
	CEOP_SIZED_FLOAT(Tanh),
	
	CEOP_SIZED_NUMERIC_PLUSF(Cmp_EQ),
	CEOP_SIZED_NUMERIC_PLUSF(Cmp_NE),
	CEOP_SIZED_NUMERIC_PLUSF(Cmp_SLT),
	CEOP_SIZED_NUMERIC(Cmp_ULT),
	CEOP_SIZED_NUMERIC_PLUSF(Cmp_SLE),
	CEOP_SIZED_NUMERIC(Cmp_ULE),
	CEOP_SIZED_NUMERIC_PLUSF(Cmp_SGT),
	CEOP_SIZED_NUMERIC(Cmp_UGT),
	CEOP_SIZED_NUMERIC_PLUSF(Cmp_SGE),
	CEOP_SIZED_NUMERIC(Cmp_UGE),

	CEOP_SIZED_NUMERIC_PLUSF(Neg),
	CeOp_Not_I1,
	CEOP_SIZED_NUMERIC(Not),

	CeOp_COUNT
};

struct CeEmitEntry
{
	int mCodePos;
	int mFile;
	int mLine;
	int mColumn;
};

class CeFunctionInfo
{
public:
	String mName;
	BfMethodInstance* mMethodInstance;
	BfMethodRef mMethodRef;
	CeFunction* mCeFunction;
	int mRefCount;

public:
	CeFunctionInfo()
	{
		mMethodInstance = NULL;
		mCeFunction = NULL;
		mRefCount = 0;
	}

	~CeFunctionInfo();
};

class CeCallEntry
{
public:
	CeFunctionInfo* mFunctionInfo;
	int mBindRevision;
	CeFunction* mFunction;

public:
	CeCallEntry()
	{
		mFunctionInfo = NULL;
		mBindRevision = -1;
		mFunction = NULL;
	}
};

class CeStringEntry
{
public:
	int mStringId;
	int mBindExecuteId;
	addr_ce mStringAddr;

public:
	CeStringEntry()
	{
		mStringId = -1;
		mBindExecuteId = -1;
		mStringAddr = 0;
	}
};

class CeInternalData
{
public:
	enum Kind
	{
		Kind_None,
		Kind_File,
		Kind_FindFileData,
		Kind_Spawn
	};

public:
	Kind mKind;
	bool mReleased;
	int mRefCount;

	union
	{
		BfpFile* mFile;
		BfpFindFileData* mFindFileData;
		BfpSpawn* mSpawn;
	};

	CeInternalData()
	{
		mKind = Kind_None;
		mReleased = false;
		mRefCount = 1;
	}

	void AddRef()
	{
		BfpSystem_InterlockedExchangeAdd32((uint32*)&mRefCount, 1);
	}

	void Release()
	{
		if (BfpSystem_InterlockedExchangeAdd32((uint32*)&mRefCount, (uint32)-1) == 1)
			delete this;
	}

	~CeInternalData();
};

enum CeFunctionKind
{
	CeFunctionKind_NotSet,
	CeFunctionKind_Normal,
	CeFunctionKind_Extern,
	CeFunctionKind_OOB,
	CeFunctionKind_Malloc,
	CeFunctionKind_Free,
	CeFunctionKind_DynCheckFailed,
	CeFunctionKind_FatalError,
	CeFunctionKind_DebugWrite,
	CeFunctionKind_DebugWrite_Int,
	CeFunctionKind_GetReflectType,
	CeFunctionKind_GetReflectTypeById,
	CeFunctionKind_GetReflectTypeByName,
	CeFunctionKind_GetReflectSpecializedType,
	CeFunctionKind_Type_GetCustomAttribute,
	CeFunctionKind_GetMethodCount,
	CeFunctionKind_GetMethod,
	CeFunctionKind_Method_ToString,
	CeFunctionKind_Method_GetName,
	CeFunctionKind_Method_GetInfo,
	CeFunctionKind_Method_GetParamInfo,
	
	CeFunctionKind_EmitTypeBody,
	CeFunctionKind_EmitAddInterface,
	CeFunctionKind_EmitMethodEntry,
	CeFunctionKind_EmitMethodExit,
	CeFunctionKind_EmitMixin,
	
	CeFunctionKind_BfpDirectory_Create,
	CeFunctionKind_BfpDirectory_Rename,
	CeFunctionKind_BfpDirectory_Delete,
	CeFunctionKind_BfpDirectory_GetCurrent,
	CeFunctionKind_BfpDirectory_SetCurrent,
	CeFunctionKind_BfpDirectory_Exists,
	CeFunctionKind_BfpDirectory_GetSysDirectory,

	CeFunctionKind_BfpFile_Close,
	CeFunctionKind_BfpFile_Create,
	CeFunctionKind_BfpFile_Flush,
	CeFunctionKind_BfpFile_GetFileSize,
	CeFunctionKind_BfpFile_Read,
	CeFunctionKind_BfpFile_Release,
	CeFunctionKind_BfpFile_Seek,
	CeFunctionKind_BfpFile_Truncate,
	CeFunctionKind_BfpFile_Write,
	CeFunctionKind_BfpFile_GetTime_LastWrite,
	CeFunctionKind_BfpFile_GetAttributes,
	CeFunctionKind_BfpFile_SetAttributes,
	CeFunctionKind_BfpFile_Copy,
	CeFunctionKind_BfpFile_Rename,
	CeFunctionKind_BfpFile_Delete,
	CeFunctionKind_BfpFile_Exists,
	CeFunctionKind_BfpFile_GetTempPath,
	CeFunctionKind_BfpFile_GetTempFileName,
	CeFunctionKind_BfpFile_GetFullPath,
	CeFunctionKind_BfpFile_GetActualPath,
	
	CeFunctionKind_BfpFindFileData_FindFirstFile,
	CeFunctionKind_BfpFindFileData_FindNextFile,
	CeFunctionKind_BfpFindFileData_GetFileName,
	CeFunctionKind_BfpFindFileData_GetTime_LastWrite,
	CeFunctionKind_BfpFindFileData_GetTime_Created,
	CeFunctionKind_BfpFindFileData_GetTime_Access,
	CeFunctionKind_BfpFindFileData_GetFileAttributes,
	CeFunctionKind_BfpFindFileData_GetFileSize,
	CeFunctionKind_BfpFindFileData_Release,

	CeFunctionKind_BfpSpawn_Create,
	CeFunctionKind_BfpSpawn_GetStdHandles,
	CeFunctionKind_BfpSpawn_Kill,
	CeFunctionKind_BfpSpawn_Release,
	CeFunctionKind_BfpSpawn_WaitFor,

	CeFunctionKind_BfpSystem_GetTimeStamp,
	CeFunctionKind_Sleep,

	CeFunctionKind_Char32_ToLower,
	CeFunctionKind_Char32_ToUpper,
	CeFunctionKind_Char32_IsLower,
	CeFunctionKind_Char32_IsUpper,
	CeFunctionKind_Char32_IsWhiteSpace_EX,
	CeFunctionKind_Char32_IsLetterOrDigit,
	CeFunctionKind_Char32_IsLetter,
	CeFunctionKind_Char32_IsNumber,
	CeFunctionKind_Double_Strtod,
	CeFunctionKind_Double_Ftoa,
	CeFunctionKind_Double_ToString,

	CeFunctionKind_Math_Abs,
	CeFunctionKind_Math_Acos,
	CeFunctionKind_Math_Asin,
	CeFunctionKind_Math_Atan,
	CeFunctionKind_Math_Atan2,
	CeFunctionKind_Math_Ceiling,	
	CeFunctionKind_Math_Cos,
	CeFunctionKind_Math_Cosh,
	CeFunctionKind_Math_Exp,
	CeFunctionKind_Math_Floor,
	CeFunctionKind_Math_Log,
	CeFunctionKind_Math_Log10,
	CeFunctionKind_Math_Mod,
	CeFunctionKind_Math_Pow,
	CeFunctionKind_Math_Round,
	CeFunctionKind_Math_Sin,
	CeFunctionKind_Math_Sinh,
	CeFunctionKind_Math_Sqrt,
	CeFunctionKind_Math_Tan,
	CeFunctionKind_Math_Tanh,	
};

class CeConstStructFixup
{
public:
	enum Kind
	{
		Kind_None,
		Kind_StringPtr,
		Kind_StringCharPtr,
	};

public:
	Kind mKind;
	int mValue;
	int mOffset;
};

class CeConstStructData
{
public:
	Val128 mHash;
	Array<uint8> mData;
	Array<uint8> mFixedData;
	Array<CeConstStructFixup> mFixups;
	addr_ce mAddr;
	int mBindExecuteId;
	bool mQueueFixups;

public:
	CeConstStructData()
	{
		mBindExecuteId = -1;
		mAddr = 0;
		mQueueFixups = false;
	}
};

class CeInnerFunctionInfo
{
public:
	String mName;
	BeFunction* mBeFunction;
	CeFunction* mOwner;
};

class CeStaticFieldEntry
{
public:
	String mName;
	int mTypeId;
	int mSize;
	addr_ce mAddr;
	int mBindExecuteId;

public:
	CeStaticFieldEntry()
	{
		mTypeId = -1;
		mSize = 0;
		mAddr = 0;		
		mBindExecuteId = -1;		
	}
};

class CeFunction
{
public:
	enum InitializeState
	{
		InitializeState_None,
		InitializeState_Initializing,
		InitializeState_Initializing_ReEntry,
		InitializeState_Initialized
	};

public:
	CeMachine* mCeMachine;
	CeFunctionInfo* mCeFunctionInfo;
	CeInnerFunctionInfo* mCeInnerFunctionInfo;
	BfMethodInstance* mMethodInstance;	
	CeFunctionKind mFunctionKind;	
	InitializeState mInitializeState;
	bool mFailed;
	bool mIsVarReturn;
	Array<uint8> mCode;	
	Array<String> mFiles;
	Array<CeEmitEntry> mEmitTable;
	Array<CeCallEntry> mCallTable;
	Array<CeStringEntry> mStringTable;
	Array<CeConstStructData> mConstStructTable;
	Array<CeStaticFieldEntry> mStaticFieldTable;
	Array<BfType*> mTypeTable;
	Array<CeFunction*> mInnerFunctions;
	String mGenError;
	int mFrameSize;	
	int mMaxReturnSize;
	int mId;

public:
	CeFunction()
	{
		mCeMachine = NULL;
		mCeFunctionInfo = NULL;
		mCeInnerFunctionInfo = NULL;
		mFunctionKind = CeFunctionKind_NotSet;		
		mInitializeState = InitializeState_None;
		mMethodInstance = NULL;
		mFailed = false;
		mIsVarReturn = false;
		mFrameSize = 0;
		mMaxReturnSize = 0;
		mId = -1;
	}	

	~CeFunction();
	void Print();
};

enum CeEvalFlags
{
	CeEvalFlags_None = 0,
	CeEvalFlags_Cascade = 1,
	CeEvalFlags_PersistantError = 2,
	CeEvalFlags_DeferIfNotOnlyError = 4,
	CeEvalFlags_NoRebuild = 8
};

enum CeOperandKind
{
	CeOperandKind_None,
	CeOperandKind_FrameOfs,
	CeOperandKind_AllocaAddr,
	CeOperandKind_Block,
	CeOperandKind_Immediate,
	CeOperandKind_ConstStructTableIdx,	
	CeOperandKind_CallTableIdx
};

class CeOperand
{
public:
	CeOperandKind mKind;
	union
	{
		int mFrameOfs;
		int mBlockIdx;
		int mImmediate;
		int mCallTableIdx;
		int mStructTableIdx;
		BeConstant* mConstant;
	};
	BeType* mType;

public:
	CeOperand()
	{
		mKind = CeOperandKind_None;
		mFrameOfs = 0;
		mType = NULL;
	}

	operator bool() const
	{
		return mKind != CeOperandKind_None;
	}

	bool IsImmediate()
	{
		return mKind == CeOperandKind_Immediate;
	}
};

#define BF_CE_DEFAULT_STACK_SIZE 4*1024*1024
#define BF_CE_DEFAULT_HEAP_SIZE 128*1024
#define BF_CE_INITIAL_MEMORY BF_CE_DEFAULT_STACK_SIZE + BF_CE_DEFAULT_HEAP_SIZE
#define BF_CE_MAX_MEMORY 0x7FFFFFFF
#define BF_CE_MAX_CARRYOVER_MEMORY BF_CE_DEFAULT_STACK_SIZE * 2
#define BF_CE_MAX_CARRYOVER_HEAP 1024*1024

enum CeOperandInfoKind
{
	CEOI_None,
	CEOI_FrameRef,
	CEOI_IMM8,
	CEOI_IMM16,
	CEOI_IMM32,
	CEOI_IMM64,
	CEOI_IMMF32,
	CEOI_IMMF64,
	CEOI_IMM_VAR,
	CEOI_JMPREL
};

enum CeSizeClass
{
	CeSizeClass_8,
	CeSizeClass_16,
	CeSizeClass_32,
	CeSizeClass_64,
	CeSizeClass_X,
};

class CeDumpContext
{


public:	
	CeFunction* mCeFunction;
	String mStr;
	uint8* mStart;
	uint8* mPtr;
	uint8* mEnd;

public:
	void DumpOperandInfo(CeOperandInfoKind operandInfoKind);

	void Dump();	
};

struct CePhiOutgoing
{
	BeValue* mPhiValue;
	BePhiInst* mPhiInst;	
	int mPhiBlockIdx;
};

class CeBlock
{
public:
	int mEmitOfs;
	Array<CePhiOutgoing> mPhiOutgoing;

public:
	CeBlock()
	{
		mEmitOfs = -1;		
	}
};

class CeJumpEntry
{
public:
	int mEmitPos;
	int mBlockIdx;
};

class CeBuilder
{
public:	
	CeBuilder* mParentBuilder;
	CeMachine* mCeMachine;	
	CeFunction* mCeFunction;
	BeFunction* mBeFunction;	
	CeOperand mReturnVal;
	BeType* mIntPtrType;
	int mPtrSize;	

	String mError;
	BeDbgLoc* mCurDbgLoc;
	Array<CeBlock> mBlocks;	
	Array<CeJumpEntry> mJumpTable;
	Dictionary<BeValue*, CeOperand> mValueToOperand;
	int mFrameSize;
	Dictionary<BeDbgFile*, int> mDbgFileMap;
	Dictionary<BeFunction*, int> mFunctionMap;
	Dictionary<int, int> mStringMap;
	Dictionary<BeConstant*, int> mConstDataMap;
	Dictionary<BeFunction*, int> mInnerFunctionMap;
	Dictionary<BeGlobalVariable*, int> mStaticFieldMap;
	Dictionary<String, BfFieldInstance*> mStaticFieldInstanceMap;
	
public:
	CeBuilder()
	{
		mParentBuilder = NULL;
		mPtrSize = 0;
		mCeFunction = NULL;
		mBeFunction = NULL;
		mCeMachine = NULL;
		mCurDbgLoc = NULL;
		mFrameSize = 0;
	}
	
	void Fail(const StringImpl& error);

	CeOperand FrameAlloc(BeType* type);
	CeOperand EmitConst(int64 val, int size);
	CeErrorKind EmitConst(Array<uint8>& arr, BeConstant* constant);
	CeOperand GetOperand(BeValue* value, bool allowAlloca = false, bool allowImmediate = false);
	CeSizeClass GetSizeClass(int size);
	int GetCodePos();

	void HandleParams();
	
	void Emit(uint8 val);
	void Emit(CeOp val);
	void EmitSizedOp(CeOp val, int size);
	void Emit(int32 val);
	void Emit(int64 val);
	void Emit(bool val);
	void Emit(void* ptr, int size);
	void EmitZeroes(int size);
	void EmitJump(CeOp op, const CeOperand& block);
	void EmitBinarySwitchSection(BeSwitchInst* switchInst, int startIdx, int endIdx);	

	void EmitFrameOffset(const CeOperand& val);
	void FlushPhi(CeBlock* ceBlock, int targetBlockIdx);

	void EmitBinaryOp(CeOp iOp, CeOp fOp, const CeOperand& lhs, const CeOperand& rhs, CeOperand& result);
	void EmitUnaryOp(CeOp iOp, CeOp fOp, const CeOperand& val, CeOperand& result);
	void EmitSizedOp(CeOp op, const CeOperand& operand, CeOperand* result, bool allowNonStdSize);
	void ProcessMethod(BfMethodInstance* methodInstance, BfMethodInstance* dupMethodInstance);
	void Build();
};

class CeFrame
{
public:
	CeFunction* mFunction;
	addr_ce mStackAddr;
	addr_ce mFrameAddr;
	uint8* mInstPtr;
	BfType* mReturnType;

public:
	CeFrame()
	{
		mFunction = NULL;
		mStackAddr = 0;
		mFrameAddr = 0;
		mInstPtr = NULL;
		mReturnType = NULL;
	}
};

class CeStaticFieldInfo
{
public:	
	addr_ce mAddr;

public:
	CeStaticFieldInfo()
	{
		mAddr = 0;
	}
};

class CeAppendAllocInfo
{
public:
	BfModule* mModule;
	BfIRValue mAllocValue;
	BfIRValue mAppendSizeValue;
};

class CeRebuildKey
{
public:
	enum Kind
	{
		Kind_None,
		Kind_File,
		Kind_Directory
	};

public:
	Kind mKind;
	String mString;

	bool operator==(const CeRebuildKey& other) const
	{
		return (mKind == other.mKind) && (mString == other.mString);
	}
};

class CeRebuildValue
{
public:
	union
	{
		uint64 mInt;
	};
};

class CeEmitContext
{
public:
	BfType* mType;	
	BfMethodInstance* mMethodInstance;
	Array<int32> mInterfaces;	
	String mEmitData;
	String mExitEmitData;
	bool mFailed;

	CeEmitContext()
	{
		mType = NULL;		
		mMethodInstance = NULL;
		mFailed = false;
	}

	bool HasEmissions()
	{
		return !mEmitData.IsEmpty() || !mInterfaces.IsEmpty();
	}
};

class BfCeTypeInfo
{
public:
	Dictionary<int, BfCeTypeEmitEntry> mOnCompileMap;
	Dictionary<int, BfCeTypeEmitEntry> mTypeIFaceMap;
	Array<int> mPendingInterfaces;
	Dictionary<CeRebuildKey, CeRebuildValue> mRebuildMap;
	Val128 mHash;
	bool mFailed;
	BfCeTypeInfo* mNext;

public:
	BfCeTypeInfo()
	{
		mFailed = false;
		mNext = NULL;
	}
};

class CeContext
{
public:
	CeMachine* mCeMachine;
	CeContext* mPrevContext;
	int mReflectTypeIdOffset;
	int mExecuteId;
	CeEvalFlags mCurEvalFlags;

	// These are only valid for the current execution	
	ContiguousHeap* mHeap;
	Array<CeFrame> mCallStack;
	Array<uint8> mMemory;
	int mStackSize;
	Dictionary<int, addr_ce> mStringMap;
	Dictionary<int, addr_ce> mReflectMap;
	Dictionary<Val128, addr_ce> mConstDataMap;	
	HashSet<int> mStaticCtorExecSet;	
	Dictionary<String, CeStaticFieldInfo> mStaticFieldMap;
	Dictionary<int, CeInternalData*> mInternalDataMap;
	int mCurHandleId;

	BfMethodInstance* mCurMethodInstance;
	BfType* mCurExpectingType;
	BfAstNode* mCurTargetSrc;
	BfModule* mCurModule;
	CeFrame* mCurFrame;
	CeEmitContext* mCurEmitContext;	

public:
	CeContext();
	~CeContext();

	BfError* Fail(const StringImpl& error);
	BfError* Fail(const CeFrame& curFrame, const StringImpl& error);

	void FixProjectRelativePath(StringImpl& path);
	bool AddRebuild(const CeRebuildKey& key, const CeRebuildValue& value);
	void AddFileRebuild(const StringImpl& filePath);
	uint8* CeMalloc(int size);
	bool CeFree(addr_ce addr);
	addr_ce CeAllocArray(BfArrayType* arrayType, int count, addr_ce& elemsAddr);
	addr_ce GetReflectType(int typeId);
	addr_ce GetReflectType(const String& typeName);
	int GetTypeIdFromType(addr_ce typeAddr);
	addr_ce GetReflectSpecializedType(addr_ce unspecializedType, addr_ce typeArgsSpanAddr);
	addr_ce GetString(int stringId);
	addr_ce GetString(const StringImpl& str);
	addr_ce GetConstantData(BeConstant* constant);
	BfType* GetBfType(int typeId);
	void PrepareConstStructEntry(CeConstStructData& constStructData);
	bool CheckMemory(addr_ce addr, int32 size);
	bool GetStringFromStringView(addr_ce addr, StringImpl& str);
	bool GetCustomAttribute(BfCustomAttributes* customAttributes, int attributeTypeId, addr_ce resultAddr);	

	bool WriteConstant(BfModule* module, addr_ce addr, BfConstant* constant, BfType* type, bool isParams = false);	
	BfIRValue CreateConstant(BfModule* module, uint8* ptr, BfType* type, BfType** outType = NULL);	
	BfIRValue CreateAttribute(BfAstNode* targetSrc, BfModule* module, BfIRConstHolder* constHolder, BfCustomAttribute* customAttribute);

	bool Execute(CeFunction* startFunction, uint8* startStackPtr, uint8* startFramePtr, BfType*& returnType);
	BfTypedValue Call(BfAstNode* targetSrc, BfModule* module, BfMethodInstance* methodInstance, const BfSizedArray<BfIRValue>& args, CeEvalFlags flags, BfType* expectingType);
};

struct CeTypeInfo
{
	Array<BfMethodInstance*> mMethodInstances;
	int mRevision;
};

class CeMachine
{
public:
	Dictionary<BfMethodInstance*, CeFunctionInfo*> mFunctions;
	Dictionary<String, CeFunctionInfo*> mNamedFunctionMap;
	Dictionary<int, CeFunction*> mFunctionIdMap; // Only used for 32-bit			
	Dictionary<BfType*, CeTypeInfo> mTypeInfoMap;
	HashSet<BfMethodInstance*> mMethodInstanceSet;
	
	Array<CeContext*> mContextList;

	BfCompiler* mCompiler;
	BfModule* mCeModule;
	int mRevision;
	int mMethodBindRevision;
	int mRevisionExecuteTime;	
	int mCurFunctionId;	
	int mExecuteId;
	CeAppendAllocInfo* mAppendAllocInfo;
	
	CeContext* mCurContext;
	CeEmitContext* mCurEmitContext;
	CeBuilder* mCurBuilder;
	CeFunction* mPreparingFunction;		

	BfParser* mTempParser;
	BfReducer* mTempReducer;
	BfPassInstance* mTempPassInstance;

public:
	CeMachine(BfCompiler* compiler);
	~CeMachine();		

	void Init();		
	BeContext* GetBeContext();
	BeModule* GetBeModule();

	void DerefMethodInfo(CeFunctionInfo* ceFunctionInfo);
	void RemoveMethod(BfMethodInstance* methodInstance);					
	void CreateFunction(BfMethodInstance* methodInstance, CeFunction* ceFunction);			
	CeErrorKind WriteConstant(CeConstStructData& data, BeConstant* constVal, CeContext* ceContext);	

	void CheckFunctionKind(CeFunction* ceFunction);
	void PrepareFunction(CeFunction* methodInstance, CeBuilder* parentBuilder);	
	void MapFunctionId(CeFunction* ceFunction);

	void CheckFunctions();
	CeFunction* GetFunction(BfMethodInstance* methodInstance, BfIRValue func, bool& added);
	CeFunction* GetPreparedFunction(BfMethodInstance* methodInstance);
	CeTypeInfo* GetTypeInfo(BfType* type);
	BfMethodInstance* GetMethodInstance(int64 methodHandle);

public:
	void CompileStarted();
	void CompileDone();
	CeFunction* QueueMethod(BfMethodInstance* methodInstance, BfIRValue func);
	void QueueMethod(BfModuleMethodInstance moduleMethodInstance);
	void QueueStaticField(BfFieldInstance* fieldInstance, const StringImpl& mangledFieldName);	

	void SetAppendAllocInfo(BfModule* module, BfIRValue allocValue, BfIRValue appendSizeValue);
	void ClearAppendAllocInfo();

	CeContext* AllocContext();
	void ReleaseContext(CeContext* context);
	BfTypedValue Call(BfAstNode* targetSrc, BfModule* module, BfMethodInstance* methodInstance, const BfSizedArray<BfIRValue>& args, CeEvalFlags flags, BfType* expectingType);
};

NS_BF_END

namespace std
{
	template <>
	struct hash<Beefy::CeRebuildKey>
	{
		size_t operator()(const Beefy::CeRebuildKey& key) const
		{
			return BeefHash<Beefy::String>()(key.mString) ^ (size_t)key.mKind;
		}
	};	
}