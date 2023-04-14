package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.CommandAnalyser;
import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.display.FishManager;

public class AddFishCommand implements ICommand {
    private final FishManager fishManager;
    public AddFishCommand(FishManager fishManager){
        this.fishManager = fishManager;
    }
    @Override
    public String process(String[] args) {
        if (args.length != 2 && args.length != 6)
            return "Mauvais argument(s), faites \"addFish help\" pour plus d'aide";

        if (args[1].contains("-"))
            return processOption(args);

        return "NOK";
    }

    private String processOption(String[] args){
        String option = args[1].replace("-", "");
        String result;

        if (option.equals("d")) {
            result = fishManager.getMovementType(", ", 4, "\n");
            if (result == null)
                return "Aucun mouvement enregistré !\n";

            return "Liste des types de mouvements :\n" + result;
        } else if (option.equals("n")) {
            result = fishManager.getName(", ", 4, "\n");
            if (result == null)
                return "Aucun poisson enregistré !\n";

            return "Liste des noms de poissons :\n" + result;
        }

        return "Option inconnue, faites \"addFish help\" pour plus d'aide";
    }

    @Override
    public String returnHelp() {
        return "help(addFish)" +
                "addFish arguments ... : ajoute un poisson\n" +
                "addFish -d : affiche la liste des types de déplacements\n" +
                "addFish -n : affiche la liste des noms utilisés\n";
    }
}
