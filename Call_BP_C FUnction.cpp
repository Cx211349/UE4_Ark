#include<iosstream>
#include<string>
/*When playing reverse games,
if there are some functions that cannot be analyzed or the correct function address cannot be analyzed, 
you can use the internal functions of the game to search and call it through ProcessEvent
在进行游戏逆向时，有一些无法被分析出的函数，或许它们是游戏自定义BP类下的函数，我们就可以利用此方法调用它*/

/*Fix FName
修补FName结构
*/
struct FName
{
	int ComparisonIndex;
	int Number;
}; 

/*Fix UFunction
修补UFunction结构
You can also use void * instead of it
也可以用void*代替它*/
struct UObject
{

};
class UFunction public:UObject
{
public:

};
/*UFunction is a C++function recognized by the Unreal Engine 4 (UE4) reflection system. 
UOObject or blueprint function libraries can declare member functions as UFunctions by placing the UFUNCTION macro
in the line above the function declaration in the header file.
Macros will support function descriptors to change the way UE4 interprets and uses functions.
UFunction 是虚幻引擎4（UE4）反射系统可识别的C++函数。
UObject 或蓝图函数库可将成员函数声明为UFunction，
方法是将 UFUNCTION 宏放在头文件中函数声明上方的行中。
宏将支持 函数说明符 更改UE4解译和使用函数的方式。*/

				

/*void __fastcall FName::Init(FName *this,const wchar_t *InName, int InNumber, EFindName FindType, bool bSplitName, int HardcodeIndex)*/
/*定义查找FName的函数*/
typedef void(*FNameInit_FN)(FName* result, const wchar_t* InName, int InNumber, uint8_t FindType, bool bSplitName, int HardcodeIndex);


/*UFunction *__fastcall UObject::FindFunction(UObject *this, FName InName, EIncludeSuperFlag::Type a3)*/
/*定义查找UFunction的函数*/
typedef UFunction*(*FindFunction_FN)(void*Object, FName InName, uint8_t a3);


/*void __fastcall UObject::ProcessEvent(UObject *this, UFunction *Function, void *Parms)*/
/*定义ProcessEvent*/
typedef void(*ProcessEvent_FN)(void*Object,UFunction*Function,void* Parms);


/*a simple example of calling functions under the game's custom blueprint class
ASpaceDolphin_Character_BP_C > ADino_Character_BP_C > APrimalDinoCharacter > APrimalCharacter > ACharacter > APawn > AActor > UObject
namespace ASpaceDolphin_Character_BP_C
		{
		void ServerRequestFireLasers(FVector CameraHitLoc, float CustomTimeDilation) offset:0x7ffffffffff;
		}
*/
/*Define the structure required for parameters
定义参数所需的结构*/
struct Vector3
{
	float X, Y, Z;
};
/*Params
参数*/
struct FireLasersParams
{
	Vector3 CameraHitLoc;//相机撞击位置Camera impact position
	float CustomTimeDilation;//持续周期CustomTimeDilation
};
int main()
{
	/*Determine its location in memory*/
	FNameInit_FN FNameInit = 0x12345678;
	FindFunction_FN FindFunction = 0x23456789;
	ProcessEvent_FN ProcessEvent = 0x34567890;
	/*storage*/
	FName FunctionFName; 
	UFunction *Function;
	/*Find  FName corresponding wchar_t*
	利用宽字节查找FName*/
	FNameInit(&FunctionFName,L"ServerRequestFireLasers",0,1,1,-1);
	Function=FindFunction(SpaceDolphin_Character_BP_C,FunctionFName,NULL);
	/*Avoiding crashes caused by NULL results
	避免返回空导致崩溃*/
	if (Function)
	{
		FireLasersParams Params;
		Params.CameraHitLoc = {0.0f,0.0f,0.0f};
		Params.CustomTimeDilation = 1.0f;
		ProcessEvent(SpaceDolphin_Character_BP_C,Function,&Params);
		/*If you encounter a crash problem, simply hook  processevent to execute it internally
		如果你遇到任何崩溃问题，仅仅需要Hook ProcessEvent让其在内部执行*/
	}
	return 1;
}
/*Examples of using it internally in the game 
游戏内部使用它的例子*/

