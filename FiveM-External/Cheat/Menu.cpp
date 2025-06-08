#include "Cheat.h"
#include "../Framework/Overlay/Overlay.h"

int Index = 0;
ImColor RED = ImColor(1.f, 0.f, 0.f, 1.f);
ImColor GREEN = ImColor(0.f, 1.f, 0.f, 1.f);
std::vector<std::string> MenuTitleList = { "AimBot", "Visual", "Misc", "PlayerList" };

void Cheat::RenderMenu()
{
    ImGui::SetNextWindowBgAlpha(0.975f);
    ImGui::SetNextWindowSize(ImVec2(600.f, 450.f));
    ImGui::Begin("FiveM [ EXTERNAL ]", &g.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // SelectButton
    for (int i = 0; i < MenuTitleList.size(); i++)
    {
        static float width = (ImGui::GetContentRegionAvail().x / 4.f) - 6.f;
        if (ImGui::Button(MenuTitleList[i].c_str(), ImVec2(width, 35.f)))
            Index = i;

        if (i != MenuTitleList.size() - 1)
            ImGui::SameLine();
    }

    ImGui::Separator();
    ImGui::Spacing();

    //---// Left //--------------------------------------//
    if (Index != 3)
        ImGui::BeginChild("##LeftChild", ImVec2(ImGui::GetContentRegionAvail().x / 2.f, ImGui::GetContentRegionAvail().y), false);

    switch (Index)
    {
    case 0: // aim
        ImGui::Text("AimBot");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("AimBot", &g.AimBot);
        ImGui::Checkbox("Prediction", &g.AimBotPrediction);
        ImGui::SliderFloat("FOV", &g.AimFov, 100.f, 300.f);
        break;
    case 1: // visual
        ImGui::Text("Visual");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("ESP", &g.ESP);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("ESP Options");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Box", &g.ESP_Box);
        ImGui::Checkbox("Line", &g.ESP_Line);
        ImGui::Checkbox("Skeleton", &g.ESP_Skeleton);
        ImGui::Checkbox("Distance", &g.ESP_Distance);
        ImGui::Checkbox("HealthBar", &g.ESP_HealthBar);
        break;
    case 2: // misc
        ImGui::Text("Player");
        ImGui::Separator();
        ImGui::Checkbox("GodMode", &g.GodMode);

        ImGui::NewLine();
        ImGui::Text("Weapon");
        ImGui::Separator();
        ImGui::Checkbox("NoRecoil", &g.NoRecoil);
        ImGui::Checkbox("NoSpread", &g.NoSpread);
        break;
    case 3:
        ImGui::TextColored(RED, "This is example.\nCustom & Replace it with something you like\nAnd Teleport button is Fake.");

        static char TargetName[64];
        ImGui::InputText("SerchPlayer", TargetName, IM_ARRAYSIZE(TargetName));

        if (ImGui::BeginTable("PlayerListTable", 5, ImGuiTableFlags_Borders))
        {
            // Replace it with something you like
            ImGui::TableNextColumn(); ImGui::Text("Name");
            ImGui::TableNextColumn(); ImGui::Text("Alive");
            ImGui::TableNextColumn(); ImGui::Text("Alive");
            ImGui::TableNextColumn(); ImGui::Text("Position");
            ImGui::TableNextColumn(); ImGui::Text("Teleport");

            for (auto& ped : EntityList)
            {
                // If you use this Online
                //if (!ped.IsPlayer())
                    //continue;

                // ToDo: Replace PlayerName
                std::string pName = "PLAYER";

                if (TargetName != "\0" && pName.find(TargetName) == std::string::npos)
                    continue;

                // Replace it with something you like
                ImGui::TableNextColumn();
                ImGui::Text(pName.c_str());
                ImGui::TableNextColumn();
                ImGui::TextColored(!ped.IsDead() ? GREEN : RED, !ped.IsDead() ? "Yes" : "No");
                ImGui::TableNextColumn();
                ImGui::TextColored(ped.InVehicle() ? GREEN : RED, ped.InVehicle() ? "Yes" : "No");
                ImGui::TableNextColumn();
                ImGui::Text("%.f, %.f, %.f", ped.m_vecPosition.x, ped.m_vecPosition.y, ped.m_vecPosition.z);
                ImGui::TableNextColumn();
                std::string button_string = "Teleport to ##" + pName;

                // Teleport
                ImGui::Button("Teleport");
                /*
                if (ImGui::Button("THIS BUTTON IS FAKE") && !ped.IsDead() && ped.m_pVecLocation.z > -30)
                    local.set_position(ped.BoneList[HEAD] + Vector3(0.f, 0.f, 0.5f));
                */
            }

            ImGui::EndTable();
        }
        break;
    default:
        break;
    }

    if (Index != 3)
        ImGui::EndChild();
    //---------------------------------------------------//
    ImGui::SameLine();
    //---// Right //--------------------------------------//
    if (Index != 3)
        ImGui::BeginChild("##RightChild", ImVec2(ImGui::GetContentRegionAvail()), false);

    switch (Index)
    {
    case 0: // aim
        
        break;
    case 1: // visual
        ImGui::Text("ESP Setting");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SliderFloat("Distance", &g.ESP_MaxDistance, 50.f, 1000.f);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("  ESP Colors");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::ColorEdit4("NPC", &ESP_NPC.Value.x);
        ImGui::ColorEdit4("Player", &ESP_Player.Value.x);
        ImGui::ColorEdit4("Skeleton", &ESP_Skeleton.Value.x);
        break;
    case 2: // misc
        ImGui::Text("System");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("StreamProof", &g.StreamProof);
        ImGui::Checkbox("Crosshair", &g.Crosshair);

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::NewLine();

        // Exit
        if (ImGui::Button("Exit", ImVec2(ImGui::GetContentRegionAvail().x, 30.f)))
            g.process_active = false;
        break;
    default:
        break;
    }

    if (Index != 3)
        ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::End();
}