package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.display.FishManager;
import ProjetPoisson.project.scenes.MenuScene;

public class StartFishCommand implements ICommand {

    public static final int COMMAND_SIZE = 2;

    public static final int ARG_NAME = 1;

    private final MenuScene.ConnectionStateContainer state;
    private final FishManager fishManager;

    public StartFishCommand(MenuScene.ConnectionStateContainer state, FishManager fishManager) {
        this.state = state;
        this.fishManager = fishManager;
    }

    @Override
    public ResultCommand process(String[] args) {
        if (state.get() != MenuScene.EConnectionState.Connected)
            return new ResultCommand("> NOK : Controleur introuvable");

        if (args.length == COMMAND_SIZE) {
            if (args[ARG_NAME].equalsIgnoreCase("all")){
                fishManager.startFish("all");

                return new ResultCommand("> OK : Tout les poissons sont demarrés");
            }

            FishManager.EResult result = fishManager.startFish(args[ARG_NAME]);

            if (result == FishManager.EResult.StartErrorUnknownName)
                return new ResultCommand("> NOK : Nom inconnue");

            return new ResultCommand("> OK : Poisson " + args[ARG_NAME] + " demarré", ResultCommand.EResultAction.SendServer);
        }

        return new ResultCommand("> NOK : Mauvais argument(s), faites \"startFish help\" pour plus d'aide");
    }

    @Override
    public ResultCommand returnHelp() {
        return new ResultCommand("> help(startFish) : \n" +
                "    startFish NAME : démarre le poisson nommé NAME,\n" +
                "    startFish all : démarre tout les poissons");
    }
}
