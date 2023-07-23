
const int N_PANELGAMECONFIG_SELECTIONS = 8;

class PanelGameConfig : public DropdownReceiver{
public:
    void SwitchGame(int indexInMainIO);

    void Render();
    void TakeInput(SDL_Event evt);
    void InputAccept();
private:
    int currentGameIndex = -1;
    int selection = 0;
    Dropdown* currentDropdown = NULL;

    void ReceiveDropdownEvent(int from, std::string data) override;
};