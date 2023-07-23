
class Sidebar {
public:
    int selection = -1;
    double scrollY = 0;
    Timer64 lastChange;

    void PopulateSidebar();
    void Render();
    void TakeInput(SDL_Event evt);

};