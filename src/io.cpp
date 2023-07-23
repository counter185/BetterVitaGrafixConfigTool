#include "all.h"

bool IO::CheckVitaGrafixPluginInstalled() {
    return fs::exists("ur0:data/VitaGrafix.suprx");
}

void IO::SubmitToGameEntryList(GameEntry& baseEntry, std::vector<PatchListTarget>& patchTargets){
    for (auto& listTarget : patchTargets){
        GameEntry newEntry = baseEntry;
        newEntry.target = listTarget;

        std::string appPath = std::string(ICONS_DIR) + newEntry.target.titleID;
        if (DirectoryExists(appPath)){
            newEntry.gameName = ReadGameTitleFromSFO(newEntry.target.titleID);
            GetGameConfigEntry(newEntry.target.titleID);
            compatibleInstalledGames.push_back(gameEntries.size());
        }

        gameEntries.push_back(newEntry);
    }
    baseEntry = GameEntry();
    patchTargets.clear();
}

int IO::ParsePatchlist(){
    std::ifstream patchListStream(PATCHLIST_DIR);
    if (patchListStream.good()){

        gameEntries.clear();
        compatibleInstalledGames.clear();

        enum ReadingState {
            READING_TITLEIDS,
            READING_PATCHES
        };
        ReadingState readingState = READING_TITLEIDS;
        std::vector<PatchListTarget> validTitleIDs;
        GameEntry readingEntry;

        std::string nextLine = "";
        while (std::getline(patchListStream, nextLine)) {
            auto commentPos = nextLine.find_first_of('#');
            if (commentPos != std::string::npos){
                nextLine = nextLine.substr(0, commentPos);
            }
            
            if (nextLine.empty()){
                continue;
            }

            auto bracketOpenPos = nextLine.find_first_of('[');
            auto bracketClosePos = nextLine.find_first_of(']');
            if ((bracketOpenPos != std::string::npos || bracketClosePos != std::string::npos)
                && readingState == READING_PATCHES){
                
                SubmitToGameEntryList(readingEntry, validTitleIDs);
                readingState = READING_TITLEIDS;
            }

            if ((bracketOpenPos != std::string::npos || bracketClosePos != std::string::npos)
                && readingState == READING_TITLEIDS) {
                
                std::string trimmedTargetString = nextLine.substr(bracketOpenPos + 1, (bracketClosePos - 1) - (bracketOpenPos + 1));
                //fprintf(logOutput, "Found game target: %s\n", trimmedTargetString.c_str());
                char* str = new char[trimmedTargetString.size()+1];
                str[trimmedTargetString.size()] = '\0';
                std::string patchData[3] = {"","",""};
                int writing = 0;
                char* temp;

                memcpy(str, trimmedTargetString.c_str(), trimmedTargetString.size());
                temp = strtok(str, ",");
                while (temp != NULL && writing < 3){
                    patchData[writing++] = TrimSpacesInString(std::string(temp));
                    temp = strtok(NULL, ",");
                }
                delete[] str;

                if (writing == 3){
                    PatchListTarget newPatchListTarget;
                    newPatchListTarget.titleID = patchData[0];
                    newPatchListTarget.entryPoint = patchData[1];
                    newPatchListTarget.NID = patchData[2];
                    fprintf(logOutput, "%s | %s | %s\n", patchData[0].c_str(), patchData[1].c_str(), patchData[2].c_str());
                    validTitleIDs.push_back(newPatchListTarget);
                    
                } else {
                    fprintf(logOutput, "Too few arguments.\n");
                    continue;
                }

                
            } else if (nextLine.at(0) == '@'){
                readingState = READING_PATCHES;
                if (nextLine.size() >= 4 && nextLine.substr(0,4) == "@FPS"){
                    readingEntry.hasFPSPatch = true;
                }
                else if (nextLine.size() >= 3 && nextLine.substr(0,3) == "@IB"){
                    readingEntry.hasIBPatch = true;
                }
                else if (nextLine.size() >= 3 && nextLine.substr(0,3) == "@FB"){
                    readingEntry.hasFBPatch = true;
                }
                else if (nextLine.size() >= 4 && nextLine.substr(0,4) == "@MSAA"){
                    readingEntry.hasMSAAPatch = true;
                }
            }
        }
        if (readingState == READING_PATCHES) {
            SubmitToGameEntryList(readingEntry, validTitleIDs);
        }
        patchListStream.close();
        return 0;
    } else {
        return -1;
    }
}

