#pragma once
#include "../../../Globals/Globals.h"
#include "../../../Framework/Memory/Memory.h"
#include "../../../Framework/SimpleMath/SimpleMath.h"
using namespace DirectX::SimpleMath;

// Offset manager
class GameSDK
{
public:
    // Init
    bool InitOffset();

    uintptr_t GetWorld();
    uintptr_t GetCamera();
    uintptr_t GetViewPort();
    uintptr_t GetReplayInterface();

    // address
    uintptr_t m_dwWorld;
    uintptr_t m_dwReplayInterface;
    uintptr_t m_dwViewPort;
    uintptr_t m_dwCamera;

    // pointer
    uintptr_t m_pBoneList = 0x410;
    uintptr_t m_pVehicle = 0xD10;
    uintptr_t m_pInfo = 0x10A8;
    uintptr_t m_pWeaponManager = 0x10B8;

    // offset
    uintptr_t m_pBoneMatrix = 0x60;
    uintptr_t m_vecPosition = 0x90;
    uintptr_t m_bGodMode = 0x189;
    uintptr_t m_flHealth = 0x280;
    uintptr_t m_flHealthMax = 0x284;
    uintptr_t m_vecVelocity = 0x300;
    uintptr_t m_flArmor = 0x150C;
    uintptr_t m_bPedTask = 0x144B;

    // player info
    uintptr_t m_CName = 0xE0; // 0xF0

    // vehicle offset
    uintptr_t m_vecVehicleVelocity = 0;
    uintptr_t m_flVehicleHealth = 0;
    uintptr_t m_flVehicleEngineHealth = 0;

    // weapon
    uintptr_t m_flSpread = 0;
    uintptr_t m_flRecoil = 0x2F4;
};

extern GameSDK* Game;

enum BoneID : int
{
    HEAD,
    LEFTFOOT,
    RIGHTFOOT,
    LEFTANKLE,
    RIGHTANKLE,
    LEFTHAND,
    RIGHTHAND,
    NECK,
    HIP
};

extern struct CBone {
    Vector3 position;
    int junk0;
};

extern struct AllBone {
    CBone bone[9]{};
};

extern bool Vec3_Empty(const Vector3& value);
extern Vector3 Vec3_Transform(Vector3* vIn, Matrix* mIn);
extern Vector3 CalcAngle(Vector3 local_cam, Vector3 to_point);
extern float GetDistance(Vector3 value1, Vector3 value2);
extern bool WorldToScreen(const Matrix& ViewMatrix, const Vector3& vWorld, Vector2& vOut);