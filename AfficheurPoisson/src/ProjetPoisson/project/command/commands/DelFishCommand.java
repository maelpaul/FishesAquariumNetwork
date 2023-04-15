package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.display.FishManager;

public class DelFishCommand implements ICommand {
    public static final int COMMAND_SIZE = 2;

    public static final int ARG_NAME = 1;

    private final FishManager fishManager;
    public DelFishCommand(FishManager fishManager){
        this.fishManager = fishManager;
    }

    @Override
    public String process(String[] args) {
        if (args.length == COMMAND_SIZE) {
            FishManager.EResult result = fishManager.delFish(args[ARG_NAME]);

            if (result == FishManager.EResult.DeleteErrorNameNotExisting)
                return "  -> NOK : Poisson inexistant";

            return "  -> OK : Poisson " + args[ARG_NAME] + " retiré";
        }

        return "  -> NOK : Mauvais argument(s), faites \"delFish help\" pour plus d'aide";
    }

    @Override
    public String returnHelp() {
        return "  -> help(delFish) : \n" +
                "    delFish NAME\n";
    }
}
