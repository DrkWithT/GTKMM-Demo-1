#include <iostream>
#include <gtkmm.h>

class MyWindow : public Gtk::Window
{
private:
    // todo
public:
    MyWindow()
    {
        set_title("Demo1");
        set_default_size(200, 200);
    }
};

int main(int argc, char* argv[]) {
    Gtk::Main kit(argc, argv);

    MyWindow window {};

    Gtk::Main::run(window);
}
