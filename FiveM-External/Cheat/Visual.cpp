#include "Cheat.h"

void Cheat::RenderInfo()
{
    // ウォーターマーク的な？
    std::string text = "FiveM | " + std::to_string((int)ImGui::GetIO().Framerate) + "FPS";
    String(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), text.c_str());

    // クロスヘア
    if (g.Crosshair)
    {
        ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), 3, ImColor(0.f, 0.f, 0.f, 1.f));
        ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), 2, ImColor(1.f, 1.f, 1.f, 1.f));
    }

    // AimFov
    if (g.AimBot)
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom / 2.f), g.AimFov, ImColor(1.f, 1.f, 1.f, 1.f));
}

void Cheat::RenderESP()
{
    if (!pLocal->Update())
        return;

    CPed target;
    float MinFov = FLT_MAX;
    Vector2 Center = Vector2(g.GameRect.right / 2.f, g.GameRect.bottom / 2.f);
    Matrix ViewMatrix = m.Read<Matrix>(Game->GetViewPort() + 0x24C);

    for (auto& ped : EntityList)
    {
        CPed* pEntity = &ped;

        if (!pEntity->Update())
            continue;

        float pDistance = GetDistance(pEntity->m_vecPosition, pLocal->m_vecPosition);

        if (pDistance >= g.ESP_MaxDistance)
            continue;

        std::vector<Vector3> BoneList = pEntity->GetBoneList();

        if (BoneList.size() != 9)
            continue;

        // WorldToScreen
        Vector2 pBase{}, pHead{}, pNeck{}, pLeftFoot{}, pRightFoot{};
        if (!WorldToScreen(ViewMatrix, pEntity->m_vecPosition, pBase) || 
            !WorldToScreen(ViewMatrix, BoneList[HEAD], pHead) || 
            !WorldToScreen(ViewMatrix, BoneList[NECK], pNeck) || 
            !WorldToScreen(ViewMatrix, BoneList[LEFTFOOT], pLeftFoot) || 
            !WorldToScreen(ViewMatrix, BoneList[RIGHTFOOT], pRightFoot))
            continue;

        // ESP用にいくつかサイズを出しておく
        float HeadToNeck = pNeck.y - pHead.y;
        float pTop = pHead.y - (HeadToNeck * 2.5f);
        float pBottom = (pLeftFoot.y > pRightFoot.y ? pLeftFoot.y : pRightFoot.y) * 1.001f;
        float pHeight = pBottom - pTop;
        float pWidth = pHeight / 3.5f;
        ImColor color = pEntity->IsPlayer() ? ESP_Player : ESP_NPC;

        // Line
        if (g.ESP_Line)
            DrawLine(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom), ImVec2(pBase.x, pBottom), color, 1.f);

        // Box
        if (g.ESP_Box)
        {
            DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x + pWidth, pTop), color, 1.f);
            DrawLine(ImVec2(pBase.x - pWidth, pTop), ImVec2(pBase.x - pWidth, pBottom), color, 1.f);
            DrawLine(ImVec2(pBase.x + pWidth, pTop), ImVec2(pBase.x + pWidth, pBottom), color, 1.f);
            DrawLine(ImVec2(pBase.x - pWidth, pBottom), ImVec2(pBase.x + pWidth, pBottom), color, 1.f);
        }

        // Skeleton
        if (g.ESP_Skeleton)
        {
            // Body
            for (int j = 0; j < 5; j++)
            {
                Vector3 skList[][2] = { 
                    { BoneList[NECK], BoneList[HIP] }, 
                    { BoneList[NECK], BoneList[LEFTHAND] },
                    { BoneList[NECK], BoneList[RIGHTHAND] }, 
                    { BoneList[HIP],  BoneList[LEFTFOOT] }, 
                    { BoneList[HIP],  BoneList[RIGHTFOOT] }
                };

                Vector2 ScreenB1{}, ScreenB2{};
                if (Vec3_Empty(skList[j][0]) || Vec3_Empty(skList[j][1]))
                    break;
                else if (!WorldToScreen(ViewMatrix, skList[j][0], ScreenB1) || !WorldToScreen(ViewMatrix, skList[j][1], ScreenB2))
                    break;

                DrawLine(ImVec2(ScreenB1.x, ScreenB1.y), ImVec2(ScreenB2.x, ScreenB2.y), ESP_Skeleton, 1.f);
            }
        }

        // Healthbar
        if (g.ESP_HealthBar)
        {
            HealthBar((pBase.x - pWidth) - 4.f, pBottom, 2.f, -pHeight, pEntity->m_flHealth, pEntity->m_flMaxHealth);

            if (pEntity->m_flArmor > 0.f)
                ArmorBar((pBase.x + pWidth) + 3.f, pBottom, 2.f, -pHeight, pEntity->m_flArmor, 100);
        }

        // Distance
        if (g.ESP_Distance)
        {
            std::string dist = std::to_string((int)pDistance) + "m";
            String(ImVec2(pBase.x - ImGui::CalcTextSize(dist.c_str()).x / 2.f, pBottom), ImColor(1.f, 1.f, 1.f, 1.f), dist.c_str());
        }

        float FOV = abs((Center - pBase).Length());

        if (FOV < g.AimFov)
        {
            if (FOV < MinFov)
            {
                target = ped;
                MinFov = FOV;
                continue;
            }
        }
    }

    if (target.address != NULL) {
        AimBot(target);
    }
}