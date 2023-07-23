#include "all.h"

const int ICON_SIZE_WH = 64;
const int SCROLL_SPEED = 5;

void Sidebar::Render(){

    float animPercent = XM1PW3P1(timerSinceLaunch.getPercent(1000));
    int x = -300 + 300*animPercent;

    int width = 240;

    SDL_Rect sideBarBGRect = { x,0,width, windowH };
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 127);
    SDL_RenderFillRect(rdr, &sideBarBGRect);

    if (selection != -1) {

        int y = (int)scrollY;

        SDL_Rect rectSelection = { x, y + (ICON_SIZE_WH * (selection + 1)), width, ICON_SIZE_WH };
        SDL_SetRenderDrawColor(rdr, 255, 255, 255, 90);
        SDL_RenderFillRect(rdr, &rectSelection);

        int endPointY = rectSelection.y + rectSelection.h;
        int startPointY = rectSelection.y;
        int topYPoint = ICON_SIZE_WH;
        int bottomYPoint = windowH - ICON_SIZE_WH - ICON_SIZE_WH / 2;
        if (endPointY > bottomYPoint) {
            // let's just assume 60 fps here
            scrollY -= (endPointY - bottomYPoint) * 0.016f * SCROLL_SPEED;
        }
        if (topYPoint > startPointY) {
            scrollY += (topYPoint - startPointY) * 0.016f * SCROLL_SPEED;
        }
        y = (int)scrollY;

        mainTextRenderer->RenderString("VitaGrafix Configuration", x + 3, y + 2);

        SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
        SDL_RenderDrawLine(rdr, x, y + 63, x + width, y + 63);
        y += 64;

        for (auto& a : mainIO->compatibleInstalledGames) {
            GameEntry gameEntry = mainIO->gameEntries[a];
            SDL_Texture* iconTex = mainIO->GetIconForAppID(gameEntry.target.titleID);
            SDL_Rect drawRect = { x, y, ICON_SIZE_WH, ICON_SIZE_WH };
            if (iconTex != NULL) {
                SDL_RenderCopy(rdr, iconTex, NULL, &drawRect);
            }

            std::string gameName = gameEntry.gameName;
            // lazy way of doing this lmao
            if (gameName.size() > 20) {
                gameName = gameName.substr(0, 17) + "...";
            }
            mainTextRenderer->RenderString(gameName, x + ICON_SIZE_WH + 3, y + 2);
            mainTextRenderer->RenderString(gameEntry.target.titleID, x + ICON_SIZE_WH + 3, y + 2 + 20, SDL_Color{ 255,255,255,100 });

            SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
            SDL_RenderDrawLine(rdr, x, y + 63, x + width, y + 63);
            y += 64;
            if (y > windowH) {
                break;
            }
        }
    }

    if (focusMainPanel) {
        SDL_SetRenderDrawColor(rdr, x, 0, 0, 200 * timerSinceFocusSwitch.getPercent(300));
        SDL_RenderFillRect(rdr, &sideBarBGRect);
    }
}

void Sidebar::PopulateSidebar(){
    
}

void Sidebar::TakeInput(SDL_Event evt) {
    switch (evt.type) {
        case SDL_CONTROLLERBUTTONDOWN:
            switch (evt.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    lastChange.start();
                    if (mainIO->compatibleInstalledGames.size() > 0) {
                        selection++;
                        selection %= mainIO->compatibleInstalledGames.size();
                        mainPanel->SwitchGame(mainIO->compatibleInstalledGames[selection]);
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    lastChange.start();
                    if (mainIO->compatibleInstalledGames.size() > 0) {
                        selection--;
                        if (selection == -1) {
                            selection = mainIO->compatibleInstalledGames.size() - 1;
                        }
                        mainPanel->SwitchGame(mainIO->compatibleInstalledGames[selection]);
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                case SDL_CONTROLLER_BUTTON_A:
                    if (mainIO->compatibleInstalledGames.size() > 0) {
                        SwitchToMainPanel();
                    }
                    break;
            }
            break;
        case SDL_KEYDOWN:
            switch (evt.key.keysym.sym) {
                case SDLK_DOWN:
                    lastChange.start();
                    if (mainIO->compatibleInstalledGames.size() > 0) {
                        selection++;
                        selection %= mainIO->compatibleInstalledGames.size();
                        mainPanel->SwitchGame(mainIO->compatibleInstalledGames[selection]);
                    }
                    break;
                case SDLK_UP:
                    lastChange.start();
                    if (mainIO->compatibleInstalledGames.size() > 0) {
                        selection--;
                        if (selection == -1) {
                            selection = mainIO->compatibleInstalledGames.size() - 1;
                        }
                        mainPanel->SwitchGame(mainIO->compatibleInstalledGames[selection]);
                    }
                    break;
                case SDLK_RIGHT:
                case SDLK_SPACE:
                    if (mainIO->compatibleInstalledGames.size() > 0) {
                        SwitchToMainPanel();
                    }
                    break;
            }
            break;
    }
}