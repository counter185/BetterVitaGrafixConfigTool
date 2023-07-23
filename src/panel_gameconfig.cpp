#include "all.h"

void PanelGameConfig::SwitchGame(int indexInMainIO) {
    currentGameIndex = indexInMainIO;
}

const int SPACE_BETWEEN_OPTIONS = 30;

void PanelGameConfig::Render() {
    float animPercent = XM1PW3P1(timerSinceLaunch.getPercent(1000));
    int x = 245 + windowW * (1.0f - animPercent);

    int y = 200 - 200* XM1PW3P1(mainSidebar->lastChange.getPercent(400));

    SDL_Rect panelBGRect = { x, 0, windowW-x, windowH };
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 127);
    SDL_RenderFillRect(rdr, &panelBGRect);

    if (currentGameIndex != -1){
        GameEntry gameEntry = mainIO->gameEntries[currentGameIndex];
        SDL_Texture* iconTex = mainIO->GetIconForAppID(gameEntry.target.titleID);
        SDL_Rect iconDrawRect = {x + 30, y + 30, 128, 128};
        SDL_RenderCopy(rdr, iconTex, NULL, &iconDrawRect);
        mainTextRenderer->RenderString(gameEntry.gameName, x+30+128+10, y + 30);
        mainTextRenderer->RenderString(gameEntry.target.titleID, x+30+128+10, y + 50, SDL_Color{ 255,255,255,127 });
        mainTextRenderer->RenderString(std::string("NID: ") + gameEntry.target.NID, x + 30 + 128 + 10, y + 70, SDL_Color{255,255,255,30});
        mainTextRenderer->RenderString(std::string("Patch target: ") + gameEntry.target.entryPoint, x + 30 + 128 + 10, y + 90, SDL_Color{255,255,255,30});

        int rectDrawPos = selection;
        if (selection > 1) {
            rectDrawPos++;
        }
        if (selection > 5) {
            rectDrawPos++;
        }

        SDL_Rect selectionBGRect = { x + 20, y + 30 + 128 + SPACE_BETWEEN_OPTIONS * (rectDrawPos+1) - 4, 600, SPACE_BETWEEN_OPTIONS + 4 };
        SDL_SetRenderDrawColor(rdr, 0, 0, 0, 127);
        SDL_RenderFillRect(rdr, &selectionBGRect);

        GameConfig& currentConfig = mainIO->GetGameConfigEntry(gameEntry.target.titleID);

        mainTextRenderer->RenderString("Enable VitaGrafix", x + 40, y + 30 + 128 + SPACE_BETWEEN_OPTIONS);
        mainTextRenderer->RenderString(
            currentConfig.enabled == "1" ? "ON" : "OFF", 
            x + 400, y + 30 + 128 + SPACE_BETWEEN_OPTIONS);
        
        mainTextRenderer->RenderString("Display info on start", x + 40, y + 30 + 128 + SPACE_BETWEEN_OPTIONS*2);
        mainTextRenderer->RenderString(
            currentConfig.showOSD == "1" ? "ON" : "OFF", 
            x + 400, y + 30 + 128 + SPACE_BETWEEN_OPTIONS*2);

        SDL_Color drawColor;
        drawColor = gameEntry.hasFBPatch ? SDL_Color{ 255,255,255,255 } : SDL_Color{ 255,255,255,80 };
        mainTextRenderer->RenderString("Framebuffer resolution", x+40, y + 30+128+SPACE_BETWEEN_OPTIONS*4, drawColor);
        mainTextRenderer->RenderString(
            gameEntry.hasFBPatch ? currentConfig.modeFB : "---", 
            x+400, y + 30+128+SPACE_BETWEEN_OPTIONS*4, drawColor);

        drawColor = gameEntry.hasIBPatch ? SDL_Color{ 255,255,255,255 } : SDL_Color{ 255,255,255,80 };
        mainTextRenderer->RenderString("Internal resolution", x+40, y + 30+128+SPACE_BETWEEN_OPTIONS*5, drawColor);
        mainTextRenderer->RenderString(
            gameEntry.hasIBPatch ? currentConfig.modeIB : "---", 
            x+400, y + 30+128+SPACE_BETWEEN_OPTIONS*5, drawColor);

        drawColor = gameEntry.hasFPSPatch ? SDL_Color{ 255,255,255,255 } : SDL_Color{ 255,255,255,80 };
        mainTextRenderer->RenderString("FPS limit", x+40, y + 30+128+SPACE_BETWEEN_OPTIONS*6, drawColor);
        mainTextRenderer->RenderString(
            gameEntry.hasFPSPatch ? currentConfig.modeFPS : "---", 
            x+400, y + 30+128+SPACE_BETWEEN_OPTIONS*6, drawColor);
        
        drawColor = gameEntry.hasMSAAPatch ? SDL_Color{ 255,255,255,255 } : SDL_Color{ 255,255,255,80 };
        mainTextRenderer->RenderString("Multi-sampled Antialiasing", x+40, y + 30+128+SPACE_BETWEEN_OPTIONS*7, drawColor);
        mainTextRenderer->RenderString(
            gameEntry.hasMSAAPatch ? currentConfig.modeMSAA : "---", 
            x+400, y + 30+128+SPACE_BETWEEN_OPTIONS*7, drawColor);
        
        mainTextRenderer->RenderString("Save", x+40, y + 30+128+SPACE_BETWEEN_OPTIONS*9, SDL_Color{255,255,255,255});

        mainTextRenderer->RenderString("Save and launch game", x+40, y + 30+128+SPACE_BETWEEN_OPTIONS*10, SDL_Color{255,255,255,255});

        if (currentDropdown != NULL) {
            currentDropdown->Render();
        }

    } else {
        mainTextRenderer->RenderString("No compatible games found", x+30, y+ 30);
    }

    if (!focusMainPanel) {
        SDL_SetRenderDrawColor(rdr, 0, 0, 0, 200 * timerSinceFocusSwitch.getPercent(300));
        SDL_RenderFillRect(rdr, &panelBGRect);
    }
}

