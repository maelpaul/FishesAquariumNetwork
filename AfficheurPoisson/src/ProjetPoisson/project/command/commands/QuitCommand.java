package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;

public class QuitCommand implements ICommand<String> {
    @Override
    public ResultCommand<String> process(String[] args) {
        if (args.length != 1)
            return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"quit help\" pour plus d'aide");

        return new ResultCommand<String>()
                .addAction("resultPrompt", new Object[] { "> OK : Deconnexion puis fermeture application"})
                .addAction("quit", null);
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand(
                "> help(quit) : \n" +
                "    quit : termine l'application");
    }
}
