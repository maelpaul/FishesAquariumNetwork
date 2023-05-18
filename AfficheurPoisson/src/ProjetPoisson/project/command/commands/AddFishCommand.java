package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.display.FishManager;
import ProjetPoisson.project.scenes.MenuScene;
import org.joml.Vector2f;

import java.util.Scanner;

public class AddFishCommand implements ICommand {
    public static final int OPTION_SIZE = 2;
    public static final int COMMAND_SIZE = 6;

    public static final int ARG_OPTION = 1;
    public static final int ARG_NAME = 1;
    public static final int ARG_AT = 2;
    public static final int ARG_POSITION = 3;
    public static final int ARG_SIZE = 4;
    public static final int ARG_BEHAVIOUR = 5;

    private final FishManager fishManager;

    private final MenuScene.ConnectionStateContainer state;
    public AddFishCommand(MenuScene.ConnectionStateContainer state, FishManager fishManager){
        this.state = state;
        this.fishManager = fishManager;
    }

    @Override
    public ResultCommand process(String[] args) {
        if (state.get() != MenuScene.EConnectionState.Connected)
            return new ResultCommand("-> NOK : Controleur introuvable");

        if (args.length == OPTION_SIZE) {
            if (args[ARG_OPTION].contains("-"))
                return processOption(args);

            return new ResultCommand("  -> NOK : Mauvais argument(s), faites \"addFish help\" pour plus d'aide");
        } else if (args.length == COMMAND_SIZE) {
            String[] positionPercentage = args[ARG_POSITION].replace(",", "").split("x");
            String[] sizePercentage = args[ARG_SIZE].replace(",", "").split("x");

            if (positionPercentage.length != 2)
                return new ResultCommand("  -> NOK : Mauvais nombre argument positions, faites \"addFish help\" pour plus d'aide");
            if (sizePercentage.length != 2)
                return new ResultCommand("  -> NOK : Mauvais nombre argument taille, faites \"addFish help\" pour plus d'aide");

            if (!isInteger(positionPercentage[0], 10) || !isInteger(positionPercentage[1], 10))
                return new ResultCommand("  -> NOK : Mauvais argument(s) positions(s), faites \"addFish help\" pour plus d'aide");

            if (!isInteger(sizePercentage[0], 10) || !isInteger(sizePercentage[1], 10))
                return new ResultCommand("  -> NOK : Mauvais argument(s) taille(s), faites \"addFish help\" pour plus d'aide");

            if (args[ARG_NAME].equalsIgnoreCase("all"))
                return new ResultCommand("  -> NOK : Un poisson ne peut pas s'appeler all");

            FishManager.EResult result = fishManager.addFish(
                    args[ARG_NAME],
                    new Vector2f(
                            Integer.parseInt(positionPercentage[0]) / 100f,
                            Integer.parseInt(positionPercentage[1]) / 100f
                    ),
                    new Vector2f(
                            Integer.parseInt(sizePercentage[0]) / 100f,
                            Integer.parseInt(sizePercentage[1]) / 100f
                    ),
                    args[ARG_BEHAVIOUR]);

            if (result == FishManager.EResult.AddErrorNameExisting)
                return new ResultCommand("  -> NOK : Nom poisson déjà existant");

            if (result == FishManager.EResult.AddErrorUnknownBehaviour)
                return new ResultCommand("  -> NOK : Nom comportement non existant");

            return new ResultCommand("  -> OK : Poisson ajouté", ResultCommand.EResultAction.SendServer);
        }

        return new ResultCommand("  -> NOK : Mauvais argument(s), faites \"addFish help\" pour plus d'aide");
    }

    private ResultCommand processOption(String[] args){
        String option = args[1].replace("-", "");
        String result;

        if (option.equals("d")) {
            result = fishManager.getMovementsTypeStr(", ", 4, "\n");
            if (result == null)
                return new ResultCommand("  -> OK : Aucun mouvement enregistré !");

            return new ResultCommand("  -> OK : Liste des types de mouvements :\n" + result);
        } else if (option.equals("n")) {
            result = fishManager.getNamesStr(", ", 4, "\n");
            if (result == null)
                return new ResultCommand("  -> OK : Aucun poisson enregistré !");

            return new ResultCommand("  -> OK : Liste des noms de poissons :\n" + result);
        }

        return new ResultCommand("  -> NOK : Option inconnue, faites \"addFish help\" pour plus d'aide");
    }

    public static boolean isInteger(String s, int radix) {
        Scanner sc = new Scanner(s.trim());
        if(!sc.hasNextInt(radix)) return false;
        // we know it starts with a valid int, now make sure
        // there's nothing left!
        sc.nextInt(radix);
        return !sc.hasNext();
    }

    @Override
    public ResultCommand returnHelp() {
        return new ResultCommand("  -> help(addFish) : \n" +
                "    addFish NAME at WIDTHxHEIGHT, XxY, BEHAVIOUR : ajoute un poisson\n" +
                "    addFish -d : affiche la liste des types de déplacements\n" +
                "    addFish -n : affiche la liste des noms utilisés");
    }
}
