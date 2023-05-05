package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.display.FishManager;
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
    public AddFishCommand(FishManager fishManager){
        this.fishManager = fishManager;
    }

    @Override
    public String process(String[] args) {
        if (args.length == OPTION_SIZE) {
            if (args[ARG_OPTION].contains("-"))
                return processOption(args);

            return "  -> NOK : Mauvais argument(s), faites \"addFish help\" pour plus d'aide";
        } else if (args.length == COMMAND_SIZE) {
            String[] positionPercentage = args[ARG_POSITION].replace(",", "").split("x");
            String[] sizePercentage = args[ARG_SIZE].replace(",", "").split("x");

            if (positionPercentage.length != 2)
                return "  -> NOK : Mauvais nombre argument positions, faites \"addFish help\" pour plus d'aide";
            if (sizePercentage.length != 2)
                return "  -> NOK : Mauvais nombre argument taille, faites \"addFish help\" pour plus d'aide";

            if (!isInteger(positionPercentage[0], 10) || !isInteger(positionPercentage[1], 10))
                return "  -> NOK : Mauvais argument(s) positions(s), faites \"addFish help\" pour plus d'aide";

            if (!isInteger(sizePercentage[0], 10) || !isInteger(sizePercentage[1], 10))
                return "  -> NOK : Mauvais argument(s) taille(s), faites \"addFish help\" pour plus d'aide";

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
                return "  -> NOK : Nom poisson déjà existant";

            return "  -> OK : Poisson ajouté";
        }

        return "  -> NOK : Mauvais argument(s), faites \"addFish help\" pour plus d'aide";
    }

    private String processOption(String[] args){
        String option = args[1].replace("-", "");
        String result;

        if (option.equals("d")) {
            result = fishManager.getMovementType(", ", 4, "\n");
            if (result == null)
                return "  -> OK : Aucun mouvement enregistré !\n";

            return "  -> OK : Liste des types de mouvements :\n" + result;
        } else if (option.equals("n")) {
            result = fishManager.getName(", ", 4, "\n");
            if (result == null)
                return "  -> OK : Aucun poisson enregistré !\n";

            return "  -> OK : Liste des noms de poissons :\n" + result;
        } else if (option.equals("b")){

        }

        return "  -> NOK : Option inconnue, faites \"addFish help\" pour plus d'aide";
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
    public String returnHelp() {
        return "  -> help(addFish) : \n" +
                "    addFish NAME at WIDTHxHEIGHT, XxY, BEHAVIOUR : ajoute un poisson\n" +
                "    addFish -d : affiche la liste des types de déplacements\n" +
                "    addFish -n : affiche la liste des noms utilisés";
    }
}
