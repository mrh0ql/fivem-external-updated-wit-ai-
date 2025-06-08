#pragma once
#include "../Game/GameSDK.h"
#include "../Game/offset.h"

class CPed
{
public:
    uintptr_t address;

    uintptr_t player_info;
    uintptr_t current_weapon;

    float m_flHealth;
    float m_flArmor;
    float m_flMaxHealth;
    Vector3 m_vecPosition;
    Matrix m_bMatrix; // BoneMatrix

    bool Update();
    void UpdateStatic();
    Vector3 GetVelocity();
    Vector3 GetBoneByID(BoneID id);
    std::vector<Vector3> GetBoneList();

    bool IsDead();
    bool IsPlayer();
    bool InVehicle();
};