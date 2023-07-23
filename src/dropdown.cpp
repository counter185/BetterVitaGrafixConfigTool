#include "all.h"

void Dropdown::Render() {

    std::vector<std::string> newOptions = options;
    if (hasCustomOption){
        newOptions.push_back("Custom");
    }

    int items = 1 + options.size() + (hasCustomOption ? 1 : 0);

    int dropdownItemHeight = DROPDOWN_ITEM_HEIGHT * XM1PW3P1(startTime.getPercent(300));

    SDL_SetRenderDrawColor(rdr,0x0a,0x0a,0x0a,200 * startTime.getPercent(800));
    SDL_RenderFillRect(rdr, NULL);

    SDL_SetRenderDrawColor(rdr,0,0,0,230);
    SDL_Rect rect = {posX, posY, DROPDOWN_WIDTH, items * dropdownItemHeight };
    SDL_RenderFillRect(rdr, &rect);

    rect = {posX, posY + dropdownItemHeight *(selection+1), DROPDOWN_WIDTH, dropdownItemHeight };
    SDL_SetRenderDrawColor(rdr,255,255,255,90);
    SDL_RenderFillRect(rdr, &rect);

    SDL_RenderDrawLine(rdr, posX, posY + dropdownItemHeight, posX + DROPDOWN_WIDTH, posY + dropdownItemHeight);

    for (int x = 0; x < newOptions.size(); x++){
        mainTextRenderer->RenderString(newOptions.at(x), posX + 4, posY + (x+1) * dropdownItemHeight);
    }
}

void Dropdown::TakeInput(SDL_Event evt){
    switch (evt.type){
        case SDL_KEYDOWN:
            switch (evt.key.keysym.sym){
                case SDLK_UP:
                    if (--selection == -1){
                        selection = options.size()-1 + (hasCustomOption ? 1 : 0);
                    }
                    break;
                case SDLK_DOWN:
                    if (++selection == (options.size() + (hasCustomOption ? 1 : 0))){
                        selection = 0;
                    }
                    break;
                case SDLK_SPACE:
                    if (hasCustomOption && selection == options.size()) {
                        //todo: custom input option
                    } else {
                        target->ReceiveDropdownEvent(id, options.at(selection));
                    }
                    break;
                case SDLK_ESCAPE:
                    break;
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            switch (evt.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    if (--selection == -1){
                        selection = options.size()-1 + (hasCustomOption ? 1 : 0);
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    if (++selection == (options.size() + (hasCustomOption ? 1 : 0))){
                        selection = 0;
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    if (hasCustomOption && selection == options.size()) {
                        //todo: custom input option
                    } else {
                        target->ReceiveDropdownEvent(id, options.at(selection));
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    break;
            }
            break;
    }
}
