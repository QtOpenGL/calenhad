//
// Created by martin on 28/04/17.
//

#include <iostream>
#include "CommandGroup.h"
using namespace calenhad::actions;
CommandGroup::CommandGroup() {

}

CommandGroup::~CommandGroup() {
    for (int i = 0; i < _commands.size(); i++) {
        delete _commands.at (i);
    }
}

void CommandGroup::redo() {
    for (int i = _commands.size() - 1; i >= 0; i--) {
        QUndoCommand* c = _commands.at (i);
        std::cout << "Redo " << c -> text ().toStdString () << "\n";
        c -> redo();
    }
}

void CommandGroup::undo () {
    for (int i = 0; i < _commands.size(); i++) {
        QUndoCommand* c = _commands.at (i);
        std::cout << "Undo " << c -> text ().toStdString () << "\n";
        c -> undo();
    }
}


void CommandGroup::addCommand (QUndoCommand* command) {
    _commands.append (command);
}
