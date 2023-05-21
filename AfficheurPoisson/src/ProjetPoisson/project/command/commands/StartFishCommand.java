package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.fish.FishManager;
import ProjetPoisson.project.scenes.MenuScene;

public class StartFishCommand implements ICommand<String> {

    public static final int COMMAND_SIZE = 2;

    public static final int ARG_NAME = 1;

    private final MenuScene.ConnectionStateContainer state;
    private final FishManager fishManager;

    public StartFishCommand(MenuScene.ConnectionStateContainer state, FishManager fishManager) {
        this.state = state;
        this.fishManager = fishManager;
    }

    @Override
    public ResultCommand<String> process(String[] args) {
        if (state.get() != MenuScene.EConnectionState.Connected)
            return new PromptResultCommand("> NOK : Controleur introuvable");

        if (args.length == COMMAND_SIZE) {
            for (String name : fishManager.getFishNames()) {
                if (name.equals(args[ARG_NAME]))
                    return new ResultCommand<String>()
                            .addAction("resultPrompt", new Object[] { "> OK : Demande au serveur pour demarrer Poisson" })
                            .addAction("runCommand", null)
                            .addAction("showErrorResult", null)
                            .addAction("showSuccessResult",  null);
            }

            return new PromptResultCommand("> NOK : Nom inconnue");

        }

        return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"startFish help\" pour plus d'aide");
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand("> help(startFish) : \n" +
                "    startFish NAME : démarre le poisson nommé NAME,\n" +
                "    startFish all : démarre tout les poissons", "    ");
    }
}
