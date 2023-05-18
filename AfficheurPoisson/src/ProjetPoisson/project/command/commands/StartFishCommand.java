package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.display.FishManager;

public class StartFishCommand implements ICommand {

    public static final int COMMAND_SIZE = 2;

    public static final int ARG_NAME = 1;

    private final FishManager fishManager;

    public StartFishCommand(FishManager fishManager){
        this.fishManager = fishManager;
    }

    @Override
    public String process(String[] args) {
        if (args.length == COMMAND_SIZE) {
            if (args[ARG_NAME].equalsIgnoreCase("all")){
                fishManager.startFish("all");

                return "  -> OK : Tout les poissons sont demarrés";
            }

            FishManager.EResult result = fishManager.startFish(args[ARG_NAME]);

            if (result == FishManager.EResult.StartErrorUnknownName)
                return "  -> NOK : Nom inconnue";

            return "  -> OK : Poisson " + args[ARG_NAME] + " demarré";
        }

        return "  -> NOK : Mauvais argument(s), faites \"startFish help\" pour plus d'aide";
    }

    @Override
    public String returnHelp() {
        return "  -> help(startFish) : \n" +
                "    startFish NAME : démarre le poisson nommé NAME,\n" +
                "    startFish all : démarre tout les poissons";
    }
}