int IO::ParseConfigList() {
    std::ifstream configListStream(CONFIG_DIR);
    if (configListStream.good()){

        TitleIDString readingTitleID = "";
        int appIDsRead = 0;

        std::string nextLine = "";
        while (std::getline(configListStream, nextLine)) {
            auto commentPos = nextLine.find_first_of('#');
            if (commentPos != std::string::npos){
                nextLine = nextLine.substr(0, commentPos);
            }
            while (!nextLine.empty() && (nextLine.at(nextLine.size()-1) == '\n' || nextLine.at(nextLine.size()-1) == '\r')) {
                nextLine = nextLine.substr(0, nextLine.size()-1);
            }
            
            if (nextLine.empty()){
                continue;
            }
            auto bracketOpenPos = nextLine.find_first_of('[');
            auto bracketClosePos = nextLine.find_first_of(']');
            if (bracketOpenPos != std::string::npos && bracketClosePos != std::string::npos){
                readingTitleID = nextLine.substr(bracketOpenPos+1, (bracketClosePos)-(bracketOpenPos+1));
                appIDsRead++;
            } else {
                if (nextLine.size() >= 8 && nextLine.substr(0,8) == "ENABLED="){
                    GetGameConfigEntry(readingTitleID).enabled = nextLine.substr(nextLine.find('=')+1);
                } else if (nextLine.size() >= 4 && nextLine.substr(0,4) == "OSD="){
                    GetGameConfigEntry(readingTitleID).showOSD = nextLine.substr(nextLine.find('=')+1);
                } else if (nextLine.size() >= 3 && nextLine.substr(0,3) == "FB="){
                    GetGameConfigEntry(readingTitleID).modeFB = nextLine.substr(nextLine.find('=')+1);
                } else if (nextLine.size() >= 3 && nextLine.substr(0,3) == "IB="){
                    GetGameConfigEntry(readingTitleID).modeIB = nextLine.substr(nextLine.find('=')+1);
                } else if (nextLine.size() >= 4 && nextLine.substr(0,4) == "FPS="){
                    GetGameConfigEntry(readingTitleID).modeFPS = nextLine.substr(nextLine.find('=')+1);
                } else if (nextLine.size() >= 4 && nextLine.substr(0,4) == "MSAA="){
                    GetGameConfigEntry(readingTitleID).modeFPS = nextLine.substr(nextLine.find('=')+1);
                }
            }
        }

        printf("[config.txt] Loaded %i config entries\n", appIDsRead);
        configListStream.close();
        return 0;
    } else {
        return -1;
    }
}

void IO::FindCompatibleInstalledGames() {
    compatibleInstalledGames.clear();
    for (const auto& entry : std::filesystem::directory_iterator(APP_DIR)){
        std::string path = entry.path().string();
        std::replace(path.begin(), path.end(), '\\', '/');
        while (path.at(path.size()-1) == '/'){
            path = path.substr(0, path.size()-1);
        }
        path = path.substr(path.find_last_of('/') + 1);
        auto gameEntrySearchResult = std::find_if(gameEntries.begin(), gameEntries.end(), [path](GameEntry a) { return a.target.titleID == path; });
        if (gameEntrySearchResult != gameEntries.end()){
            std::string id = path;
            uint32_t index = std::distance(gameEntries.begin(), gameEntrySearchResult);
            if (gameEntries.at(index).gameName == "") {
                gameEntries.at(index).gameName = ReadGameTitleFromSFO(path);
            }
            compatibleInstalledGames.push_back(index);
            fprintf(logOutput, "Compatible game found: %s\n", id.c_str());
        }
    }
}

SDL_Texture* IO::GetIconForAppID(TitleIDString appID) {
    if (appIcons.contains(appID)){
        return appIcons[appID];
    } else {
        std::string iconFiles[] = {"/icon0.png", "/sce_sys/icon0.png"};
        SDL_Surface* loadedImg = NULL;
        for (int x = 0; x != 2 && loadedImg == NULL; x++) {
            std::string path = ICONS_DIR + appID + iconFiles[x];
            printf("Trying to read %s\n", path.c_str());
            loadedImg = IMG_Load(path.c_str());
        }
        SDL_Texture* nTex = NULL;
        if (loadedImg != NULL){
            nTex = SDL_CreateTextureFromSurface(rdr, loadedImg);
            SDL_FreeSurface(loadedImg);
        }
        appIcons[appID] = nTex;
        return appIcons[appID];
    }
}

