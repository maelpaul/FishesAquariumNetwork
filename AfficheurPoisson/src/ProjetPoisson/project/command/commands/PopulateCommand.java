package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;

public class PopulateCommand implements ICommand<String> {
    public static final int COMMAND_FULL_SIZE = 2;
    public static final int COMMAND_MINIMUM_SIZE = 1;

    public static final int COMMAND_ARG_NUMBER = 1;
    @Override
    public ResultCommand<String> process(String[] args) {
        if (args.length != COMMAND_MINIMUM_SIZE) {
            if (args.length != COMMAND_FULL_SIZE)
                return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"quit help\" pour plus d'aide");

            try {
                int num = Integer.parseInt(args[COMMAND_ARG_NUMBER]);

                if (num > 0 && num <= 15){
                    return new ResultCommand<String>()
                            .addAction("resultPrompt", new Object[]{ "> OK : tentative ajout " + num + " poisson(s)"})
                            .addAction("addFish", new Object[]{num});
                }

                return new PromptResultCommand("> NOK : Mauvais nombre poisson, faites \"quit help\" pour plus d'aide");
            } catch (NumberFormatException e) {
                return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"quit help\" pour plus d'aide");
            }
        }

        return new ResultCommand<String>()
                .addAction("resultPrompt", new Object[]{ "> OK : tentative ajout 15 poisson"})
                .addAction("addFish", new Object[]{15});
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand("> help(populate) : \n" +
                "    populate : ajoute 15 nouveau poisson avec des noms valides\n" +
                "   populate NUMBER  : ajoute NUMBER nouveau(x) poissons avec des noms valides\n" +
                "   [NUMBER > 0 && NUMBER <= 15]", "    ");
    }
}
