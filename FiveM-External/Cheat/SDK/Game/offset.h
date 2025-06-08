#pragma once

// b3258
namespace offset
{
    // Base
    constexpr auto ViewPort = 0x201DBA0;
    constexpr auto Camera = 0x201E7D0;

    // Pointer
    constexpr auto m_pBoneList = 0x410;
    constexpr auto m_pInfo = 0x10A8;          // PlayerInfo
    constexpr auto m_pWeaponManager = 0x10B8; // WeaponManager

    // Ped
    constexpr auto m_bMatrix = 0x60;
    constexpr auto m_vecPosition = 0x90;
    constexpr auto m_bGodMode = 0x189;
    constexpr auto m_flHealth = 0x280;
    constexpr auto m_flHealthMax = 0x284;
    constexpr auto m_vecVelocity = 0x300;
    constexpr auto m_bPedTask = 0x144B;
    constexpr auto m_flArmor = 0x150C;

    // CurrentWeapon (WepaonManager + 0x20)
    constexpr auto m_flSpread = 0x74;
    constexpr auto m_flRecoil = 0x2F4;
}