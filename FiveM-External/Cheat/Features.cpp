#include "Cheat.h"

const int ReadCount = 256;
Vector3 GetPrediction(CPed& target, CPed& local);

struct Entity {
    uint64_t address;
    uint64_t junk0;
};

struct EntityList_t {
    Entity entity[ReadCount];
};

void Cheat::UpdateList()
{
    while (g.process_active)
    {
        std::vector<CPed> list;
        local.address = m.Read<uintptr_t>(Game->GetWorld() + 0x8);

        // Read EntityList
        uintptr_t pEntityList = m.ReadChain(Game->GetReplayInterface(), { 0x18, 0x100 });
        EntityList_t entitylist = m.Read<EntityList_t>(pEntityList);

        for (int i = 0; i < 256; i++)
        {
            if (entitylist.entity[i].address == NULL || entitylist.entity[i].address == local.address)
                continue;

            CPed p = CPed();
            p.address = entitylist.entity[i].address;
            p.UpdateStatic();

            list.push_back(p);
        }

        this->EntityList = list;
        Sleep(500);
    }
}

void Cheat::Misc()
{
    // ToDo : Rewrite

    /*
    if (g.GodMode && m.Read<bool>(pLocal->address + offset::m_bGodMode) == false)
        m.Write<bool>(pLocal->address + offset::m_bGodMode, true);
    else if (!g.GodMode && m.Read<bool>(pLocal->address + offset::m_bGodMode) == true)
        m.Write<bool>(pLocal->address + offset::m_bGodMode, false);

    if (g.NoRecoil && m.Read<float>(pLocal->current_weapon + offset::m_flRecoil) != 0.f)
        m.Write<float>(pLocal->current_weapon + offset::m_flRecoil, 0.f);

    if (g.NoSpread && m.Read<float>(pLocal->current_weapon + offset::m_flSpread) != 0.f)
        m.Write<float>(pLocal->current_weapon + offset::m_flSpread, 0.f);
        */
}

void Cheat::AimBot(CPed target)
{
    if (IsKeyDown(g.AimKey0) || IsKeyDown(g.AimKey1))
    {
        if (Vec3_Empty(target.GetBoneByID(NECK)))
            return;

        uintptr_t camera = Game->GetCamera();
        Vector3 ViewAngle = m.Read<Vector3>(camera + 0x3D0); // and 0x40
        Vector3 CameraPos = m.Read<Vector3>(camera + 0x60);
        Vector3 predict = g.AimBotPrediction ? GetPrediction(target, local) : Vector3();
        Vector3 targetPos = target.GetBoneByID(NECK) + predict;
        Vector3 Angle = CalcAngle(CameraPos, targetPos);
        Vector3 Delta = Angle - ViewAngle;
        Vector3 WriteAngle = ViewAngle + Delta;

        if (!Vec3_Empty(WriteAngle)) {
            m.Write<Vector3>(camera + 0x3D0, WriteAngle);
        }
    }
}

// As a concept.
// Some improvements are needed.
Vector3 GetPrediction(CPed& target, CPed& local)
{
    Vector3 vOut{};

    // ToDo
    Vector3 m_vecVelocity = target.GetVelocity(); // InVehicle() ? GetVehicleVelocity() : GetVelocity();

    if (Vec3_Empty(m_vecVelocity))
        return Vector3();

    float distance = GetDistance(target.m_vecPosition, local.m_vecPosition);
    float BulletSpeed = m.Read<float>(local.current_weapon + 0xE0);

    // ToDo
    if (BulletSpeed == 0.f)
        return Vector3();
    else if (BulletSpeed < 100.f)
        BulletSpeed = 1800.f; // temp

    // ToDo
    float time = distance / BulletSpeed;
    vOut = m_vecVelocity * time;

    return vOut;
}