std::string IO::ReadGameTitleFromSFO(TitleIDString titleID) {
    std::string appDirectory = APP_DIR;
    appDirectory += "/" + titleID ;
    appDirectory += "/sce_sys/param.sfo";
    printf("Opening %s\n", appDirectory.c_str());
    std::ifstream sfoFile(appDirectory);
    if (sfoFile.good()){

        std::string ret = "???";

        SFOHeader sfoHeader;
        sfoFile.read((char*)&sfoHeader.magic, 4);
        sfoFile.read((char*)&sfoHeader.version, 4);
        sfoFile.read((char*)&sfoHeader.keyTableOffset, 4);
        sfoFile.read((char*)&sfoHeader.dataTableOffset, 4);
        sfoFile.read((char*)&sfoHeader.indexTableEntries, 4);


        sfoFile.seekg(0x14, std::ios_base::beg);
        int nIndexTableEntries = sfoHeader.indexTableEntries;
        IndexTableEntry* indexTableEntries = new IndexTableEntry[nIndexTableEntries];
        for (int x = 0; x != nIndexTableEntries; x++) {
            sfoFile.read((char*)&(indexTableEntries[x].keyOffset), 2);
            sfoFile.read((char*)&(indexTableEntries[x].param_fmt), 2);
            sfoFile.read((char*)&(indexTableEntries[x].paramLen), 4);
            sfoFile.read((char*)&(indexTableEntries[x].paramMaxLen), 4);
            sfoFile.read((char*)&(indexTableEntries[x].dataOffset), 4);
        }

        sfoFile.seekg(sfoHeader.keyTableOffset, std::ios_base::beg);
        int stringsFromKeyTableRead = 0;
        std::string currentString = "";
        while (stringsFromKeyTableRead < nIndexTableEntries){
            char ch = '\0';
            sfoFile.read(&ch, 1);
            if (ch == '\0'){
                if (currentString == "TITLE") {
                    break;
                } else {
                    fprintf(logOutput, "Found param.sfo key: %s\n", currentString.c_str());
                    currentString = "";
                    stringsFromKeyTableRead++;
                }
            } else {
                currentString += ch;
            }
        }
        if (stringsFromKeyTableRead != nIndexTableEntries){
            fprintf(logOutput, "TITLE key found at index %u\n", stringsFromKeyTableRead);
            sfoFile.seekg(sfoHeader.dataTableOffset + indexTableEntries[stringsFromKeyTableRead].dataOffset, std::ios_base::beg);
            int titleLen = indexTableEntries[stringsFromKeyTableRead].paramLen;
            char* title = new char[titleLen+1];
            title[titleLen] = '\0';
            sfoFile.read(title, titleLen);
            fprintf(logOutput, "%u %u %s\n", titleLen, indexTableEntries[stringsFromKeyTableRead].paramMaxLen, title);
            ret = std::string(title);
            delete[] title;

        } else {
            fprintf(logOutput, "TITLE key not found\n");
        }

        delete[] indexTableEntries;
        sfoFile.close();

        return ret;
        
    } else {
        return "---";
    }
}

GameConfig& IO::GetGameConfigEntry(TitleIDString appID) {
    if (!configEntries.contains(appID)){
        configEntries[appID] = GameConfig();
    }
    return configEntries[appID];
}

bool IO::SaveConfigTXT() {
    printf("SaveConfigTXT()");
    FILE* file = fopen(CONFIG_DIR, "w");
    if (file != NULL){

        fprintf(file, "[MAIN]\n");
        fprintf(file, "ENABLED=%i\n", 1);
        fprintf(file, "OSD=%i\n", 1);
        fprintf(file, "\n");

        for (auto& gameEntry : gameEntries){
            if (configEntries.contains(gameEntry.target.titleID)) {
                GameConfig configEntry = configEntries[gameEntry.target.titleID];
                fprintf(file, "[%s]\n", gameEntry.target.titleID.c_str());
                fprintf(file, "ENABLED=%i\n", configEntry.enabled == "1" ? 1 : 0);
                fprintf(file, "OSD=%i\n", configEntry.showOSD == "1" ? 1 : 0);
                if (gameEntry.hasFBPatch){
                    fprintf(file, "FB=%s\n", configEntry.modeFB.c_str());
                }
                if (gameEntry.hasIBPatch) {
                    fprintf(file, "IB=%s\n", configEntry.modeIB.c_str());
                }
                if (gameEntry.hasFPSPatch) {
                    fprintf(file, "FPS=%s\n", configEntry.modeFPS.c_str());
                }
                if (gameEntry.hasMSAAPatch) {
                    fprintf(file, "MSAA=%s\n", configEntry.modeMSAA.c_str());
                }
                fprintf(file, "\n");
            }
        }
        fclose(file);
        printf("Save complete");
        return true;
    }
    printf("Save failed.");
    return false;
}

void IO::LaunchAppID(std::string appID) {
#ifdef VITASDK
    char param[1024];
    memset(param, 0, 1024);
    sceAppMgrGetAppParam(param);
    sceAppMgrLaunchAppByName(0x60000, appID.c_str(), param);
#endif
}