#include "all.h"

SDL_Renderer* rdr = NULL;
SDL_Window* wdw = NULL;
bool quit = false;

int windowW = 960;
int windowH = 544;

IO* mainIO = NULL;
TextRenderer* mainTextRenderer = NULL;
Sidebar* mainSidebar = NULL;
PanelGameConfig* mainPanel = NULL;
FILE* logOutput = stdout;
bool focusMainPanel = false;
Timer64 timerSinceLaunch;
Timer64 timerSinceFocusSwitch;

std::vector<PopupWindow> popups;

void SwitchToMainPanel() {
    focusMainPanel = true;
    timerSinceFocusSwitch.start();
}
void SwitchToSidebar() {
    focusMainPanel = false;
    timerSinceFocusSwitch.start();
}

void PopupCancel() {
    popups.pop_back();
}

void AddPopup(PopupWindow target) {
    popups.push_back(target);
}

void DrawBackground() {
    SDL_Vertex vtxs[4];

    vtxs[0].position = SDL_FPoint(0, 0);
    vtxs[0].color = SDL_Color{ 0x00, 0x04, 0x6e, 0xff };
    
    vtxs[1].position = SDL_FPoint(windowW, windowH);
    vtxs[1].color = SDL_Color{ 0x17, 0x00, 0x40, 0xff };

    vtxs[2].position = SDL_FPoint(windowW, 0);
    vtxs[2].color = SDL_Color{ 0x0d, 0x00, 0x4a, 0xff };

    vtxs[3].position = SDL_FPoint(0, windowH);
    vtxs[3].color = SDL_Color{ 0x0d, 0x00, 0x4a, 0xff };

    int idx[6] = { 0,2,1, 0,3,1 };

    SDL_RenderGeometry(rdr, NULL, vtxs, 4, idx, 6);
}

int main( int argc, char * argv[] )
{
#ifdef VITASDK
    logOutput = fopen("ux0:data/vgc.txt", "w");
#endif

	SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD,"2");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(logOutput, "SDL_Init ERROR: ");
        fprintf(logOutput, "%s", SDL_GetError());
        exit(-1);
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        fprintf(logOutput, "IMG_Init ERROR: ");
        fprintf(logOutput, "%s", IMG_GetError());
        exit(-1);
    }

    if (TTF_Init() != 0) {
        fprintf(logOutput, "TTF_Init ERROR: ");
        fprintf(logOutput, "%s", TTF_GetError());
        exit(-1);
    }

    wdw = SDL_CreateWindow("---", 50, 50, windowW, windowH, 0);

    if (wdw == NULL) {
        fprintf(logOutput, "SDL_Init ERROR: ");
        fprintf(logOutput, "%s", SDL_GetError());
        exit(-1);
    }

    rdr = SDL_CreateRenderer(wdw, -1, 0);
    SDL_SetRenderDrawBlendMode(rdr, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");

    SDL_GameController* controller = SDL_GameControllerOpen(0);

    mainIO = new IO();
    mainTextRenderer = new TextRenderer();
    mainSidebar = new Sidebar();
    mainPanel = new PanelGameConfig();

    mainIO->ParsePatchlist();
    mainIO->ParseConfigList();
    //mainIO->FindCompatibleInstalledGames();

    if (mainIO->compatibleInstalledGames.size() > 0){
        mainPanel->SwitchGame(mainIO->compatibleInstalledGames[0]);
        mainSidebar->selection = 0;
    }

    timerSinceLaunch.start();

    printf("compatible games: %u\n", mainIO->compatibleInstalledGames.size());

    if (mainIO->gameEntries.size() == 0) {
        AddPopup(PopupWindow("The patchlist does not exist or is empty.\nDownload the latest patchlist to ux0:data/VitaGrafix/patchlist.txt\nand try again.", false));
    }
    else if (mainIO->compatibleInstalledGames.size() == 0) {
        AddPopup(PopupWindow("No games compatible with VitaGrafix found." , false));
    }

    SDL_Event evt;
    while (!quit){
        uint64_t pc_start = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&evt)){
			switch (evt.type){
				case SDL_QUIT:
					SDL_Quit();
					return 0;
                case SDL_KEYDOWN:
				case SDL_CONTROLLERBUTTONDOWN:
                    if (popups.size() > 0) {
                        popups.back().TakeInput(evt);
                    }
					else if (!focusMainPanel) {
                        mainSidebar->TakeInput(evt);
                    } else {
                        mainPanel->TakeInput(evt);
                    }
					break;
			}
        }

		SDL_SetRenderDrawColor(rdr, 0,0,0,255);
		SDL_RenderClear(rdr);
        DrawBackground();

		//render
        mainPanel->Render();
        mainSidebar->Render();
		
        if (popups.size() > 0){
            popups.back().Render();
        }

		SDL_RenderPresent(rdr);

        uint64_t pc_end = SDL_GetPerformanceCounter();
        float elapsed = (pc_end - pc_start) / (float)SDL_GetPerformanceFrequency();
        float frameTime = (1.0f / 60 * 1000) - elapsed * 1000;
        if (frameTime < 0) {
            frameTime = 0;
        }
        SDL_Delay(frameTime);
        //SDL_Delay(1);
        //lastFPS = (int)(1000.0f / std::max(elapsed * 1000, 16.666f));
    }

    SDL_DestroyRenderer(rdr);
    SDL_DestroyWindow(wdw);
    delete mainIO;
    delete mainTextRenderer;
    delete mainSidebar;
    delete mainPanel;
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    exit(0);

    return 0;
}