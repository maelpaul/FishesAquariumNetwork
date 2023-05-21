package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;

public class ClearCommand implements ICommand<String> {
    public static final int COMMAND_SIZE = 1;
    @Override
    public ResultCommand<String> process(String[] args) {
        if (args.length != COMMAND_SIZE)
            return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"clear help\" pour plus d'aide");

        return new ResultCommand<String>().addAction("clearPrompt", null);
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand(
                   "> help(clear) : \n" +
                        "    clear : nettoie le terminal", "    ");
    }
}
