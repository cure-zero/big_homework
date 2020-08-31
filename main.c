#include <stdio.h>
#include "course_select_system.h"

course_select_system*  Course_select_system;

int main() {
    init();
    Course_select_system->login();
    return 0;
}
