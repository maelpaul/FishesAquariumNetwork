package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.display.FishManager;
import org.joml.Vector2f;

import java.util.Scanner;

public class ShowFishNameCommand implements ICommand {
    public static final int COMMAND_SIZE = 2;
    public static final int ARG_VALUE = 1;

    private final FishManager manager;
    public ShowFishNameCommand(FishManager fishManager){
        this.manager = fishManager;
    }

    @Override
    public ResultCommand process(String[] args) {
        if (args.length == COMMAND_SIZE) {
            String value = args[ARG_VALUE];

            if (value.equalsIgnoreCase("true")
                    || value.equalsIgnoreCase("t")
                    || value.equalsIgnoreCase("1") ) {
                manager.setShowName(true);
                return new ResultCommand("  -> OK : Noms poisson affichés");
            } else if (value.equalsIgnoreCase("false")
                    || value.equalsIgnoreCase("f")
                    || value.equalsIgnoreCase("0") ) {
                manager.setShowName(false);
                return new ResultCommand("  -> OK : Noms poisson cachés");
            }
        }

        return new ResultCommand("  -> NOK : Mauvais argument(s), faites \"showFishName help\" pour plus d'aide");
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
        return new ResultCommand("  -> help(showFishCommand) : \n" +
                "    showFishCommand true/false or t/f or 0/1");
    }
}