/*
void __fastcall AShooterCharacter::ServerFireBallistaProjectile(
        AShooterCharacter *this,
        FVector *Origin,
        FVector_NetQuantizeNormal *ShootDir)
{
  UFunction *Function; // rax
  char v4[12]; // [rsp+28h] [rbp-30h] BYREF
  char v5[12]; // [rsp+34h] [rbp-24h] BYREF

  qmemcpy(v4, Origin, sizeof(v4));
  qmemcpy(v5, ShootDir, sizeof(v5));
  Function = UObject::FindFunction(this, SHOOTERGAME_ServerFireBallistaProjectile, (EIncludeSuperFlag::Type)ShootDir);
  this->ProcessEvent(this, Function, v4);
}


FHitResult *__fastcall AShooterWeapon::WeaponTrace(
        AShooterWeapon *this,
        FHitResult *result,
        const FVector *StartTrace,
        const FVector *EndTrace)
{
  UWorld *v4; // rax
  __int128 v5; // xmm0
  __int128 v6; // xmm0
  __int128 v7; // xmm0
  __int128 v8; // xmm0
  __int128 v9; // xmm0
  FRotator *v10; // rax
  __int128 v11; // xmm0
  __int128 v12; // xmm0
  __int128 v13; // xmm0
  __int128 v14; // xmm0
  __int128 v15; // xmm0
  UWorld *v16; // rax
  UWorld *v17; // rax
  __int64 TraceChannel; // [rsp+20h] [rbp-618h]
  FCollisionQueryParams *Params; // [rsp+28h] [rbp-610h]
  int v21; // [rsp+50h] [rbp-5E8h]
  int ObjectIndex; // [rsp+54h] [rbp-5E4h]
  FVector *v23; // [rsp+58h] [rbp-5E0h]
  int v24[3]; // [rsp+60h] [rbp-5D8h] BYREF
  float v25; // [rsp+6Ch] [rbp-5CCh]
  AActor **v26; // [rsp+70h] [rbp-5C8h]
  int v27; // [rsp+78h] [rbp-5C0h]
  __int64 *v28; // [rsp+80h] [rbp-5B8h]
  int v29; // [rsp+88h] [rbp-5B0h]
  int v30; // [rsp+8Ch] [rbp-5ACh]
  float v31; // [rsp+90h] [rbp-5A8h]
  int v32; // [rsp+94h] [rbp-5A4h]
  FChunkedFixedUObjectArray *p_ObjObjects; // [rsp+98h] [rbp-5A0h]
  FChunkedFixedUObjectArray *v34; // [rsp+A0h] [rbp-598h]
  USceneComponent *RootComponent; // [rsp+B0h] [rbp-588h]
  BOOL v37; // [rsp+B8h] [rbp-580h]
  BOOL v39; // [rsp+C0h] [rbp-578h]
  int v40[3]; // [rsp+C4h] [rbp-574h] BYREF
  int v41[6]; // [rsp+D0h] [rbp-568h] BYREF
  int NumElements; // [rsp+E8h] [rbp-550h]
  float v43; // [rsp+F0h] [rbp-548h]
  int v44; // [rsp+F8h] [rbp-540h]
  float v45; // [rsp+100h] [rbp-538h]
  int v46; // [rsp+108h] [rbp-530h]
  __int128 v47; // [rsp+110h] [rbp-528h]
  EObjectFlags ObjectFlags; // [rsp+120h] [rbp-518h]
  float v49; // [rsp+128h] [rbp-510h]
  int v50; // [rsp+130h] [rbp-508h]
  float v51; // [rsp+138h] [rbp-500h]
  int v52; // [rsp+140h] [rbp-4F8h]
  float v53; // [rsp+148h] [rbp-4F0h]
  __int64 *v54; // [rsp+150h] [rbp-4E8h]
  float v55; // [rsp+158h] [rbp-4E0h]
  AShooterCharacter *MyPawn; // [rsp+160h] [rbp-4D8h]
  float v57; // [rsp+168h] [rbp-4D0h]
  int v58; // [rsp+170h] [rbp-4C8h]
  float v59; // [rsp+178h] [rbp-4C0h]
  int v60; // [rsp+180h] [rbp-4B8h]
  float v61; // [rsp+188h] [rbp-4B0h]
  float v62; // [rsp+190h] [rbp-4A8h]
  float v63; // [rsp+198h] [rbp-4A0h]
  float v64; // [rsp+1A0h] [rbp-498h]
  int v65; // [rsp+1A8h] [rbp-490h]
  float v66; // [rsp+1ACh] [rbp-48Ch]
  float v67; // [rsp+1B0h] [rbp-488h]
  float v68; // [rsp+1B4h] [rbp-484h]
  float v69; // [rsp+1B8h] [rbp-480h]
  float v70; // [rsp+1C0h] [rbp-478h]
  __int128 v71; // [rsp+1D0h] [rbp-468h]
  int v72; // [rsp+1E0h] [rbp-458h]
  int v73[3]; // [rsp+1E8h] [rbp-450h] BYREF
  FVector v74; // [rsp+1F8h] [rbp-440h] BYREF
  AShooterCharacter *v75; // [rsp+208h] [rbp-430h]
  FUObjectItem *v76; // [rsp+210h] [rbp-428h]
  __int64 *v77; // [rsp+218h] [rbp-420h]
  __int64 *v78; // [rsp+220h] [rbp-418h]
  AActor *v79; // [rsp+228h] [rbp-410h]
  AActor *v80; // [rsp+230h] [rbp-408h]
  AActor *InIgnoreActor; // [rsp+238h] [rbp-400h]
  char *v82; // [rsp+240h] [rbp-3F8h]
  FWeakObjectPtr *p_InvocationList; // [rsp+248h] [rbp-3F0h]
  FUObjectItem *v84; // [rsp+250h] [rbp-3E8h]
  char *v85; // [rsp+258h] [rbp-3E0h]
  AActor **v86; // [rsp+260h] [rbp-3D8h]
  AActor *v87; // [rsp+268h] [rbp-3D0h]
  AActor **v88; // [rsp+270h] [rbp-3C8h]
  AActor *v89; // [rsp+278h] [rbp-3C0h]
  FWeakObjectPtr *p_RidingDino; // [rsp+280h] [rbp-3B8h]
  FVector v91; // [rsp+288h] [rbp-3B0h] BYREF
  int v92[3]; // [rsp+294h] [rbp-3A4h] BYREF
  FVector v93; // [rsp+2A0h] [rbp-398h] BYREF
  __int128 v94; // [rsp+2B0h] [rbp-388h]
  char *v95; // [rsp+2C0h] [rbp-378h]
  __int128 v96; // [rsp+2D0h] [rbp-368h]
  __int128 v97; // [rsp+2E0h] [rbp-358h]
  __int128 v98; // [rsp+2F0h] [rbp-348h]
  char v99[12]; // [rsp+300h] [rbp-338h] BYREF
  char v100[12]; // [rsp+30Ch] [rbp-32Ch] BYREF
  __int64 v101[3]; // [rsp+318h] [rbp-320h] BYREF
  __int128 v102; // [rsp+330h] [rbp-308h]
  __int128 v103; // [rsp+340h] [rbp-2F8h]
  __int128 v104; // [rsp+350h] [rbp-2E8h]
  __int128 v105; // [rsp+360h] [rbp-2D8h]
  __int128 v106; // [rsp+370h] [rbp-2C8h]
  __int128 v107; // [rsp+380h] [rbp-2B8h]
  __int128 v108; // [rsp+390h] [rbp-2A8h]
  __int128 v109; // [rsp+3A0h] [rbp-298h]
  __int128 v110; // [rsp+3B0h] [rbp-288h]
  __int128 v111; // [rsp+3C0h] [rbp-278h]
  __int128 v112; // [rsp+3D0h] [rbp-268h]
  __int128 v113; // [rsp+3E0h] [rbp-258h]
  __int128 v114; // [rsp+3F0h] [rbp-248h]
  __int128 v115; // [rsp+400h] [rbp-238h]
  __int128 v116; // [rsp+410h] [rbp-228h]
  __int128 v117; // [rsp+420h] [rbp-218h]
  __int128 v118; // [rsp+430h] [rbp-208h]
  __int128 v119; // [rsp+440h] [rbp-1F8h]
  FVector resulta; // [rsp+450h] [rbp-1E8h] BYREF
  char v121[20]; // [rsp+45Ch] [rbp-1DCh] BYREF
  FPenetrationTraceHit v122; // [rsp+470h] [rbp-1C8h] BYREF
  FPenetrationTraceHit v123; // [rsp+500h] [rbp-138h] BYREF
  __m128 Translation; // [rsp+590h] [rbp-A8h]
  FCollisionQueryParams v125; // [rsp+5A0h] [rbp-98h] BYREF

  if ( (dword_144A28F68 & 1) == 0 )
  {
    dword_144A28F68 |= 1u;
    FName::FName(&WeaponFireTag_1, L"WeaponTrace", FNAME_Add, 1);
  }
  FCollisionQueryParams::FCollisionQueryParams(&v125, WeaponFireTag_1, 0, this->Instigator);
  v125.bReturnPhysicalMaterial = 1;
  if ( this->Instigator
    && this->Instigator->IsShooterCharacter(this->Instigator)
    && APrimalCharacter::CharacterIsCarriedAsPassenger((AShooterCharacter *)this->Instigator)
    && ((__int64)this->Instigator[4].Controller & 0x10) != 0 )
  
*/