void PanelGameConfig::TakeInput(SDL_Event evt) {
    if (currentDropdown != NULL) {
        currentDropdown->TakeInput(evt);
    } else {
        switch (evt.type){
            case SDL_CONTROLLERBUTTONDOWN:
                switch (evt.cbutton.button){
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        if (--selection == -1) {
                            selection = N_PANELGAMECONFIG_SELECTIONS-1;
                        }
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        if (++selection == N_PANELGAMECONFIG_SELECTIONS) {
                            selection = 0;
                        }
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    case SDL_CONTROLLER_BUTTON_B:
                        SwitchToSidebar();
                        break;
                    case SDL_CONTROLLER_BUTTON_A:
                        InputAccept();
                        break;
                }
                break;
            case SDL_KEYDOWN:
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        if (--selection == -1) {
                            selection = N_PANELGAMECONFIG_SELECTIONS-1;
                        }
                        break;
                    case SDLK_DOWN:
                        if (++selection == N_PANELGAMECONFIG_SELECTIONS) {
                            selection = 0;
                        }
                        break;
                    case SDLK_LEFT:
                    case SDLK_ESCAPE:
                        SwitchToSidebar();
                        break;
                    case SDLK_SPACE:
                        InputAccept();
                        break;
                }
                break;
        }
    }
}

void PanelGameConfig::ReceiveDropdownEvent(int from, std::string data) {
    GameEntry gameEntry = mainIO->gameEntries[currentGameIndex];
    TitleIDString titleID = gameEntry.target.titleID;
    switch (from){
        case 0:
            mainIO->GetGameConfigEntry(titleID).enabled = data == "ON" ? "1" : "0";
            break;
        case 1:
            mainIO->GetGameConfigEntry(titleID).showOSD = data == "ON" ? "1" : "0";
            break;
        case 2:
            mainIO->GetGameConfigEntry(titleID).modeFB = data;
            break;
        case 3:
            mainIO->GetGameConfigEntry(titleID).modeIB = data;
            break;
        case 4:
            mainIO->GetGameConfigEntry(titleID).modeFPS = data;
            break;
        case 5:
            mainIO->GetGameConfigEntry(titleID).modeMSAA = data;
            break;
    }
    delete currentDropdown;
    currentDropdown = NULL;
}

void PanelGameConfig::InputAccept(){
    int x = 245;
    if (currentGameIndex != -1){
        GameEntry gameEntry = mainIO->gameEntries[currentGameIndex];
        switch (selection) {
            case 0:
                currentDropdown = new Dropdown(0, this, {"ON", "OFF"}, x + 400, 30 + 128 + SPACE_BETWEEN_OPTIONS, false);
                break;        
            case 1:
                currentDropdown = new Dropdown(1, this, {"ON", "OFF"}, x + 400, 30 + 128 + SPACE_BETWEEN_OPTIONS*2, false);
                break;
            case 2:
                if (gameEntry.hasFBPatch){
                    currentDropdown = new Dropdown(2, this, {"OFF", "240x136", "480x272", "640x362", "720x408", "960x544"}, x + 400, 30 + 128 + SPACE_BETWEEN_OPTIONS*4, true);
                }
                break;
            case 3:
                if (gameEntry.hasIBPatch) {
                    currentDropdown = new Dropdown(3, this, {"OFF", "240x136", "480x272", "640x362", "720x408", "960x544"}, x + 400, 30 + 128 + SPACE_BETWEEN_OPTIONS*5, true);
                }
                break;
            case 4:
                if (gameEntry.hasFPSPatch) {
                    currentDropdown = new Dropdown(4, this, {"OFF", "30", "60"}, x + 400, 30 + 128 + SPACE_BETWEEN_OPTIONS*6, false);
                }
                break;
            case 5:
                if (gameEntry.hasMSAAPatch) {
                    currentDropdown = new Dropdown(5, this, {"OFF", "2", "4"}, x + 400, 30 + 128 + SPACE_BETWEEN_OPTIONS*7, false);
                }
                break;
            case 6:
                if (mainIO->SaveConfigTXT()) {
                    AddPopup(PopupWindow("Save complete"));
                } else {
                    AddPopup(PopupWindow("Failed to save config.txt"));
                }
                break;
            case 7:
                if (mainIO->SaveConfigTXT()){
                    mainIO->LaunchAppID(gameEntry.target.titleID);
                } else {
                    AddPopup(PopupWindow("Failed to save config.txt"));
                }
                break;
        }
    }
}