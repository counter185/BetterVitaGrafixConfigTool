
class IO {
public:
    std::vector<GameEntry> gameEntries;
    std::vector<uint32_t> compatibleInstalledGames; //indexes corresponding to gameEntries

    bool CheckVitaGrafixPluginInstalled();
    int ParsePatchlist();
    int ParseConfigList();
    void FindCompatibleInstalledGames();
    std::string ReadGameTitleFromSFO(TitleIDString titleID);
    SDL_Texture* GetIconForAppID(TitleIDString appID);
    GameConfig& GetGameConfigEntry(TitleIDString appID);
    bool SaveConfigTXT();
    void LaunchAppID(std::string appID);

private:
    std::map<TitleIDString, SDL_Texture*> appIcons;
    std::map<TitleIDString, GameConfig> configEntries;

    void SubmitToGameEntryList(GameEntry& baseEntry, std::vector<PatchListTarget>& patchTargets);
};