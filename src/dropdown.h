
const int DROPDOWN_WIDTH = 300;
const int DROPDOWN_ITEM_HEIGHT = 26;

class Dropdown {
public:
    Dropdown(int receiveID, DropdownReceiver* receiveTarget, std::vector<std::string> optionsList, int x, int y, bool allowCustomOption = false) {
        id = receiveID;
        target = receiveTarget;
        options = optionsList;
        hasCustomOption = allowCustomOption;
        posX = x;
        posY = y;
        startTime.start();
    }

    void Render();
    void TakeInput(SDL_Event evt);
private:
    int id;
    DropdownReceiver* target = NULL;
    std::vector<std::string> options;
    bool hasCustomOption = false;

    int posX, posY;

    int selection = 0;
    Timer64 startTime;
};

class DropdownReceiver {
public:
    virtual void ReceiveDropdownEvent(int from, std::string data) {}
};