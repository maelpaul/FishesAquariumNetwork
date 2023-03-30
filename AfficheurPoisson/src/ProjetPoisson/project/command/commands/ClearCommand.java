package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;

public class ClearCommand implements ICommand {
    @Override
    public String process(String[] args) {
        return "¤¤clear¤¤";
    }

    @Override
    public String returnHelp() {
        return null;
    }
}
