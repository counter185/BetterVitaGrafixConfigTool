
class PopupWindow {
public:
    PopupWindow(std::string showText, bool canCancel = true) {
        text = showText;
        isCancellable = canCancel;
        startTimer.start();
    }
    void Render();
    void TakeInput(SDL_Event evt);
private:
    bool isCancellable;
    std::string text = "";
    Timer64 startTimer;
};