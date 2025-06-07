#include "app_constraint.h"

int main() {
    AppConstraint app;
    app.setup();

    while (app.is_running()) {
        app.input();
        app.update();
        app.render();
    }

    app.destroy();

    return 0;
}
