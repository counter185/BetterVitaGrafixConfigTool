#include "all.h"

void PopupWindow::Render() {
    float animPercent = XM1PW3P1(startTimer.getPercent(300));
    int y = (windowH - windowH * animPercent);
    SDL_Rect drawRect = {windowW/6, y + windowH/6, windowW / 6 * 4, windowH / 6 * 4};
    SDL_SetRenderDrawColor(rdr, 0,0,0,250);
    SDL_RenderFillRect(rdr, &drawRect);
    mainTextRenderer->RenderString(text, windowW/6 + 20, y + windowH/2 - 9);
}

void PopupWindow::TakeInput(SDL_Event evt) {
    if (isCancellable){
        switch (evt.type){
            case SDL_CONTROLLERBUTTONDOWN:
                PopupCancel();
                break;
            case SDL_KEYDOWN:
                PopupCancel();
                break;
        }
    }
}