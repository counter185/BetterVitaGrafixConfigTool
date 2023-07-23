
struct PatchListTarget {
    TitleIDString titleID;
    std::string entryPoint;
    std::string NID;
};

struct GameEntry { // patch list entry
    std::string gameName = "";
    PatchListTarget target;
    bool hasFPSPatch = false;
    bool hasIBPatch = false;
    bool hasFBPatch = false;
    bool hasMSAAPatch = false;
};

struct GameConfig {
    std::string enabled = "1";
    std::string showOSD = "1";

    std::string modeFPS = "OFF";
    std::string modeIB = "OFF";
    std::string modeFB = "OFF";
    std::string modeMSAA = "OFF";
};

struct GlyphData {
    int minx = 0, miny = 0, maxx = 0, maxy = 0, advance = 0;
    int w = 0, h = 0;
    SDL_Texture* texture = NULL;
};

struct SFOHeader {
    uint32_t magic;         //must be 00 50 53 46 (PSF)
    uint32_t version;       //01 01 00 00 (1.1)
    uint32_t keyTableOffset;
    uint32_t dataTableOffset;
    uint32_t indexTableEntries;
};

struct IndexTableEntry {
    uint16_t keyOffset; //offset of keytable + keyOffset
    uint16_t param_fmt; //enum (see below)
    uint32_t paramLen;
    uint32_t paramMaxLen;
    uint32_t dataOffset; //offset of datatable + dataOffset
};

class Timer64 {
public:
    bool started = false;
    uint64_t startTime = 0;
    
    Timer64() {}
    void stop() { started = false; }
    void start() { 
        started = true;
        startTime = SDL_GetTicks64(); 
    }
    int64_t ticksElapsed() { return SDL_GetTicks64() - startTime; }
    float getPercent(float time, bool clampTo1 = true) { return clampTo1 ? std::min(1.0f, ticksElapsed() / time) : (startTime / time); }
};