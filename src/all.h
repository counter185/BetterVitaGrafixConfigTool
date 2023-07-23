
#include <new>
#include <string>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>
#include <filesystem>

#ifdef VITASDK
#include <psp2/appmgr.h>
#endif

#define fs std::filesystem
typedef std::string TitleIDString;

#ifdef VITASDK
#define FONT_PATH "app0:app_font.ttf"
#define FONT_PATH_JP "app0:app_font_jp.ttf"
#define ICONS_DIR "ur0:/appmeta/"
#define APP_DIR "ux0:/app"
#define PATCHLIST_DIR "ux0:data/VitaGrafix/patchlist.txt"
#define CONFIG_DIR "ux0:data/VitaGrafix/config.txt"
#else
#define FONT_PATH "app_font.ttf"
#define FONT_PATH_JP "app_font_jp.ttf"
// define these paths like this if you want to test on a PC build:
/*#define ICONS_DIR "C:/Users/user/AppData/Roaming/Vita3K/Vita3K/ux0/app/"
#define APP_DIR "C:/Users/user/AppData/Roaming/Vita3K/Vita3K/ux0/app"
#define PATCHLIST_DIR "C:/Users/user/AppData/Roaming/Vita3K/Vita3K/ux0/data/VitaGrafix/patchlist.txt"
#define CONFIG_DIR "C:/Users/user/AppData/Roaming/Vita3K/Vita3K/ux0/data/VitaGrafix/config.txt"*/
#endif


#ifdef _MSC_BUILD
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif

// class/struct prototypes
struct PatchListTarget;
struct GameEntry;
struct GameConfig;
struct Timer64;
class IO;
class Sidebar;
class PanelGameConfig;
class TextRenderer;
class Dropdown;
class DropdownReceiver;
class PopupWindow;

// globals
extern IO* mainIO;
extern TextRenderer* mainTextRenderer;
extern Sidebar* mainSidebar;
extern PanelGameConfig* mainPanel;
extern SDL_Renderer* rdr;
extern SDL_Window* wdw;
extern int windowW;
extern int windowH;
extern FILE* logOutput;
extern bool focusMainPanel;
extern Timer64 timerSinceLaunch;
extern Timer64 timerSinceFocusSwitch;

// global functions
void SwitchToMainPanel();
void SwitchToSidebar();
void PopupCancel();
void AddPopup(PopupWindow target);

#include "structs.h"
#include "utils.h"
#include "io.h"
#include "sidebar.h"
#include "textrenderer.h"
#include "dropdown.h"
#include "panel_gameconfig.h"
#include "popup.h"